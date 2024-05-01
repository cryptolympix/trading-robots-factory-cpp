#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <filesystem>
#include "../libs/json.hpp"
#include "math_utils.hpp"
#include "config.hpp"
#include "node.hpp"
#include "connection_gene.hpp"
#include "connection_history.hpp"
#include "genome.hpp"

std::string generate_uid_for_genome(int size)
{
    const std::string characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    thread_local std::random_device rd;
    thread_local std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, characters.size() - 1);

    std::string uid;
    uid.reserve(size); // Reserve space to avoid reallocation inside the loop

    for (int i = 0; i < size; ++i)
    {
        uid += characters[dis(gen)];
    }

    return uid;
}

int next_innovation_nb = 1;

Genome::Genome(){};

Genome::Genome(const NeatConfig &config, bool crossover) : config(config), inputs(config.num_inputs), outputs(config.num_outputs), layers(2), next_node(0), fitness(0)
{
    id = generate_uid_for_genome(8);

    if (crossover)
    {
        return;
    }

    // create input nodes
    for (int i = 0; i < inputs; ++i)
    {
        nodes.push_back(std::make_shared<Node>(i, config.activation_default, 0));
        ++next_node;
    }

    // create output nodes
    for (int i = 0; i < outputs; ++i)
    {
        nodes.push_back(std::make_shared<Node>(i + inputs, config.activation_default, 1));
        ++next_node;
    }
}

void Genome::fully_connect(std::vector<std::shared_ptr<ConnectionHistory>> innovation_history)
{
    for (int i = 0; i < inputs; ++i)
    {
        for (int j = 0; j < outputs; ++j)
        {
            int connection_innovation_nb = get_innovation_number(
                innovation_history,
                nodes[i],
                nodes[inputs + j]);

            genes.push_back(
                std::make_shared<ConnectionGene>(
                    nodes[i],
                    nodes[inputs + j],
                    new_connection_weight(),
                    connection_innovation_nb,
                    config.enabled_default));
        }
    }

    connect_nodes();
}

std::shared_ptr<Node> Genome::get_node(int id)
{
    for (auto &n : nodes)
    {
        if (n->id == id)
        {
            return n;
        }
    }
    return nullptr;
}

void Genome::connect_nodes()
{
    // Clear the connections for each node
    for (auto &n : nodes)
    {
        n->output_connections.clear();
    }

    // Add the connections to the nodes
    for (auto &g : genes)
    {
        g->from_node->output_connections.push_back(g);
    }
}

std::vector<double> Genome::feed_forward(std::vector<double> input_values)
{
    try
    {
        if (inputs != static_cast<int>(input_values.size()))
        {
            throw std::invalid_argument("The number of inputs must match the number of input nodes. Should be " + std::to_string(input_values.size()));
        }

        // Set the outputs of the input nodes
        for (int i = 0; i < inputs; ++i)
        {
            nodes[i]->output_value = input_values[i];
        }

        // Engage each node in the network
        for (auto &n : network)
        {
            n->activate();
            n->propagate_output();
        }

        // The outputs are nodes[inputs] to nodes[inputs+outputs-1]
        std::vector<double> outs(outputs, 0.0);
        for (int i = 0; i < outputs; ++i)
        {
            // Store the outputs of the output nodes
            outs[i] = nodes[inputs + i]->output_value;
        }

        // Reset all the nodes for the next feed forward
        for (auto &n : nodes)
        {
            n->input_sum = 0;
        }

        return outs;
    }
    catch (const std::exception &e)
    {
        std::cerr << "An error occurred during feed forward: " << e.what() << std::endl;
        return std::vector<double>{};
    }
}

void Genome::generate_network()
{
    connect_nodes();
    network.clear();

    // For each layer, add the nodes in that layer to the network
    for (int l = 0; l < layers; ++l)
    {
        for (auto &n : nodes)
        {
            if (n->layer == l)
            {
                network.push_back(n);
            }
        }
    }
}

void Genome::add_node(std::vector<std::shared_ptr<ConnectionHistory>> innovation_history)
{
    // Pick a random connection to create a node between
    if (genes.empty())
    {
        add_connection(innovation_history);
        return;
    }

    int random_connection = randrange(0, genes.size());
    genes[random_connection]->enabled = false; // Disable the connection

    int new_node_nb = next_node;
    int layer = genes[random_connection]->from_node->layer + 1;

    std::shared_ptr<Node> new_node = std::make_shared<Node>(new_node_nb, config.activation_default, layer);
    nodes.push_back(new_node);
    ++next_node;

    // Add a new connection to the new node with a weight of 1
    int connection_innovation_nb = get_innovation_number(
        innovation_history,
        genes[random_connection]->from_node,
        new_node);

    genes.push_back(
        std::make_shared<ConnectionGene>(
            genes[random_connection]->from_node,
            new_node,
            1,
            connection_innovation_nb,
            config.enabled_default));

    connection_innovation_nb = get_innovation_number(
        innovation_history,
        new_node,
        genes[random_connection]->to_node);

    // Add a new connection from the new node with a weight the same as the disabled connection
    genes.push_back(
        std::make_shared<ConnectionGene>(
            new_node,
            genes[random_connection]->to_node,
            genes[random_connection]->weight,
            connection_innovation_nb,
            config.enabled_default));

    // If the layer of the new node is equal to the layer of the output node of the old connection,
    // then a new layer needs to be created
    if (new_node->layer == genes[random_connection]->to_node->layer)
    {
        for (size_t i = 0; i < nodes.size() - 1; ++i)
        {
            // Don't include the new node
            if (nodes[i]->layer >= new_node->layer)
            {
                nodes[i]->layer += 1;
            }
        }
        ++layers;
    }

    connect_nodes();
}

void Genome::remove_node()
{
    // Select a random node by excluding inputs, outputs
    auto it = std::find_if(nodes.begin(), nodes.end(), [&](const std::shared_ptr<Node> n)
                           { return n->layer != 0 && n->layer != layers; });

    if (it != nodes.end())
    {
        std::shared_ptr<Node> random_node = *it;
        nodes.erase(it);

        // Remove the connections that are connected to the random node selected
        auto removeCondition = [&](const std::shared_ptr<ConnectionGene> g)
        {
            return g->from_node == random_node || g->to_node == random_node;
        };

        genes.erase(std::remove_if(genes.begin(), genes.end(), removeCondition), genes.end());
    }
}

void Genome::add_connection(std::vector<std::shared_ptr<ConnectionHistory>> innovation_history)
{
    // Cannot add a connection to a fully connected network
    if (fully_connected())
    {
        return;
    }

    auto random_connection_nodes_are_valid = [&](int rand1, int rand2)
    {
        return (nodes[rand1]->layer != nodes[rand2]->layer) && !nodes[rand1]->is_connected_to(nodes[rand2]);
    };

    // Get random nodes
    int random_node_1 = rand() % nodes.size();
    int random_node_2 = rand() % nodes.size();

    while (!random_connection_nodes_are_valid(random_node_1, random_node_2))
    {
        // While the random nodes are not valid, get new ones
        random_node_1 = rand() % nodes.size();
        random_node_2 = rand() % nodes.size();
    }

    int temp;
    if (nodes[random_node_1]->layer > nodes[random_node_2]->layer)
    {
        // If the first random node is after the second, then switch
        temp = random_node_2;
        random_node_2 = random_node_1;
        random_node_1 = temp;
    }

    // Get the innovation number of the connection
    // This will be a new number if no identical genome has mutated in the same way
    int connection_innovation_nb = get_innovation_number(
        innovation_history,
        nodes[random_node_1],
        nodes[random_node_2]);

    // Add the connection with a random weight
    genes.push_back(
        std::make_shared<ConnectionGene>(
            nodes[random_node_1],
            nodes[random_node_2],
            new_connection_weight(),
            connection_innovation_nb,
            config.enabled_default));

    connect_nodes();
}

void Genome::remove_connection()
{
    if (!genes.empty())
    {
        auto random_gene = genes.begin() + randrange(0, genes.size());
        genes.erase(random_gene);
    }
}

double Genome::new_connection_weight() const
{
    double weight = 0.0;

    if (config.weight_init_type == "normal")
    {
        weight = normal(config.weight_init_mean, config.weight_init_stdev);

        // Keep the value between bounds
        if (weight > config.weight_max_value)
        {
            weight = config.weight_max_value;
        }
        else if (weight < config.weight_min_value)
        {
            weight = config.weight_min_value;
        }
    }
    else if (config.weight_init_type == "uniform")
    {
        weight = uniform(config.weight_min_value, config.weight_max_value);
    }

    return weight;
}

int Genome::get_innovation_number(std::vector<std::shared_ptr<ConnectionHistory>> innovation_history, std::shared_ptr<Node> from_node, std::shared_ptr<Node> to_node) const
{
    bool is_new = true;
    int connection_innovation_nb = next_innovation_nb;

    for (auto &history : innovation_history)
    {
        // For each previous mutation
        if (history->matches(from_node, to_node))
        {
            // If a match is found
            is_new = false; // It's not a new mutation
            connection_innovation_nb = history->innovation_nb;
            break;
        }
    }

    if (is_new)
    {
        // Add this mutation to the innovation history
        innovation_history.push_back(std::make_shared<ConnectionHistory>(from_node, to_node, connection_innovation_nb));
        ++next_innovation_nb;
    }

    return connection_innovation_nb;
}

bool Genome::fully_connected() const
{
    int max_connections = 0;

    // Array that stores the number of nodes in each layer
    std::vector<int> nodes_in_layers(layers, 0);

    // Populate the array
    for (const auto &node : nodes)
    {
        ++nodes_in_layers[node->layer];
    }

    // For each layer, calculate the maximum number of connections
    for (int i = 0; i < layers - 1; ++i)
    {
        int nodes_in_front = 0;
        for (int j = i + 1; j < layers; ++j)
        {
            // For each layer in front of this layer
            nodes_in_front += nodes_in_layers[j]; // Add up nodes
        }

        max_connections += nodes_in_layers[i] * nodes_in_front;
    }

    return max_connections <= static_cast<int>(genes.size());
}

void Genome::mutate(std::vector<std::shared_ptr<ConnectionHistory>> innovation_history)
{
    try
    {
        if (genes.empty())
        {
            add_connection(innovation_history);
        }

        for (auto &node : nodes)
        {
            node->mutate(config);
        }

        for (auto &gene : genes)
        {
            gene->mutate(config);
        }

        if (randrange() < config.conn_add_prob)
        {
            add_connection(innovation_history);
        }

        if (randrange() < config.conn_delete_prob)
        {
            remove_connection();
        }

        if (randrange() < config.node_add_prob)
        {
            add_node(innovation_history);
        }

        // if (randrange() < config.node_delete_prob)
        // {
        //     remove_node();
        // }
    }
    catch (const std::exception &e)
    {
        std::cerr << "An error occurred during mutation: " << e.what() << std::endl;
    }
}

Genome *Genome::crossover(Genome *parent) const
{
    Genome *child = new Genome(config, true);
    child->genes.clear();
    child->nodes.clear();
    child->layers = layers;
    child->next_node = next_node;

    std::vector<std::shared_ptr<ConnectionGene>> child_genes;
    std::vector<bool> is_enabled;

    // All inherited genes
    for (const auto &gene : genes)
    {
        bool set_enabled = true; // Is this connection in the child going to be enabled

        int parent_gene_index = matching_gene(parent, gene->innovation_nb);
        if (parent_gene_index != -1)
        {
            // If the genes match
            if (!gene->enabled || !parent->genes[parent_gene_index]->enabled)
            {
                // If either of the matching genes is disabled
                if (randrange() < 0.75)
                {
                    // 75% of the time disable the child gene
                    set_enabled = false;
                }
            }

            if (randrange() < 0.5)
            {
                child_genes.push_back(gene);
            }
            else
            {
                // Get gene from the parent
                child_genes.push_back(parent->genes[parent_gene_index]);
            }
        }
        else
        {
            // Disjoint or excess gene
            child_genes.push_back(gene);
            set_enabled = gene->enabled;
        }

        is_enabled.push_back(set_enabled);
    }

    // Since all excess and disjoint genes are inherited from the more fit parent (this Genome),
    // the child structure is no different from this parent, except for dormant connections being enabled,
    // but this won't affect nodes.
    // So, all the nodes can be inherited from this parent.
    for (auto &node : nodes)
    {
        child->nodes.push_back(node->clone());
    }

    // Clone all the connections so that they connect the child new nodes
    for (size_t i = 0; i < child_genes.size(); ++i)
    {
        child->genes.push_back(child_genes[i]->clone(
            child->get_node(child_genes[i]->from_node->id),
            child->get_node(child_genes[i]->to_node->id)));
        child->genes[i]->enabled = is_enabled[i];
    }

    child->connect_nodes();
    return child;
}

int Genome::matching_gene(Genome *parent, int innovation) const
{
    for (size_t i = 0; i < parent->genes.size(); ++i)
    {
        if (parent->genes[i]->innovation_nb == innovation)
        {
            return i;
        }
    }

    return -1; // No matching gene found
}

void Genome::print() const
{
    std::cout << "------------------------------ GENOME ----------------------------\n";
    std::cout << "⚪️ Resume: {"
              << "layers: " << layers
              << ", nodes: " << nodes.size()
              << "}\n";
    std::cout << "⚪️ Connection genes:\n";

    for (const auto &gene : genes)
    {
        std::cout << "{"
                  << "innovation_nb: " << gene->innovation_nb << ", "
                  << "from_node: " << gene->from_node->id << ", "
                  << "to_node: " << gene->to_node->id << ", "
                  << "enabled: " << gene->enabled << ", "
                  << "from_layer: " << gene->from_node->layer << ", "
                  << "to_layer: " << gene->to_node->layer << ", "
                  << "weight: " << gene->weight << "}\n";
    }

    std::cout << std::endl;
}

bool Genome::is_equal(Genome *other)
{
    // Compare the number of nodes
    if (nodes.size() != other->nodes.size())
    {
        return false;
    }

    // Compare the number of genes
    if (genes.size() != other->genes.size())
    {
        return false;
    }

    // Compare the nodes
    for (auto &node1 : nodes)
    {
        bool found = false;
        for (auto &node2 : other->nodes)
        {
            if (node1->is_equal(node2))
            {
                found = true;
                break;
            }
        }
        if (found == false)
        {
            return false;
        }
    }

    // Compare the genes
    for (auto &gene1 : genes)
    {
        bool found = false;
        for (auto &gene2 : other->genes)
        {
            if (gene1->is_equal(gene2))
            {
                found = true;
                break;
            }
        }
        if (found == false)
        {
            return false;
        }
    }

    return true;
}

Genome *Genome::clone()
{
    Genome *clone = new Genome(config, true);

    // Copy nodes
    for (auto &node : nodes)
    {
        clone->nodes.push_back(node->clone());
    }

    // Copy genes and connect them to the clone new nodes
    for (auto &gene : genes)
    {
        clone->genes.push_back(gene->clone(
            clone->get_node(gene->from_node->id),
            clone->get_node(gene->to_node->id)));
    }

    clone->layers = layers;
    clone->next_node = next_node;
    clone->fitness = fitness;
    clone->connect_nodes();

    return clone;
}

void Genome::save(const std::string &file_path)
{
    try
    {
        // Check if the directory exists, create it if not
        std::filesystem::path dir = std::filesystem::path(file_path).parent_path();
        if (!std::filesystem::exists(dir))
        {
            std::filesystem::create_directories(dir);
        }

        // If the file has no extension, append ".json"
        std::filesystem::path path = file_path;
        if (path.extension().empty())
        {
            path += ".json";
        }

        // Create a JSON object to hold the genome data
        nlohmann::json genome_json;

        // Serialize simple data members
        genome_json["id"] = id;
        genome_json["inputs"] = inputs;
        genome_json["outputs"] = outputs;
        genome_json["layers"] = layers;
        genome_json["next_node"] = next_node;
        genome_json["fitness"] = fitness;

        // Serialize nodes
        nlohmann::json nodes_json;
        for (const auto &node : nodes)
        {
            nlohmann::json node_json;
            // Serialize node data
            node_json["id"] = node->id;
            node_json["layer"] = node->layer;
            node_json["activation_function"] = node->activation_function;
            // Add node to nodes array
            nodes_json.push_back(node_json);
        }
        genome_json["nodes"] = nodes_json;

        // Serialize genes
        nlohmann::json genes_json;
        for (const auto &gene : genes)
        {
            nlohmann::json gene_json;
            // Serialize gene data
            gene_json["innovation_nb"] = gene->innovation_nb;
            gene_json["from_node_id"] = gene->from_node->id;
            gene_json["to_node_id"] = gene->to_node->id;
            gene_json["enabled"] = gene->enabled;
            gene_json["weight"] = gene->weight;
            // Add gene to genes array
            genes_json.push_back(gene_json);
        }
        genome_json["genes"] = genes_json;

        // Write JSON to file
        std::ofstream file(file_path);
        if (!file.is_open())
        {
            throw std::runtime_error("Failed to open file for writing: " + file_path);
        }
        file << genome_json.dump(4); // Pretty-print with 4 spaces
        file.close();                // Close the file after writing
    }
    catch (const std::exception &e)
    {
        std::cerr << "Failed to save genome: " << e.what() << std::endl;
    }
}

Genome *Genome::load(const std::string &file_path)
{
    try
    {
        // Read JSON from file
        std::ifstream file(file_path);
        nlohmann::json genome_json;
        file >> genome_json;

        // Create a new Genome object
        Genome *loaded_genome = new Genome;

        // Deserialize simple data members
        loaded_genome->id = genome_json["id"];
        loaded_genome->inputs = genome_json["inputs"];
        loaded_genome->outputs = genome_json["outputs"];
        loaded_genome->layers = genome_json["layers"];
        loaded_genome->next_node = genome_json["next_node"];
        loaded_genome->fitness = genome_json["fitness"];

        // Deserialize nodes
        for (const auto &node_json : genome_json["nodes"])
        {
            int id = node_json["id"];
            int layer = node_json["layer"];
            std::string activation_function = node_json["activation_function"];
            loaded_genome->nodes.push_back(std::make_shared<Node>(id, activation_function, layer));
        }

        // Deserialize genes
        for (const auto &gene_json : genome_json["genes"])
        {
            int innovation_nb = gene_json["innovation_nb"];
            int from_node_id = gene_json["from_node_id"];
            int to_node_id = gene_json["to_node_id"];
            bool enabled = gene_json["enabled"];
            double weight = gene_json["weight"];

            std::shared_ptr<Node> from_node = loaded_genome->get_node(from_node_id);
            std::shared_ptr<Node> to_node = loaded_genome->get_node(to_node_id);

            loaded_genome->genes.push_back(std::make_shared<ConnectionGene>(from_node, to_node, weight, innovation_nb, enabled));
        }

        loaded_genome->generate_network();
        return loaded_genome;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Failed to load genome: " << e.what() << std::endl;
        return nullptr;
    }
}
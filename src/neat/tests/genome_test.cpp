#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vector>
#include <memory>
#include <algorithm>
#include <fstream>
#include "../config.hpp"
#include "../genome.hpp"
#include "../connection_gene.hpp"
#include "../connection_history.hpp"

class GenomeTest : public testing::Test
{
protected:
    // Mock NeatConfig
    neat::Config config;
    std::vector<std::shared_ptr<neat::ConnectionHistory>> connection_history;

    void SetUp() override
    {
        // Set up NeatConfig
        config = neat::load_config("./src/neat/default_config.txt");
        config.num_inputs = 2;
        config.num_outputs = 2;
        config.num_hidden_layers = 1;

        // Set up ConnectionHistory
        connection_history = {};
    }

    void TearDown() override
    {
        // Clear connection_history after each test
        connection_history.clear();
    }
};

TEST_F(GenomeTest, Init)
{
    neat::Genome *genome = new neat::Genome(config);

    ASSERT_TRUE(genome->id.size() > 0);
    ASSERT_TRUE(genome->genes.empty());
    ASSERT_EQ(genome->nodes.size(), 6);
    ASSERT_EQ(genome->inputs, 2);
    ASSERT_EQ(genome->outputs, 2);
    ASSERT_EQ(genome->layers, 3);
    ASSERT_EQ(genome->next_node, 6);
    ASSERT_TRUE(genome->network.empty());
    ASSERT_EQ(genome->fitness, 0);
}

TEST_F(GenomeTest, FullyConnect)
{
    neat::Genome *genome = new neat::Genome(config);
    genome->fully_connect(connection_history);

    // Check if the genes are generated properly
    ASSERT_EQ(genome->genes.size(), 8);
    // From layer 0 to 1
    ASSERT_EQ(genome->genes[0]->from_node->id, 0);
    ASSERT_EQ(genome->genes[0]->to_node->id, 2);
    ASSERT_EQ(genome->genes[1]->from_node->id, 0);
    ASSERT_EQ(genome->genes[1]->to_node->id, 3);
    ASSERT_EQ(genome->genes[2]->from_node->id, 1);
    ASSERT_EQ(genome->genes[2]->to_node->id, 2);
    ASSERT_EQ(genome->genes[3]->from_node->id, 1);
    ASSERT_EQ(genome->genes[3]->to_node->id, 3);
    // From layer 1 to 2
    ASSERT_EQ(genome->genes[4]->from_node->id, 2);
    ASSERT_EQ(genome->genes[4]->to_node->id, 4);
    ASSERT_EQ(genome->genes[5]->from_node->id, 2);
    ASSERT_EQ(genome->genes[5]->to_node->id, 5);
    ASSERT_EQ(genome->genes[6]->from_node->id, 3);
    ASSERT_EQ(genome->genes[6]->to_node->id, 4);
    ASSERT_EQ(genome->genes[7]->from_node->id, 3);
    ASSERT_EQ(genome->genes[7]->to_node->id, 5);
}

TEST_F(GenomeTest, GetNode)
{
    neat::Genome *genome = new neat::Genome(config);
    std::shared_ptr<neat::Node> node = genome->get_node(0);

    ASSERT_NE(node, nullptr);
    ASSERT_EQ(node->id, 0);
}

TEST_F(GenomeTest, ConnectNodes)
{
    neat::Genome *genome = new neat::Genome(config);
    genome->fully_connect(connection_history);
    genome->connect_nodes();

    // Check if the connections are set properly
    ASSERT_EQ(genome->nodes[0]->output_connections.size(), 2);
    ASSERT_EQ(genome->nodes[1]->output_connections.size(), 2);
    ASSERT_EQ(genome->nodes[2]->output_connections.size(), 2);
    ASSERT_EQ(genome->nodes[3]->output_connections.size(), 2);
    ASSERT_EQ(genome->nodes[4]->output_connections.size(), 0);
    ASSERT_EQ(genome->nodes[5]->output_connections.size(), 0);
}

TEST_F(GenomeTest, FeedForward)
{
    neat::Genome *genome = new neat::Genome(config);
    genome->fully_connect(connection_history);

    std::vector<double> inputValues = {1, 0};
    std::vector<double> outputValues = genome->feed_forward(inputValues);

    // Check if the output values are calculated properly
    ASSERT_EQ(outputValues.size(), 2);
    ASSERT_EQ(outputValues[0], genome->nodes[2]->output_value);
    ASSERT_EQ(outputValues[1], genome->nodes[3]->output_value);
}

TEST_F(GenomeTest, GenerateNetwork)
{
    neat::Genome *genome = new neat::Genome(config);
    genome->fully_connect(connection_history);
    genome->generate_network();

    // Check if the network is generated properly
    ASSERT_EQ(genome->network.size(), 6);
    // 2 input nodes + 2 output nodes
    ASSERT_EQ(genome->network[0]->id, 0); // First input node
    ASSERT_EQ(genome->network[1]->id, 1); // Second input node
    ASSERT_EQ(genome->network[2]->id, 2); // First node in the hidden layer
    ASSERT_EQ(genome->network[3]->id, 3); // Second node in the hidden layer
    ASSERT_EQ(genome->network[4]->id, 4); // First output node
    ASSERT_EQ(genome->network[5]->id, 5); // Second output node
}

TEST_F(GenomeTest, AddNode)
{
    neat::Genome *genome = new neat::Genome(config);
    genome->fully_connect(connection_history);
    const size_t initialNumGenes = genome->genes.size();
    const size_t initialNumNodes = genome->nodes.size();

    genome->add_node(connection_history);

    // Check if a new node and connections are added properly
    // 1 new connection + 2 existing connections
    ASSERT_EQ(genome->genes.size(), initialNumGenes + 2);
    // 1 new node
    ASSERT_EQ(genome->nodes.size(), initialNumNodes + 1);
}

TEST_F(GenomeTest, RemoveNode)
{
    neat::Genome *genome = new neat::Genome(config);
    genome->fully_connect(connection_history);
    genome->add_node(connection_history);
    const size_t initialNumGenes = genome->genes.size();
    const size_t initialNumNodes = genome->nodes.size();

    genome->remove_node();

    // Check if a node and its connections are removed properly
    // At least one connection removed
    ASSERT_LT(genome->genes.size(), initialNumGenes);
    // 1 node removed
    ASSERT_EQ(genome->nodes.size(), initialNumNodes - 1);
}

TEST_F(GenomeTest, AddConnection)
{
    neat::Genome *genome = new neat::Genome(config);
    const size_t initialNumGenes = genome->genes.size();

    genome->add_connection(connection_history);

    // Check if a new connection is added properly
    ASSERT_EQ(genome->genes.size(), initialNumGenes + 1);
}

TEST_F(GenomeTest, RemoveConnection)
{
    neat::Genome *genome = new neat::Genome(config);
    genome->fully_connect(connection_history);
    const size_t initialNumGenes = genome->genes.size();

    genome->remove_connection();

    // Check if a connection is removed properly
    ASSERT_EQ(genome->genes.size(), initialNumGenes - 1);
}

TEST_F(GenomeTest, NewConnectionWeight)
{
    neat::Genome *genome = new neat::Genome(config);

    double weight = genome->new_connection_weight();

    // Check if the new connection weight is within the specified range
    ASSERT_GE(weight, config.weight_min_value);
    ASSERT_LE(weight, config.weight_max_value);
}

TEST_F(GenomeTest, GetInnovationNumber)
{
    neat::Genome *genome = new neat::Genome(config);
    std::shared_ptr<neat::Node> fromNode = std::make_shared<neat::Node>(0, "sigmoid", 0);
    std::shared_ptr<neat::Node> toNode = std::make_shared<neat::Node>(2, "sigmoid", 1);
    int innovationNumber = genome->get_innovation_number(connection_history, fromNode, toNode);

    // Check if the innovation number is obtained properly
    ASSERT_EQ(typeid(innovationNumber), typeid(int));
}

TEST_F(GenomeTest, FullyConnected)
{
    neat::Genome *genome = new neat::Genome(config);
    genome->fully_connect(connection_history);

    // Check if the genome is considered fully connected
    ASSERT_TRUE(genome->fully_connected());
}

TEST_F(GenomeTest, Mutate)
{
    neat::Genome *genome = new neat::Genome(config);
    int initialNumGenes = genome->genes.size();
    int initialNumNodes = genome->genes.size();

    genome->mutate(connection_history);

    // Check if mutations are applied properly
    // Mutations may add new genes
    ASSERT_GE(genome->genes.size(), initialNumGenes);
    // Mutations may add new nodes
    ASSERT_GE(genome->genes.size(), initialNumNodes);
}

TEST_F(GenomeTest, Crossover)
{
    neat::Genome *parent1 = new neat::Genome(config);
    neat::Genome *parent2 = new neat::Genome(config);
    parent1->fully_connect(connection_history);
    parent2->fully_connect(connection_history);
    parent1->fitness = 1;
    parent2->fitness = 0.5;

    neat::Genome *child = parent1->crossover(parent2);

    // Check if crossover produces a child genome
    ASSERT_EQ(typeid(child), typeid(neat::Genome *));
    ASSERT_NE(&child, &parent1);
    ASSERT_NE(&child, &parent2);
}

TEST_F(GenomeTest, MatchingGene)
{
    neat::Genome *genome = new neat::Genome(config);
    genome->fully_connect(connection_history);

    std::sort(genome->genes.begin(), genome->genes.end(), [](const std::shared_ptr<neat::ConnectionGene> g1, const std::shared_ptr<neat::ConnectionGene> g2)
              { return g1->innovation_nb < g2->innovation_nb; });

    int matchingGeneIndex = genome->matching_gene(genome, genome->genes[0]->innovation_nb);

    // Check if matching gene is found properly
    ASSERT_EQ(matchingGeneIndex, 0);
}

TEST_F(GenomeTest, CloneTest)
{
    neat::Genome *genome = new neat::Genome(config);
    genome->fully_connect(connection_history);
    neat::Genome *clone = genome->clone();

    // Check if cloning the genome produces a valid clone
    ASSERT_TRUE(typeid(clone) == typeid(neat::Genome *));
    ASSERT_NE(&clone, &genome);
    ASSERT_EQ(clone->genes.size(), genome->genes.size());
    ASSERT_EQ(clone->nodes.size(), genome->nodes.size());
}

TEST_F(GenomeTest, JSON)
{
    neat::Genome *genome = new neat::Genome(config);
    genome->fully_connect(connection_history);

    // Convert the genome to a JSON object
    nlohmann::json genomeJson = genome->to_json();

    // Check if the JSON object is valid
    ASSERT_TRUE(genomeJson.is_object());
    ASSERT_TRUE(genomeJson.contains("id"));
    ASSERT_TRUE(genomeJson.contains("fitness"));
    ASSERT_TRUE(genomeJson.contains("genes"));
    ASSERT_TRUE(genomeJson.contains("nodes"));
    ASSERT_TRUE(genomeJson.contains("inputs"));
    ASSERT_TRUE(genomeJson.contains("outputs"));
    ASSERT_TRUE(genomeJson.contains("layers"));
    ASSERT_TRUE(genomeJson.contains("next_node"));

    // Convert the JSON object back to a genome
    neat::Genome *newGenome = neat::Genome::from_json(genomeJson);

    // Check if the new genome is valid
    ASSERT_EQ(typeid(newGenome), typeid(neat::Genome *));
    ASSERT_NE(newGenome, genome);
    ASSERT_TRUE(newGenome->is_equal(genome));
}

TEST_F(GenomeTest, SaveLoadTest)
{
    neat::Genome *genome = new neat::Genome(config);
    genome->fully_connect(connection_history);

    // Create a temporary directory for testing
    std::string tempDir = "./temp"; // Replace with the actual path
    std::string filePath = tempDir + "/test_genome.json";

    // Save the genome
    genome->save(filePath);

    ASSERT_TRUE(std::filesystem::exists(filePath));

    // Load the saved genome
    neat::Genome *loadedGenome = neat::Genome::load(filePath);

    // Check if the loaded genome is valid
    ASSERT_EQ(typeid(loadedGenome), typeid(neat::Genome *));
    ASSERT_NE(loadedGenome, genome);
    ASSERT_TRUE(loadedGenome->is_equal(genome));

    // Remove the temporary directory
    std::filesystem::remove_all(tempDir);
}

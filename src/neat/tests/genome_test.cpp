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

class TestGenome : public testing::Test
{
protected:
    // Mock NeatConfig
    NeatConfig config;
    std::vector<std::shared_ptr<ConnectionHistory>> connection_history;

    void SetUp() override
    {
        // Set up NeatConfig
        config = load_config("./src/neat/default_config.txt");
        config.num_inputs = 2;
        config.num_outputs = 2;

        // Set up ConnectionHistory
        connection_history = {};
    }

    void TearDown() override
    {
        // Clear connection_history after each test
        connection_history.clear();
    }
};

TEST_F(TestGenome, Init)
{
    Genome *genome = new Genome(config);

    ASSERT_TRUE(genome->id.size() > 0);
    ASSERT_TRUE(genome->genes.empty());
    ASSERT_EQ(genome->nodes.size(), 4);
    ASSERT_EQ(genome->inputs, 2);
    ASSERT_EQ(genome->outputs, 2);
    ASSERT_EQ(genome->layers, 2);
    ASSERT_EQ(genome->next_node, 4);
    ASSERT_TRUE(genome->network.empty());
    ASSERT_EQ(genome->fitness, 0);
}

TEST_F(TestGenome, FullyConnect)
{
    Genome *genome = new Genome(config);
    genome->fully_connect(connection_history);

    // Check if the genes are generated properly
    ASSERT_EQ(genome->genes.size(), 4);
    // First input node
    ASSERT_EQ(genome->genes[0]->from_node->id, 0);
    ASSERT_EQ(genome->genes[0]->to_node->id, 2);
    ASSERT_EQ(genome->genes[1]->from_node->id, 0);
    ASSERT_EQ(genome->genes[1]->to_node->id, 3);
    // Second input node
    ASSERT_EQ(genome->genes[2]->from_node->id, 1);
    ASSERT_EQ(genome->genes[2]->to_node->id, 2);
    ASSERT_EQ(genome->genes[3]->from_node->id, 1);
    ASSERT_EQ(genome->genes[3]->to_node->id, 3);
}

TEST_F(TestGenome, GetNode)
{
    Genome *genome = new Genome(config);
    std::shared_ptr<Node> node = genome->get_node(0);

    ASSERT_NE(node, nullptr);
    ASSERT_EQ(node->id, 0);
}

TEST_F(TestGenome, ConnectNodes)
{
    Genome *genome = new Genome(config);
    genome->fully_connect(connection_history);
    genome->connect_nodes();

    // Check if the connections are set properly
    ASSERT_EQ(genome->nodes[0]->output_connections.size(), 2);
    ASSERT_EQ(genome->nodes[1]->output_connections.size(), 2);
    ASSERT_EQ(genome->nodes[2]->output_connections.size(), 0);
}

TEST_F(TestGenome, FeedForward)
{
    Genome *genome = new Genome(config);
    genome->fully_connect(connection_history);

    std::vector<double> inputValues = {1, 0};
    std::vector<double> outputValues = genome->feed_forward(inputValues);

    // Check if the output values are calculated properly
    ASSERT_EQ(outputValues.size(), 2);
    ASSERT_EQ(outputValues[0], genome->nodes[2]->output_value);
    ASSERT_EQ(outputValues[1], genome->nodes[3]->output_value);
}

TEST_F(TestGenome, GenerateNetwork)
{
    Genome *genome = new Genome(config);
    genome->fully_connect(connection_history);
    genome->generate_network();

    // Check if the network is generated properly
    ASSERT_EQ(genome->network.size(), 4);
    // 2 input nodes + 2 output nodes
    ASSERT_EQ(genome->network[0]->id, 0); // First input node
    ASSERT_EQ(genome->network[1]->id, 1); // Second input node
    ASSERT_EQ(genome->network[2]->id, 2); // First output node
    ASSERT_EQ(genome->network[3]->id, 3); // Second output node
}

TEST_F(TestGenome, AddNode)
{
    Genome *genome = new Genome(config);
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

TEST_F(TestGenome, RemoveNode)
{
    Genome *genome = new Genome(config);
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

TEST_F(TestGenome, AddConnection)
{
    Genome *genome = new Genome(config);
    const size_t initialNumGenes = genome->genes.size();

    genome->add_connection(connection_history);

    // Check if a new connection is added properly
    ASSERT_EQ(genome->genes.size(), initialNumGenes + 1);
}

TEST_F(TestGenome, RemoveConnection)
{
    Genome *genome = new Genome(config);
    genome->fully_connect(connection_history);
    const size_t initialNumGenes = genome->genes.size();

    genome->remove_connection();

    // Check if a connection is removed properly
    ASSERT_EQ(genome->genes.size(), initialNumGenes - 1);
}

TEST_F(TestGenome, NewConnectionWeight)
{
    Genome *genome = new Genome(config);

    double weight = genome->new_connection_weight();

    // Check if the new connection weight is within the specified range
    ASSERT_GE(weight, config.weight_min_value);
    ASSERT_LE(weight, config.weight_max_value);
}

TEST_F(TestGenome, GetInnovationNumber)
{
    Genome *genome = new Genome(config);
    std::shared_ptr<Node> fromNode = std::make_shared<Node>(0, "sigmoid", 0);
    std::shared_ptr<Node> toNode = std::make_shared<Node>(2, "sigmoid", 1);
    int innovationNumber = genome->get_innovation_number(connection_history, fromNode, toNode);

    // Check if the innovation number is obtained properly
    ASSERT_EQ(typeid(innovationNumber), typeid(int));
}

TEST_F(TestGenome, FullyConnected)
{
    Genome *genome = new Genome(config);
    genome->fully_connect(connection_history);

    // Check if the genome is considered fully connected
    ASSERT_TRUE(genome->fully_connected());
}

TEST_F(TestGenome, Mutate)
{
    Genome *genome = new Genome(config);
    int initialNumGenes = genome->genes.size();
    int initialNumNodes = genome->genes.size();

    genome->mutate(connection_history);

    // Check if mutations are applied properly
    // Mutations may add new genes
    ASSERT_GE(genome->genes.size(), initialNumGenes);
    // Mutations may add new nodes
    ASSERT_GE(genome->genes.size(), initialNumNodes);
}

TEST_F(TestGenome, Crossover)
{
    Genome *parent1 = new Genome(config);
    Genome *parent2 = new Genome(config);
    parent1->fully_connect(connection_history);
    parent2->fully_connect(connection_history);
    parent1->fitness = 1;
    parent2->fitness = 0.5;

    Genome *child = parent1->crossover(parent2);

    // Check if crossover produces a child genome
    ASSERT_EQ(typeid(child), typeid(Genome *));
    ASSERT_NE(&child, &parent1);
    ASSERT_NE(&child, &parent2);
}

TEST_F(TestGenome, MatchingGene)
{
    Genome *genome = new Genome(config);
    genome->fully_connect(connection_history);

    std::sort(genome->genes.begin(), genome->genes.end(), [](const std::shared_ptr<ConnectionGene> g1, const std::shared_ptr<ConnectionGene> g2)
              { return g1->innovation_nb < g2->innovation_nb; });

    int matchingGeneIndex = genome->matching_gene(genome, genome->genes[0]->innovation_nb);

    // Check if matching gene is found properly
    ASSERT_EQ(matchingGeneIndex, 0);
}

TEST_F(TestGenome, CloneTest)
{
    Genome *genome = new Genome(config);
    genome->fully_connect(connection_history);
    Genome *clone = genome->clone();

    // Check if cloning the genome produces a valid clone
    ASSERT_TRUE(typeid(clone) == typeid(Genome *));
    ASSERT_NE(&clone, &genome);
    ASSERT_EQ(clone->genes.size(), genome->genes.size());
    ASSERT_EQ(clone->nodes.size(), genome->nodes.size());
}

TEST_F(TestGenome, SaveLoadTest)
{
    Genome *genome = new Genome(config);
    genome->fully_connect(connection_history);

    // Create a temporary directory for testing
    std::string tempDir = "./temp"; // Replace with the actual path
    std::string filePath = tempDir + "/test_genome.pkl";

    // Save the genome
    genome->save(filePath);

    // Load the saved genome
    Genome *loadedGenome = Genome::load(filePath);

    // Check if the loaded genome is valid
    ASSERT_EQ(typeid(loadedGenome), typeid(Genome *));
    ASSERT_NE(loadedGenome, genome);
    ASSERT_TRUE(loadedGenome->is_equal(genome));

    // Remove the temporary directory
    std::filesystem::remove_all(tempDir);
}

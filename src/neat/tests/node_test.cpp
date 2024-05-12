#include <gtest/gtest.h>
#include "../config.hpp"
#include "../node.hpp"
#include "../connection_gene.hpp"

class TestNode : public ::testing::Test
{
protected:
    neat::Config config;
    std::shared_ptr<neat::Node> node;

    void SetUp() override
    {
        config = neat::load_config("./src/neat/default_config.txt");
        node = std::make_shared<neat::Node>(1, "relu", 1);
    }

    void test_initialization()
    {
        ASSERT_EQ(node->id, 1);
        ASSERT_EQ(node->input_sum, 0);
        ASSERT_EQ(node->output_value, 0);
        ASSERT_EQ(node->output_connections.size(), 0);
        ASSERT_EQ(node->layer, 1);
        ASSERT_EQ(node->activation_function, "relu");
    }

    void test_activation()
    {
        // Test activate method when layer is not 0
        node->input_sum = 0.5;
        node->activate();
        node->propagate_output();
        // Output value should be updated
        ASSERT_NE(node->output_value, 0);
        ASSERT_EQ(typeid(node->output_value), typeid(double));
    }

    void test_mutate()
    {
        // Test activation mutation
        config.activation_mutate_rate = 1.0;
        node->mutate(config);
        ASSERT_NE(node->activation_function, "relu");
    }

    void test_is_connected_to()
    {
        std::shared_ptr<neat::Node> node1 = std::make_shared<neat::Node>(2, "step", 2);
        std::shared_ptr<neat::Node> node2 = std::make_shared<neat::Node>(3, "sigmoid", 3);
        node->output_connections.push_back(std::make_shared<neat::ConnectionGene>(node, node1, 1.0, 1, true));
        node->output_connections.push_back(std::make_shared<neat::ConnectionGene>(node, node2, 1.0, 1, true));
        // Validate if the node is connected to the provided node
        ASSERT_TRUE(node->is_connected_to(node1));
        ASSERT_TRUE(node->is_connected_to(node2));

        std::shared_ptr<neat::Node> node3 = std::make_shared<neat::Node>(4, "tanh", 1);
        // Can't be connected if is on the same layer
        ASSERT_FALSE(node->is_connected_to(node3));

        std::shared_ptr<neat::Node> node4 = std::make_shared<neat::Node>(4, "relu", 0);
        node4->output_connections.push_back(std::make_shared<neat::ConnectionGene>(node4, node, 1.0, 1, true));
        // Connected because of the connection gene
        ASSERT_TRUE(node->is_connected_to(node4));

        std::shared_ptr<neat::Node> node5 = std::make_shared<neat::Node>(5, "softmax", 0);
        node5->output_connections = {};
        // Can't be connected if is on a previous layer and no connection gene
        ASSERT_FALSE(node->is_connected_to(node5));
    }

    void test_clone()
    {
        std::shared_ptr<neat::Node> cloned_node = node->clone();
        ASSERT_TRUE(node->is_equal(cloned_node));
    }
};

TEST_F(TestNode, Initialization)
{
    test_initialization();
}

TEST_F(TestNode, Activation)
{
    test_activation();
}

TEST_F(TestNode, Mutate)
{
    test_mutate();
}

TEST_F(TestNode, IsConnectedTo)
{
    test_is_connected_to();
}

TEST_F(TestNode, Clone)
{
    test_clone();
}

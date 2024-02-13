#include <gtest/gtest.h>
#include <vector>
#include "../config.hpp"
#include "../genome.hpp"
#include "../node.hpp"
#include "../connection_history.hpp"
#include "../activation_functions.hpp"

class ConnectionHistoryTest : public ::testing::Test
{
protected:
    NeatConfig config;
    Genome *genome;
    std::shared_ptr<Node> fromNode;
    std::shared_ptr<Node> toNode;
    std::shared_ptr<ConnectionHistory> connectionHistory;

    void SetUp() override
    {
        config = load_config("./src/neat/default_config.txt");
        genome = new Genome(config);
        fromNode = std::make_shared<Node>(1, "sigmoid", 1);
        toNode = std::make_shared<Node>(2, "sigmoid", 2);
        genome->genes.emplace_back(std::make_shared<ConnectionGene>(fromNode, toNode, 0.5, 1, true));
        connectionHistory = std::make_shared<ConnectionHistory>(fromNode, toNode, 1);
    }

    void testInit()
    {
        ASSERT_EQ(connectionHistory->from_node, fromNode);
        ASSERT_EQ(connectionHistory->to_node, toNode);
    }

    void testMatchesWithExistingConnection()
    {
        bool result = connectionHistory->matches(fromNode, toNode);
        ASSERT_TRUE(result);
    }

    void testMatchesWithNonExistingConnection()
    {
        std::shared_ptr<Node> node = std::make_shared<Node>(3, "relu", 2);
        bool result = connectionHistory->matches(fromNode, node);
        ASSERT_FALSE(result);
    }
};

TEST_F(ConnectionHistoryTest, Init)
{
    testInit();
}

TEST_F(ConnectionHistoryTest, MatchesWithExistingConnection)
{
    testMatchesWithExistingConnection();
}

TEST_F(ConnectionHistoryTest, MatchesWithNonExistingConnection)
{
    testMatchesWithNonExistingConnection();
}

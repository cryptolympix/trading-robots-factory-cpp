#include <gtest/gtest.h>
#include "../config.hpp"
#include "../node.hpp"
#include "../connection_gene.hpp"
#include "../activation_functions.hpp"

class ConnectionGeneTest : public ::testing::Test
{
protected:
    neat::Config config;
    std::shared_ptr<neat::Node> fromNode;
    std::shared_ptr<neat::Node> toNode;
    std::shared_ptr<neat::ConnectionGene> gene;

    void SetUp() override
    {
        config = neat::load_config("./src/neat/default_config.txt");
        fromNode = std::make_shared<neat::Node>(1, "sigmoid", 1);
        toNode = std::make_shared<neat::Node>(2, "sigmoid", 2);
        gene = std::make_shared<neat::ConnectionGene>(fromNode, toNode, 0.5, 1, true);
    }
};

TEST_F(ConnectionGeneTest, Init)
{
    ASSERT_EQ(gene->from_node, fromNode);
    ASSERT_EQ(gene->to_node, toNode);
    ASSERT_DOUBLE_EQ(gene->weight, 0.5);
    ASSERT_EQ(gene->innovation_nb, 1);
    ASSERT_TRUE(gene->enabled);
}

TEST_F(ConnectionGeneTest, MutateWeightReplace)
{
    config.weight_replace_rate = 1.0;
    gene->mutate(config);
    ASSERT_TRUE(gene->weight >= -1.0 && gene->weight <= 1.0);
}

TEST_F(ConnectionGeneTest, MutateWeightMutate)
{
    config.weight_replace_rate = 0.0;
    config.weight_mutate_rate = 1.0;
    gene->mutate(config);
    ASSERT_NE(gene->weight, 0.5);
}

TEST_F(ConnectionGeneTest, MutateEnabledMutate)
{
    config.enabled_mutate_rate = 1.0;
    gene->mutate(config);
    ASSERT_FALSE(gene->enabled);
}

TEST_F(ConnectionGeneTest, Clone)
{
    std::shared_ptr<neat::ConnectionGene> clonedConnectionGene = gene->clone(fromNode, toNode);
    ASSERT_TRUE(gene->is_equal(clonedConnectionGene));
}

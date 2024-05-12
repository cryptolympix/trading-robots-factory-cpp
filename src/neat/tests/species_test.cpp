#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../config.hpp"
#include "../genome.hpp"
#include "../species.hpp"

class SpeciesTest : public testing::Test
{
protected:
    neat::Config config;
    neat::Genome *genome;
    std::vector<std::shared_ptr<neat::ConnectionHistory>> connection_history;
    neat::Species *species;

    void SetUp() override
    {
        config = neat::load_config("default_config.txt");
        config.num_inputs = 10;
        config.num_outputs = 2;

        genome = new neat::Genome(config);
        connection_history = init_connection_history(10, 2);
        genome->fully_connect(connection_history);

        species = new neat::Species(genome);
    }

    void TearDown() override
    {
        delete species;
    }

    std::vector<std::shared_ptr<neat::ConnectionHistory>> init_connection_history(int num_inputs, int num_outputs)
    {
        std::vector<std::shared_ptr<neat::ConnectionHistory>> connections;
        int innovation_nb = 0;
        std::vector<int> innovation_nbs(num_inputs * num_outputs + 1);

        for (int i = 0; i < num_inputs; ++i)
        {
            std::shared_ptr<neat::Node> node_input = std::make_shared<neat::Node>(i, "relu", 0);
            for (int j = 0; j < num_outputs; ++j)
            {
                std::shared_ptr<neat::Node> node_output = std::make_shared<neat::Node>(j, "sigmoid", 1);
                connections.push_back(std::make_shared<neat::ConnectionHistory>(node_input, node_output, innovation_nb));
                ++innovation_nb;
            }
        }

        return connections;
    }
};

TEST_F(SpeciesTest, Init)
{
    ASSERT_EQ(species->genomes.size(), 1);
    ASSERT_EQ(species->champion, genome);
    ASSERT_EQ(species->best_fitness, 0.0);
    ASSERT_EQ(species->average_fitness, 0.0);
    ASSERT_EQ(species->stagnation, 0);
}

TEST_F(SpeciesTest, SameSpeciesTrue)
{
    neat::Genome *otherGenome = genome->clone();
    ASSERT_TRUE(species->same_species(otherGenome, config));
    delete otherGenome;
}

TEST_F(SpeciesTest, SameSpeciesFalse)
{
    neat::Config otherConfig = neat::load_config("default_config.txt");
    otherConfig.num_inputs = 5;
    otherConfig.num_outputs = 1;

    neat::Genome *otherGenome = new neat::Genome(otherConfig);
    std::vector<std::shared_ptr<neat::ConnectionHistory>> otherConnectionHistory = init_connection_history(5, 1);
    otherGenome->fully_connect(otherConnectionHistory);

    ASSERT_FALSE(species->same_species(otherGenome, config));

    delete otherGenome;
}

TEST_F(SpeciesTest, AddToSpecies)
{
    neat::Genome *newGenome = new neat::Genome(config);
    species->add_to_species(newGenome);

    ASSERT_EQ(species->genomes.size(), 2);
    ASSERT_EQ(species->genomes[1], newGenome);
}

TEST_F(SpeciesTest, ExcessDisjointGenes)
{
    // Test with two same genomes
    neat::Genome *otherGenomeSame = genome->clone();
    double resultSame = species->get_excess_disjoint_genes(genome, otherGenomeSame);
    ASSERT_DOUBLE_EQ(resultSame, 0.0);
    delete otherGenomeSame;

    // Test with two totally different genomes
    neat::Genome *otherGenomeDifferent = new neat::Genome(config);
    std::vector<std::shared_ptr<neat::ConnectionHistory>> otherConnectionHistoryDifferent = init_connection_history(10, 2);
    otherGenomeDifferent->fully_connect(otherConnectionHistoryDifferent);
    double resultDifferent = species->get_excess_disjoint_genes(genome, otherGenomeDifferent);
    ASSERT_GT(resultDifferent, 0.0);
    delete otherGenomeDifferent;

    // Test with two genomes a little different
    neat::Genome *otherGenomeLittleDifferent = genome->clone();
    otherGenomeLittleDifferent->remove_connection();
    double resultLittleDifferent = species->get_excess_disjoint_genes(genome, otherGenomeLittleDifferent);
    ASSERT_DOUBLE_EQ(resultLittleDifferent, 1.0);
    delete otherGenomeLittleDifferent;
}

TEST_F(SpeciesTest, AverageWeightDifference)
{
    // Test average weight diff of two same genomes
    neat::Genome *otherGenomeSame = genome->clone();
    double resultSame = species->average_weight_diff(genome, otherGenomeSame);
    ASSERT_DOUBLE_EQ(resultSame, 0.0);
    delete otherGenomeSame;

    // Test average weight diff of two totally different genomes
    neat::Genome *otherGenomeDifferent = new neat::Genome(config);
    std::vector<std::shared_ptr<neat::ConnectionHistory>> otherConnectionHistoryDifferent = init_connection_history(10, 2);
    otherGenomeDifferent->fully_connect(otherConnectionHistoryDifferent);
    double resultDifferent = species->average_weight_diff(genome, otherGenomeDifferent);
    ASSERT_DOUBLE_EQ(resultDifferent, 100.0);
    delete otherGenomeDifferent;

    // Test average weight diff without connection genes
    neat::Genome *otherGenomeNoConnections = new neat::Genome(config);
    double resultNoConnections = species->average_weight_diff(genome, otherGenomeNoConnections);
    ASSERT_DOUBLE_EQ(resultNoConnections, 0.0);
    delete otherGenomeNoConnections;

    // Test average weight diff with two genomes a little bit different
    neat::Genome *otherGenomeLittleDifferent = genome->clone();
    otherGenomeLittleDifferent->config.weight_mutate_rate = 1.0;
    std::vector<std::shared_ptr<neat::ConnectionHistory>>
        otherGenomeLittleDifferentHistory = init_connection_history(10, 2);
    otherGenomeLittleDifferent->mutate(otherGenomeLittleDifferentHistory);
    double resultLittleDifferent = species->average_weight_diff(genome, otherGenomeLittleDifferent);
    ASSERT_GT(resultLittleDifferent, 0.0);
    delete otherGenomeLittleDifferent;
}

TEST_F(SpeciesTest, SortGenomesStagnationIncrement)
{
    species->stagnation = 0;
    species->sort_genomes();
    ASSERT_EQ(species->stagnation, 1);
}

TEST_F(SpeciesTest, SortGenomesWithNewBestGenome)
{
    species->stagnation = 100;

    // Create a new best genome with higher fitness
    neat::Genome *newBestGenome = new neat::Genome(config);
    newBestGenome->fitness = 10;

    species->add_to_species(newBestGenome);
    species->sort_genomes();

    ASSERT_EQ(species->stagnation, 0);
}

TEST_F(SpeciesTest, SetAverageFitness)
{
    // Create some mock genomes with fitness values
    neat::Genome *genome1 = new neat::Genome(config);
    genome1->fitness = 5;

    neat::Genome *genome2 = new neat::Genome(config);
    genome2->fitness = 10;

    neat::Genome *genome3 = new neat::Genome(config);
    genome3->fitness = 15;

    // Add the genomes to the species
    species->genomes = {genome1, genome2, genome3};

    // Call the function to calculate average fitness
    species->set_average_fitness();

    // Assert that the average fitness is calculated correctly
    ASSERT_EQ(species->average_fitness, (5 + 10 + 15) / 3);
}

TEST_F(SpeciesTest, GiveMeBaby)
{
    // Create a mock genome
    neat::Genome *mockGenome = new neat::Genome(config);

    // Call the function to generate a baby genome
    neat::Genome *babyGenome = species->give_me_baby(connection_history);

    // Assert that the result is a valid genome
    ASSERT_TRUE(babyGenome != nullptr);
    ASSERT_TRUE(babyGenome->nodes.size() > 0); // Adjust based on your genome structure
    ASSERT_TRUE(babyGenome->genes.size() > 0); // Adjust based on your genome structure
}

TEST_F(SpeciesTest, SelectGenome)
{
    // Add some mock genomes to the species
    neat::Genome *mockGenome1 = new neat::Genome(config);
    neat::Genome *mockGenome2 = new neat::Genome(config);
    species->genomes = {mockGenome1, mockGenome2};

    // Call the function to select a genome
    neat::Genome *selectedGenome = species->select_genome();

    // Assert that the selected genome is one of the genomes in the species
    ASSERT_TRUE(selectedGenome != nullptr);
    ASSERT_TRUE(selectedGenome == mockGenome1 || selectedGenome == mockGenome2);
}

TEST_F(SpeciesTest, KillGenomes)
{
    // Add some mock genomes to the species
    const int numGenomes = 10;
    for (int i = 0; i < numGenomes; ++i)
        species->add_to_species(new neat::Genome(config));

    // Call the function to kill genomes
    species->kill_genomes(config);

    // Assert that the number of genomes after killing is equal to minSpeciesSize
    ASSERT_EQ(species->genomes.size(), config.min_species_size);
}

TEST_F(SpeciesTest, FitnessSharing)
{
    // Set up some mock genomes with fitness values
    std::vector<neat::Genome *> genomes;
    genomes.push_back(new neat::Genome(config));
    genomes.push_back(new neat::Genome(config));
    genomes.push_back(new neat::Genome(config));

    genomes[0]->fitness = 5;
    genomes[1]->fitness = 10;
    genomes[2]->fitness = 15;

    species->genomes = genomes;

    // Call the function to apply fitness sharing
    species->fitness_sharing();

    // Define the expected fitness values after fitness sharing
    const double expectedFitness1 = 5.0 / genomes.size();
    const double expectedFitness2 = 10.0 / genomes.size();
    const double expectedFitness3 = 15.0 / genomes.size();

    // Assert that the fitness values of the genomes are updated correctly
    ASSERT_DOUBLE_EQ(genomes[0]->fitness, expectedFitness1);
    ASSERT_DOUBLE_EQ(genomes[1]->fitness, expectedFitness2);
    ASSERT_DOUBLE_EQ(genomes[2]->fitness, expectedFitness3);
}

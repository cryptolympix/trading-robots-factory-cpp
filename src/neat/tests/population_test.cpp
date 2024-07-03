#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vector>
#include <random>
#include "../config.hpp"
#include "../population.hpp"

class PopulationTest : public testing::Test
{
protected:
    neat::Config config;

    void SetUp() override
    {
        // Mock NeatConfig
        config = neat::load_config("./src/neat/default_config.txt");
        config.num_inputs = 5;
        config.num_outputs = 2;
        config.population_size = 10;
        config.species_elitism = 2;
        config.max_stagnation = 5;
        config.bad_species_threshold = 0.5;
        config.no_fitness_termination = false;
        config.min_species_size = 2;
        config.fitness_threshold = 100;
    }
};

TEST_F(PopulationTest, PopulationInitialization)
{
    neat::Population *p = new neat::Population(config);
    ASSERT_EQ(p->genomes.size(), config.population_size);
    ASSERT_EQ(p->species.size(), 0);
    ASSERT_EQ(p->generation, 0);
}

TEST_F(PopulationTest, SetBestGenome)
{
    neat::Population *p = new neat::Population(config);

    // Assume best fitness is set to 10 for simplicity
    p->best_fitness = 10;

    // Mock species and genomes
    neat::Genome *genome = new neat::Genome(config);
    genome->fitness = 20;
    neat::Species *species = new neat::Species(genome);
    species->genomes = {genome};
    p->species = {species};

    // Set best genome
    p->set_best_genome();

    // Assert that the best genome is set
    ASSERT_EQ(p->best_genome, genome);
}

TEST_F(PopulationTest, Speciate)
{
    neat::Population *p = new neat::Population(config);

    // Mock genomes
    neat::Genome *genome1 = new neat::Genome(config);
    neat::Genome *genome2 = new neat::Genome(config);
    p->genomes = {genome1, genome2};

    // Mock species
    neat::Species *species1 = new neat::Species(genome1);
    neat::Species *species2 = new neat::Species(genome2);
    p->species = {species1, species2};

    // Run speciation
    p->speciate();

    // Assert that genomes are grouped into species
    ASSERT_GT(p->species.size(), 0);
}

TEST_F(PopulationTest, ReproduceSpecies)
{
    neat::Population *p = new neat::Population(config);

    // Mock species
    neat::Genome *genome = new neat::Genome(config);
    neat::Species *species = new neat::Species(genome);
    p->species = {species};

    // Run reproduction
    p->reproduce_species();

    // Assert that the p's genomes are updated
    ASSERT_EQ(p->generation, 1);
    ASSERT_EQ(p->best_genome, genome);
    ASSERT_EQ(p->genomes.size(), config.population_size);
}

TEST_F(PopulationTest, SortSpecies)
{
    neat::Population *p = new neat::Population(config);

    // Mock species
    for (int i = 0; i < 5; ++i)
    {
        neat::Genome *genome = new neat::Genome(config);
        genome->fitness = random() % 100;
        neat::Species *species = new neat::Species(genome);
        p->species.push_back(species);
    }

    // Run species sorting
    p->sort_species();

    // Assert species are sorted
    ASSERT_EQ(p->species.size(), 5);

    auto getBestFitness = [](const neat::Species *s)
    { return s->best_fitness; };
    ASSERT_TRUE(std::is_sorted(p->species.begin(), p->species.end(), [&getBestFitness](const neat::Species *s1, const neat::Species *s2)
                               { return getBestFitness(s1) > getBestFitness(s2); }));

    for (auto &s : p->species)
    {
        ASSERT_TRUE(std::is_sorted(s->genomes.begin(), s->genomes.end(), [](const neat::Genome *g1, const neat::Genome *g2)
                                   { return g1->fitness > g2->fitness; }));
    }
}

TEST_F(PopulationTest, KillStagnantSpecies)
{
    neat::Population *p = new neat::Population(config);

    // Mock genomes
    neat::Genome *genome1 = new neat::Genome(config);
    neat::Genome *genome2 = new neat::Genome(config);
    neat::Genome *genome3 = new neat::Genome(config);
    neat::Genome *genome4 = new neat::Genome(config);

    // Mock species
    neat::Species *speciesToKeep1 = new neat::Species(genome1);
    neat::Species *speciesToKeep2 = new neat::Species(genome2);
    neat::Species *speciesToRemove1 = new neat::Species(genome3);
    neat::Species *speciesToRemove2 = new neat::Species(genome4);

    // Set the stagnation
    speciesToKeep1->stagnation = 2;
    speciesToKeep2->stagnation = 4;
    speciesToRemove1->stagnation = 6;
    speciesToRemove2->stagnation = 8;

    p->genomes = {genome1, genome2, genome3, genome4};
    p->species = {speciesToKeep1, speciesToKeep2, speciesToRemove1, speciesToRemove2};

    // Run killing stagnant species
    p->kill_stagnant_species();

    // Assert stagnant species are removed
    const auto &remainingSpecies = p->species;
    ASSERT_EQ(remainingSpecies.size(), 2);
    ASSERT_EQ(remainingSpecies[0], speciesToKeep1);
    ASSERT_EQ(remainingSpecies[1], speciesToKeep2);

    // Assert the genomes of the stagnant species are removed
    ASSERT_TRUE(std::find(p->genomes.begin(), p->genomes.end(), genome1) != p->genomes.end());
    ASSERT_TRUE(std::find(p->genomes.begin(), p->genomes.end(), genome2) != p->genomes.end());
    ASSERT_TRUE(std::find(p->genomes.begin(), p->genomes.end(), genome3) == p->genomes.end());
    ASSERT_TRUE(std::find(p->genomes.begin(), p->genomes.end(), genome4) == p->genomes.end());

    // Cleanup
    delete speciesToKeep1;
    delete speciesToKeep2;
    delete speciesToRemove1;
    delete speciesToRemove2;
}

TEST_F(PopulationTest, KillBadSpecies)
{
    neat::Population *p = new neat::Population(config);

    // Mock genomes
    neat::Genome *genome1 = new neat::Genome(config);
    neat::Genome *genome2 = new neat::Genome(config);
    neat::Genome *genome3 = new neat::Genome(config);

    // Mock species
    neat::Species *goodSpecies = new neat::Species(genome1);
    neat::Species *badSpecies1 = new neat::Species(genome2);
    neat::Species *badSpecies2 = new neat::Species(genome3);

    // Mock average fitness
    double average_fitness1 = 100.0;
    double average_fitness2 = 3.0;
    double average_fitness3 = 1.0;

    // Set average fitness
    goodSpecies->average_fitness = average_fitness1;
    badSpecies1->average_fitness = average_fitness2;
    badSpecies2->average_fitness = average_fitness3;

    p->genomes = {genome1, genome2, genome3};
    p->species = {goodSpecies, badSpecies1, badSpecies2};

    // Run killing bad species
    p->kill_bad_species();

    // Assert bad species are removed
    const auto &remainingSpecies = p->species;
    ASSERT_EQ(remainingSpecies.size(), 1);
    ASSERT_EQ(remainingSpecies[0], goodSpecies);

    // Assert genomes are correctly removed
    ASSERT_TRUE(std::find(p->genomes.begin(), p->genomes.end(), genome1) != p->genomes.end());
    ASSERT_TRUE(std::find(p->genomes.begin(), p->genomes.end(), genome2) == p->genomes.end());
    ASSERT_TRUE(std::find(p->genomes.begin(), p->genomes.end(), genome3) == p->genomes.end());

    // Cleanup
    delete goodSpecies;
    delete badSpecies1;
    delete badSpecies2;
}

TEST_F(PopulationTest, UpdateSpecies)
{
    neat::Population *p = new neat::Population(config);

    // Mock species
    neat::Species *species = new neat::Species(new neat::Genome(config));
    for (int i = 0; i < 10; ++i)
    {
        species->add_to_species(new neat::Genome(config));
    }
    p->species.push_back(species);

    // Run updating species
    p->update_species();

    // Assert species is updated
    const auto &updatedSpecies = p->species;
    ASSERT_EQ(updatedSpecies.size(), 1);

    const auto &updatedGenomes = updatedSpecies[0]->genomes;
    ASSERT_EQ(updatedGenomes.size(), config.min_species_size);
}

TEST_F(PopulationTest, Clone)
{
    neat::Population *population = new neat::Population(config);
    neat::Population *clone = population->clone();

    // Asserts the clone is valid

    for (size_t i = 0; i < clone->genomes.size(); ++i)
    {
        ASSERT_TRUE(clone->genomes[i]->is_equal(population->genomes[i]));
    }

    for (size_t i = 0; i < clone->species.size(); ++i)
    {
        ASSERT_TRUE(clone->species[i]->is_equal(population->species[i]));
    }

    if (population->best_genome != nullptr)
    {
        ASSERT_TRUE(clone->best_genome->is_equal(population->best_genome));
    }

    ASSERT_EQ(clone->generation, population->generation);
    ASSERT_EQ(clone->average_fitness, population->average_fitness);
    ASSERT_EQ(clone->best_fitness, population->best_fitness);
}

TEST_F(PopulationTest, SaveAndLoad)
{
    // Create a new population
    neat::Population *population = new neat::Population(this->config);

    // Add genomes to the loaded population
    for (size_t i = 0; i < 10; ++i)
    {
        population->genomes.push_back(new neat::Genome(this->config));
    }

    // Add species to the loaded population
    population->speciate();

    // Set the best genome
    population->best_genome = new neat::Genome(this->config);

    // Create a temporary directory for testing
    std::string tempDir = "./temp"; // Replace with the actual path
    std::string filePath = tempDir + "/test_population.json";

    // Save the population
    population->save(filePath);

    ASSERT_TRUE(std::filesystem::exists(filePath));

    // Load the saved population
    neat::Population *loadedPopulation = neat::Population::load(filePath, this->config);

    // Check if the loaded population is valid
    ASSERT_EQ(typeid(loadedPopulation), typeid(neat::Population *));
    ASSERT_NE(loadedPopulation, population);
    ASSERT_EQ(loadedPopulation->average_fitness, population->average_fitness);
    ASSERT_EQ(loadedPopulation->best_fitness, population->best_fitness);
    ASSERT_EQ(loadedPopulation->generation, population->generation);
    ASSERT_EQ(loadedPopulation->genomes.size(), population->genomes.size());
    ASSERT_EQ(loadedPopulation->species.size(), population->species.size());

    // Check if the loaded best genome is valid
    if (population->best_genome != nullptr)
    {
        ASSERT_TRUE(loadedPopulation->best_genome->is_equal(population->best_genome));
    }

    // Check if the loaded genomes are valid
    for (size_t i = 0; i < loadedPopulation->genomes.size(); ++i)
    {
        ASSERT_TRUE(loadedPopulation->genomes[i]->is_equal(population->genomes[i]));
    }

    // Check if the loaded species are valid
    for (size_t i = 0; i < loadedPopulation->species.size(); ++i)
    {
        ASSERT_TRUE(loadedPopulation->species[i]->is_equal(population->species[i]));
    }

    // Remove the temporary directory
    std::filesystem::remove_all(tempDir);
}
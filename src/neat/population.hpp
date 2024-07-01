#ifndef NEAT_POPULATION_HPP
#define NEAT_POPULATION_HPP

#include <cmath>
#include <cstdlib>
#include <algorithm>
#include <functional>
#include "genome.hpp"
#include "species.hpp"
#include "connection_history.hpp"

namespace neat
{

    /**
     * @class Population
     * @brief Represents a population of genomes evolving through the NEAT (NeuroEvolution of Augmenting Topologies) algorithm.
     */
    class Population
    {
    public:
        Config config;                                                      // NEAT configuration settings.
        std::vector<Genome *> genomes;                                      // List of genomes in the population.
        Genome *best_genome;                                                // Best genome in the population.
        int generation;                                                     // Current generation number.
        double best_fitness;                                                // Fitness of the best genome.
        double average_fitness;                                             // Average fitness of the population.
        std::vector<std::shared_ptr<ConnectionHistory>> innovation_history; // List of connection history to track innovations.
        std::vector<Species *> species;                                     // List of species within the population.

        /**
         * @brief Constructor for the Population class.
         * @param config NEAT configuration settings.
         * @param init_genomes Flag to initialize genomes in the population.
         */
        Population(const Config &config, bool init_genomes = true);

        /**
         * @brief Destructor for the Population class to ensure proper cleanup.
         */
        ~Population();

        /**
         * @brief Sets the best genome based on the current species.
         */
        void set_best_genome();

        /**
         * @brief Runs the NEAT algorithm for a specified number of generations.
         * @param evaluate_genome Function for evaluating the fitness of a genome.
         * @param nb_generations Number of generations to run the algorithm.
         * @param callback_generation Callback function for each generation (optional).
         */
        void run(std::function<void(Genome *, int)> evaluate_genome, int nb_generations, std::function<void(Population *, int)> callback_generation = nullptr);

        /**
         * @brief Groups the genomes into species based on their compatibility.
         */
        void speciate();

        /**
         * @brief Reproduces genomes within each species to create the next generation.
         */
        void reproduce_species();

        /**
         * @brief Sorts genomes within each species and sorts species by best fitness.
         */
        void sort_species();

        /**
         * @brief Removes stagnant species that haven't improved in N generations.
         */
        void kill_stagnant_species();

        /**
         * @brief Removes species with average fitness below a threshold.
         */
        void kill_bad_species();

        /**
         * @brief Resets the population if all species go extinct.
         */
        void reset_on_extinction();

        /**
         * @brief Calculates the sum of average fitness across all species.
         * @return Sum of average fitness values.
         */
        double get_average_fitness_sum();

        /**
         * @brief Update the average fitness of the population genomes.
         */
        void set_average_fitness();

        /**
         * @brief Updates species by removing low-performing genomes, applying fitness sharing, and updating average fitness.
         */
        void update_species();

        /**
         * @brief Return a copy of this population.
         * @return A copy of this population.
         */
        Population *clone();

        /**
         * @brief Save the population to a file.
         * @param filename Name of the file to save the population.
         */
        void save(const std::string &filename);

        /**
         * @brief Load the population from a file.
         * @param filename Name of the file to load the population.
         * @param config NEAT configuration settings.
         */
        static neat::Population *load(const std::string &filename, const Config &config);
    };
}

#endif // NEAT_POPULATION_HPP

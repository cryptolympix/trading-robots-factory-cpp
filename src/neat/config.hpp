#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include "activation_functions.hpp"

// Type aliases for readability
using InitialConnection = std::string;
using DistributionType = std::string;

/**
 * @brief Configuration parameters for the NEAT (NeuroEvolution of Augmenting Topologies) algorithm.
 *
 * This struct encapsulates various parameters that control the behavior of the NEAT algorithm during evolution.
 * NEAT is a genetic algorithm designed for evolving artificial neural networks.
 */
struct NeatConfig
{
    // ======== NEAT =========== //
    // The number of individuals in each generation.
    int population_size;
    // When the fitness computed by fitness_criterion meets or exceeds this threshold, the evolution process will terminate
    double fitness_threshold;
    bool no_fitness_termination;
    // If this evaluates to True, when all species simultaneously become extinct due to stagnation, a new random population will be created.
    bool reset_on_extinction;

    // ======== GENOME =========== //
    ActivationFunction activation_default;
    double activation_mutate_rate;
    int num_inputs;
    int num_outputs;
    double compatibility_disjoint_coefficient;
    double compatibility_weight_coefficient;
    double conn_add_prob;
    double conn_delete_prob;
    bool enabled_default;
    double enabled_mutate_rate;
    InitialConnection initial_connections;
    double node_add_prob;
    double node_delete_prob;
    double weight_init_mean;
    double weight_init_stdev;
    DistributionType weight_init_type;
    double weight_max_value;
    double weight_min_value;
    double weight_mutate_rate;
    double weight_replace_rate;

    // ======== STAGNATION =========== //
    // Species that have not shown improvement in more than this number of generations will be considered stagnant and removed.
    int max_stagnation;
    // The number of species that will be protected from stagnation; mainly intended to prevent total extinctions caused by all species becoming stagnant before new species arise. For example, a species_elitism setting of 3 will prevent the 3 species with the highest species fitness from being removed for stagnation regardless of the amount of time they have not shown improvement.
    int species_elitism;

    // ======== REPRODUCTION =========== //
    // The number of most-fit individuals in each species that will be preserved as-is from one generation to the next.
    int elitism;
    // The fraction for each species allowed to reproduce each generation.
    double survival_threshold;
    // The minimum number of genomes per species after reproduction.
    int min_species_size;

    // ======== SPECIES =========== //
    // Individuals whose genomic distance is less than this threshold are considered to be in the same species.
    double compatibility_threshold;
    // If average fitness of the specie if below the average fitness fitness of all species
    double bad_species_threshold;
};

/**
 * @brief Loads NEAT configuration from a file.
 * @param filename Name of the file containing the configuration settings.
 * @return NeatConfig object.
 */
NeatConfig load_config(const std::string &filename);

#endif

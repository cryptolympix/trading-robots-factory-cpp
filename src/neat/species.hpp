#ifndef SPECIES_H
#define SPECIES_H

#include <vector>
#include "genome.hpp"
#include "config.hpp"
#include "connection_history.hpp"

/**
 * @brief Represents a species of genomes in the NEAT algorithm for evolving neural networks.
 */
class Species
{
public:
    std::vector<Genome *> genomes; //  List of genomes in the species.
    Genome *champion;              //  The best genome in the species.
    double best_fitness;           //  Best fitness in the species.
    double average_fitness;        //  Average fitness of the genomes in the species.
    int stagnation;                //  Number of generations the species has gone without an improvement.

    /**
     * @brief Initializes a Species instance.
     * @param genome The initial genome for the species.
     */
    Species(Genome *genome = NULL);

    /**
     * @brief Deconstructor for Species instance.
     */
    ~Species();

    /**
     * @brief Adds a genome to the species.
     * @param genome The genome to add to the species.
     */
    void add_to_species(Genome *genome);

    /**
     * @brief Checks if the genome belongs to this species.
     * @param genome The genome to check.
     * @param config NEAT configuration settings.
     * @return True if the genome is in this species, False otherwise.
     */
    bool same_species(Genome *genome, const NeatConfig &config);

    /**
     * @brief Returns the number of excess and disjoint genes between two genomes.
     * i.e., returns the number of genes which don't match.
     * @param genome1 The first genome.
     * @param genome2 The second genome.
     * @return The number of excess and disjoint genes.
     */
    int get_excess_disjoint_genes(Genome *genome1, Genome *genome2);

    /**
     * @brief Returns the average weight difference between matching genes in two genomes.
     * @param genome1 The first genome.
     * @param genome2 The second genome.
     * @return The average weight difference.
     */
    float average_weight_diff(Genome *genome1, Genome *genome2);

    /**
     * @brief Sorts genomes in the species by fitness.
     */
    void sort_genomes();

    /**
     * @brief Sets the average fitness of the species.
     */
    void set_average_fitness();

    /**
     * @brief Gets a baby genome from the genomes in this species.
     * @param innovation_history List of connection history to track innovations.
     * @return The baby genome.
     */
    Genome *give_me_baby(std::vector<std::shared_ptr<ConnectionHistory>> innovation_history);

    /**
     * @brief Selects a genome from the species based on its fitness.
     * @return The selected genome.
     */
    Genome *select_genome();

    /**
     * @brief Kills a part of the species based on a survival threshold.
     * @param config NEAT configuration settings.
     */
    void kill_genomes(const NeatConfig &config);

    /**
     * @brief Applies fitness sharing to protect unique genomes.
     */
    void fitness_sharing();

    /**
     * @brief Compare two species.
     * @param other The other species to compare with it
     * @returns True if the species are equals, otherwise false.
     */
    bool is_equal(Species *other);

    /**
     * @brief Return a copy of this species.
     * @return A copy of this species.
     */
    Species *clone();
};

#endif // SPECIES_H

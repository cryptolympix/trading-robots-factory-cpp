#ifndef CONNECTION_GENE_H
#define CONNECTION_GENE_H

#include "config.hpp"

class Node; // Forward declaration

/**
 * @brief Represents a connection gene in the NEAT (NeuroEvolution of Augmenting Topologies) algorithm.
 */
class ConnectionGene
{
public:
    std::shared_ptr<Node> from_node; // Pointer to the source node.
    std::shared_ptr<Node> to_node;   // Pointer to the target node.
    double weight;                   // Weight of the connection.
    int innovation_nb;               // Innovation number of the connection.
    bool enabled;                    // Flag indicating whether the connection is enabled or disabled.

    /**
     * @brief Constructor for ConnectionGene.
     * @param from Pointer to the source node.
     * @param to Pointer to the target node.
     * @param w Weight of the connection.
     * @param innovation Innovation number of the connection.
     * @param enabled Flag indicating whether the connection is enabled or disabled.
     */
    ConnectionGene(std::shared_ptr<Node> from, std::shared_ptr<Node> to, double w, int innovation, bool enabled);

    /**
     * @brief Mutates the connection gene based on the NEAT configuration.
     * @param config Reference to the NEAT configuration.
     */
    void mutate(const NeatConfig &config);

    /**
     * @brief Compare two connection genes.
     * @param other The genes to compare with it.
     * @return True if the genes are the same and connection genes, otherwise false.
     */
    bool is_equal(std::shared_ptr<ConnectionGene> other);

    /**
     * @brief Creates a clone of the connection gene with specified source and target nodes.
     * @param from Pointer to the new source node.
     * @param to Pointer to the new target node.
     * @return Pointer to the cloned ConnectionGene.
     */
    std::shared_ptr<ConnectionGene> clone(std::shared_ptr<Node> from, std::shared_ptr<Node> to);
};

#endif

#ifndef CONNECTION_HISTORY_H
#define CONNECTION_HISTORY_H

#include <vector>
#include "node.hpp"
#include "genome.hpp"

/**
 * @brief Represents the history of a connection innovation in the NEAT (NeuroEvolution of Augmenting Topologies) algorithm.
 */
class ConnectionHistory
{
public:
    std::shared_ptr<Node> from_node; // Pointer to the source node.
    std::shared_ptr<Node> to_node;   // Pointer to the target node.
    int innovation_nb;               // Innovation number of the connection.

    /**
     * @brief Constructor for ConnectionHistory.
     * @param from_node Pointer to the source node.
     * @param to_node Pointer to the target node.
     * @param innovation_nb Innovation number of the connection.
     */
    ConnectionHistory(std::shared_ptr<Node> from_node, std::shared_ptr<Node> to_node, int innovation_nb);

    /**
     * @brief Returns whether the genome matches the original genome and the connection is between the same nodes
     * @param from_node Pointer to the source node.
     * @param to_node Pointer to the target node.
     * @return True if the connection history matches, false otherwise.
     */
    bool matches(std::shared_ptr<Node> from_node, std::shared_ptr<Node> to_node);
};

#endif

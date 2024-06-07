#ifndef NEAT_NODE_HPP
#define NEAT_NODE_HPP

#include <vector>
#include "connection_gene.hpp"
#include "activation_functions.hpp"
#include "config.hpp"

namespace neat
{
    // Type alias for the function pointer
    using ActivationFunctionPointer = double (*)(double);

    class ConnectionGene; // Forward declaration

    /**
     * @brief Represents a node in a neural network.
     */
    class Node
    {
    public:
        int id;                                                          // Unique identifier for the node.
        double input_sum;                                                // Accumulated input sum for the node.
        double output_value;                                             // Output value after activation.
        std::vector<std::shared_ptr<ConnectionGene>> output_connections; // List of outgoing connections.
        int layer;                                                       // Layer to which the node belongs.
        neat::ActivationFunction activation_function;                    // Activation function of the node.

        /**
         * @brief Constructor for the Node class.
         * @param id Unique identifier for the node.
         * @param activation_function Activation function of the node.
         * @param layer Layer to which the node belongs.
         */
        Node(int id, ActivationFunction activation_function, int layer);

        /**
         * @brief Activate the node using its activation function.
         */
        void activate();

        /**
         * @brief Propagate the output to connected nodes.
         */
        void propagate_output();

        /**
         * @brief Mutates the node's properties based on the NEAT algorithm.
         * @param config NEAT configuration settings.
         */
        void mutate(const Config &config);

        /**
         * @brief Checks if the node is connected to another node.
         * @param node Pointer to the target node.
         * @return True if connected, false otherwise.
         */
        bool is_connected_to(std::shared_ptr<Node> node);

        /**
         * @brief Compare two nodes.
         * @param other The node to compare with it.
         * @return True if the nodes are the same and connection genes, otherwise false.
         */
        bool is_equal(std::shared_ptr<Node> other);

        /**
         * @brief Creates a clone of the node.
         * @return Pointer to the cloned node.
         */
        std::shared_ptr<Node> clone();

    private:
        /**
         * @brief Retrieves the activation function based on the specified enum.
         * @param activation_function Activation function specified by the enum.
         * @return Pointer to the activation function.
         */
        ActivationFunctionPointer get_function(ActivationFunction function);
    };

}

#endif

#include <vector>
#include <cmath>
#include <iostream>
#include <algorithm>
#include "config.hpp"
#include "activation_functions.hpp"
#include "math_utils.hpp"
#include "connection_gene.hpp"
#include "node.hpp"

Node::Node(int id, ActivationFunction activation_function, int layer) : id(id), input_sum(0), output_value(0), layer(layer), activation_function(activation_function) {}

void Node::activate()
{
    if (layer != 0)
    {
        ActivationFunctionPointer activation = get_function(activation_function);
        output_value = activation(input_sum);
    }
}

void Node::propagate_output()
{
    for (auto &c : output_connections)
    {
        if (c->enabled)
        {
            c->to_node->input_sum += c->weight * output_value;
        }
    }
}

void Node::mutate(const NeatConfig &config)
{
    if (randrange() < config.activation_mutate_rate)
    {
        std::vector<ActivationFunction> activations_functions = {
            "step",
            "sigmoid",
            "tanh",
            "relu",
            "leaky_relu",
            "prelu",
            "elu",
            "softmax",
            "linear",
            "swish"};

        int random_index = floor(randrange() * activations_functions.size());
        ActivationFunction random_function = activations_functions[random_index];

        while (random_function == activation_function)
        {
            random_index = floor(randrange() * activations_functions.size());
            random_function = activations_functions[random_index];
        }

        activation_function = random_function;
    }
}

bool Node::is_connected_to(std::shared_ptr<Node> node)
{
    if (node->layer == layer)
    {
        return false;
    }

    if (node->layer < layer)
    {
        for (auto &c : node->output_connections)
        {
            if (c->to_node.get() == this)
            {
                return true;
            }
        }
    }
    else
    {
        for (auto &c : output_connections)
        {
            if (c->to_node == node)
            {
                return true;
            }
        }
    }

    return false;
}

bool Node::is_equal(std::shared_ptr<Node> other)
{
    if (id != other->id || activation_function != other->activation_function || layer != other->layer)
    {
        return false;
    }

    for (auto &connection1 : output_connections)
    {
        bool found = false;

        for (auto &connection2 : other->output_connections)
        {
            if (connection1->is_equal(connection2))
            {
                found = true;
            }
        }

        if (found == false)
        {
            return false;
        }
    }

    return true;
}

std::shared_ptr<Node> Node::clone()
{
    return std::make_shared<Node>(id, activation_function, layer);
}

ActivationFunctionPointer Node::get_function(ActivationFunction function)
{
    if (activation_function == "step")
    {
        return step;
    }
    else if (activation_function == "sigmoid")
    {
        return sigmoid;
    }
    else if (activation_function == "tanh")
    {
        return tan_h;
    }
    else if (activation_function == "relu")
    {
        return relu;
    }
    else if (activation_function == "leaky_relu")
    {
        return leaky_relu;
    }
    else if (activation_function == "prelu")
    {
        return prelu;
    }
    else if (activation_function == "elu")
    {
        return elu;
    }
    else if (activation_function == "softmax")
    {
        return softmax;
    }
    else if (activation_function == "linear")
    {
        return linear;
    }
    else if (activation_function == "swish")
    {
        return swish;
    }
    else
    {
        std::cerr << "Unknown activation function: " << activation_function << std::endl;
        return sigmoid;
    }
}

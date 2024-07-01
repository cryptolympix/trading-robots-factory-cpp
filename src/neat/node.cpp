#include <vector>
#include <cmath>
#include <iostream>
#include <algorithm>
#include "config.hpp"
#include "activation_functions.hpp"
#include "math_utils.hpp"
#include "connection_gene.hpp"
#include "node.hpp"

neat::Node::Node(int id, ActivationFunction activation_function, int layer) : id(id), input_sum(0), output_value(0), layer(layer), activation_function(activation_function) {}

neat::Node::~Node()
{
    for (auto &c : this->output_connections)
    {
        c.reset();
    }

    this->output_connections.clear();
}

void neat::Node::activate()
{
    if (this->layer != 0)
    {
        ActivationFunctionPointer activation = get_function(this->activation_function);
        this->output_value = activation(input_sum);
    }
}

void neat::Node::propagate_output()
{
    for (auto &c : this->output_connections)
    {
        if (c->enabled)
        {
            c->to_node->input_sum += c->weight * this->output_value;
        }
    }
}

void neat::Node::mutate(const neat::Config &config)
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
        neat::ActivationFunction random_function = activations_functions[random_index];

        while (random_function == this->activation_function)
        {
            random_index = floor(randrange() * activations_functions.size());
            random_function = activations_functions[random_index];
        }

        this->activation_function = random_function;
    }
}

bool neat::Node::is_connected_to(std::shared_ptr<Node> node)
{
    if (node->layer == this->layer)
    {
        return false;
    }

    if (node->layer < this->layer)
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
        for (auto &c : this->output_connections)
        {
            if (c->to_node == node)
            {
                return true;
            }
        }
    }

    return false;
}

bool neat::Node::is_equal(std::shared_ptr<Node> other)
{
    if (id != other->id || this->activation_function != other->activation_function || this->layer != other->layer)
    {
        return false;
    }

    for (auto &connection1 : this->output_connections)
    {
        bool found = false;
        for (auto &connection2 : other->output_connections)
        {
            if (connection1->is_equal(connection2))
            {
                found = true;
                break;
            }
        }

        if (!found)
        {
            return false;
        }
    }

    return true;
}

std::shared_ptr<neat::Node> neat::Node::clone()
{
    return std::make_shared<neat::Node>(this->id, this->activation_function, this->layer);
}

neat::ActivationFunctionPointer neat::Node::get_function(neat::ActivationFunction function)
{
    if (this->activation_function == "step")
    {
        return step;
    }
    else if (this->activation_function == "sigmoid")
    {
        return sigmoid;
    }
    else if (this->activation_function == "tanh")
    {
        return tanh;
    }
    else if (this->activation_function == "relu")
    {
        return relu;
    }
    else if (this->activation_function == "leaky_relu")
    {
        return leaky_relu;
    }
    else if (this->activation_function == "prelu")
    {
        return prelu;
    }
    else if (this->activation_function == "elu")
    {
        return elu;
    }
    else if (this->activation_function == "softmax")
    {
        return softmax;
    }
    else if (this->activation_function == "linear")
    {
        return linear;
    }
    else if (this->activation_function == "swish")
    {
        return swish;
    }
    else
    {
        std::cerr << "Unknown activation function: " << this->activation_function << std::endl;
        return sigmoid;
    }
}

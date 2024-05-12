#ifndef NEAT_ACTIVATION_FUNCTIONS_HPP
#define NEAT_ACTIVATION_FUNCTIONS_HPP

#include <string>

namespace neat
{
    // Type alias for activation function
    using ActivationFunction = std::string;

    double step(double x);
    double sigmoid(double x);
    double tanh(double x);
    double relu(double x);
    double leaky_relu(double x);
    double prelu(double x);
    double elu(double x);
    double softmax(double x);
    double linear(double x);
    double swish(double x);
}

#endif

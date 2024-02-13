#include <cmath>
#include <algorithm>
#include <vector>
#include "activation_functions.hpp"

/**
 * @file activation_functions.h
 * @brief Defines various activation functions used in neural networks.
 */

/**
 * @brief Implements the step activation function.
 * @param x Input value.
 * @return 1.0 if x is greater than 0, else 0.0.
 */
double step(double x)
{
    return x > 0 ? 1.0 : 0.0;
}

/**
 * @brief Implements the sigmoid activation function.
 * @param x Input value.
 * @return The sigmoid of the input value.
 */
double sigmoid(double x)
{
    return 1.0 / (1.0 + exp(-x));
}

/**
 * @brief Implements the hyperbolic tangent (tanh) activation function.
 * @param x Input value.
 * @return The hyperbolic tangent of the input value.
 */
double tan_h(double x)
{
    return tanh(x);
}

/**
 * @brief Implements the rectified linear unit (ReLU) activation function.
 * @param x Input value.
 * @return The ReLU of the input value.
 */
double relu(double x)
{
    return std::max(0.0, x);
}

/**
 * @brief Implements the leaky ReLU activation function.
 * @param x Input value.
 * @return The leaky ReLU of the input value.
 */
double leaky_relu(double x)
{
    double alpha = 0.01;
    return x > 0 ? x : alpha * x;
}

/**
 * @brief Implements the parametric ReLU (PReLU) activation function.
 * @param x Input value.
 * @return The PReLU of the input value.
 */
double prelu(double x)
{
    double alpha = 0.1;
    return x > 0 ? x : alpha * x;
}

/**
 * @brief Implements the exponential linear unit (ELU) activation function.
 * @param x Input value.
 * @return The ELU of the input value.
 */
double elu(double x)
{
    double alpha = 1.0;
    return x > 0 ? x : alpha * (exp(x) - 1.0);
}

/**
 * @brief Compute the softmax activation function for a single value.
 *
 * The softmax function is commonly used in neural networks to convert a vector
 * of real numbers into a probability distribution. It exponentiates each input
 * value and normalizes the results to obtain probabilities.
 *
 * @param x The input value.
 * @return The softmax-activated value, representing a probability.
 */
double softmax(double x)
{
    // Softmax function for a single value
    return std::exp(x) / (1.0 + std::exp(x));
}

/**
 * @brief Implements the linear activation function.
 * @param x Input value.
 * @return The input value.
 */
double linear(double x)
{
    return x;
}

/**
 * @brief Implements the swish activation function.
 * @param x Input value.
 * @return The swish of the input value.
 */
double swish(double x)
{
    double beta = 1.0;
    return x / (1.0 + exp(-beta * x));
}

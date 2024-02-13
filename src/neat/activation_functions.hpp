#ifndef ACTIVATION_FUNCTIONS_H
#define ACTIVATION_FUNCTIONS_H

using ActivationFunction = std::string;

double step(double x);
double sigmoid(double x);
double tan_h(double x);
double relu(double x);
double leaky_relu(double x);
double prelu(double x);
double elu(double x);
double softmax(double x);
double linear(double x);
double swish(double x);

#endif

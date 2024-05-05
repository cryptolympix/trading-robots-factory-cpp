#ifndef NEAT_MATH_UTILS_H
#define NEAT_MATH_UTILS_H

/**
 * Generates a random double value in the range [min, max).
 *
 * @param min The minimum value of the range (inclusive).
 * @param max The maximum value of the range (exclusive).
 * @return A random double value in the specified range.
 */
double randrange(double min = 0.0, double max = 1.0);

/**
 * Generates a random double value in the uniform distribution.
 *
 * @param min The minimum value of the distribution.
 * @param max The maximum value of the distribution.
 * @return A random double value in the specified uniform distribution.
 */
double uniform(double min, double max);

/**
 * Generates a random double value from a normal (Gaussian) distribution.
 *
 * @param mean The mean (average) of the distribution.
 * @param stdev The standard deviation of the distribution.
 * @return A random double value from the normal distribution.
 */
double normal(double mean, double stdev);

#endif
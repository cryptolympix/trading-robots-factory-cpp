#include <cmath>
#include <iostream>
#include <random>

double randrange(double min = 0.0, double max = 1.0)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> distribution(min, max);
    return distribution(gen);
}

double uniform(double min, double max)
{
    return min + (max - min) * randrange();
}

double normal(double mean, double stdev)
{
    double u1 = randrange();
    double u2 = randrange();
    double z = sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2);
    return mean + stdev * z;
}
#include <vector>
#include <cmath>
#include "utils.hpp"

/**
 * @brief Calculate the simple moving average (SMA) of a given vector of values.
 *
 * @param values Vector of values.
 * @param period Period or number of values to consider for the SMA calculation.
 * @return std::vector<double> Vector containing the simple moving average values.
 */
std::vector<double> calculate_simple_moving_average(const std::vector<double> &values, int period)
{
    std::vector<double> sma(values.size(), 0.0); // Initialize sma vector with the same size as values

    // Calculate the initial SMA as the average of the first period values
    double sum = 0.0;
    for (int i = 0; i < period; ++i)
    {
        sum += values[i];
    }
    sma[period - 1] = sum / period;

    // Calculate SMA for subsequent periods
    for (size_t i = period; i < values.size(); ++i)
    {
        sum += values[i] - values[i - period]; // Update sum by removing the oldest value and adding the newest
        sma[i] = sum / period;                 // Calculate and store the SMA
    }

    return sma;
}

/**
 * @brief Calculate the exponential moving average (EMA) of a given vector of values.
 *
 * @param values Vector of values.
 * @param period Period or number of values to consider for the EMA calculation.
 * @return std::vector<double> Vector containing the exponential moving average values.
 */
std::vector<double> calculate_exponential_moving_average(const std::vector<double> &values, int period)
{
    std::vector<double> ema(values.size(), 0.0); // Initialize ema with zeros and same size as data
    double multiplier = 2.0 / (1 + period);
    double ema_prev = 0;

    // Calculate SMA for the first n days
    for (int i = 0; i < period; ++i)
    {
        ema_prev += values[i];
    }
    ema_prev /= period;

    // Set the first EMA value
    ema[period - 1] = ema_prev;

    // Calculate EMA for the rest of the data
    for (size_t i = period; i < values.size(); ++i)
    {
        double ema_today = values[i] * multiplier + ema_prev * (1 - multiplier);
        ema[i] = ema_today;
        ema_prev = ema_today;
    }

    return ema;
}

/**
 * @brief Calculate the standard deviation of a given vector of values.
 *
 * @param values Vector of values.
 * @param period Period to consider.
 * @return double Standard deviation.
 */
std::vector<double> calculate_standard_deviation(const std::vector<double> &values, int period)
{
    // Initialize std_deviation vector with the same size as values
    std::vector<double> std_deviation(values.size(), 0.0);

    for (size_t i = period - 1; i < values.size(); ++i)
    {
        // Calculate the mean
        double mean = 0.0;
        for (int j = i - period + 1; j <= i; ++j)
        {
            mean += values[j];
        }
        mean /= period;

        // Calculate the sum of squared differences
        double sum_squared_diff = 0.0;
        for (int j = i - period + 1; j <= i; ++j)
        {
            sum_squared_diff += std::pow(values[j] - mean, 2);
        }

        // Calculate the standard deviation
        double std_dev = std::sqrt(sum_squared_diff / period);
        std_deviation[i] = std_dev;
    }

    return std_deviation;
}
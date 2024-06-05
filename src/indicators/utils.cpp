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

    if (period > values.size())
    {
        return sma;
    }

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

    if (period > values.size())
    {
        return ema;
    }

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

/**
 * @brief Calculate the pivots position of a given vector of values.
 *
 * @param values Vector of values.
 * @param left_bars Number of bars to the left to consider.
 * @param right_bars Number of bars to the right to consider.
 * @return std::vector<double> Vector containing the pivot values represented as 1, otherwise 0.
 */
std::vector<double> calculate_pivots_high(std::vector<double> &values, int left_bars, int right_bars)
{
    std::vector<double> pivots(values.size(), 0.0);

    for (size_t i = 0; i < values.size() - right_bars; ++i)
    {
        std::vector<double>::iterator start = (i < left_bars) ? values.begin() : values.begin() + i - left_bars;
        std::vector<double>::iterator end = values.begin() + i + right_bars + 1;

        if (i - left_bars < 0)
        {
            start = values.begin();
        }
        if (i + right_bars + 1 > values.size())
        {
            end = values.end();
        }

        if (i < left_bars)
        {
            pivots[i] = std::max_element(start, end) == values.begin() + i ? 1 : 0;
        }
        else
        {
            pivots[i] = std::max_element(start, end) == values.begin() + i ? 1 : 0;
        }
    }

    return pivots;
}

/**
 * @brief Calculate the pivots position of a given vector of values.
 *
 * @param values Vector of values.
 * @param left_bars Number of bars to the left to consider.
 * @param right_bars Number of bars to the right to consider.
 * @return std::vector<double> Vector containing the pivot values represented as 1, otherwise 0.
 */
std::vector<double> calculate_pivots_low(std::vector<double> &values, int left_bars, int right_bars)
{
    std::vector<double> pivots(values.size(), 0.0);

    for (size_t i = 0; i < values.size() - right_bars; ++i)
    {
        std::vector<double>::iterator start = (i < left_bars) ? values.begin() : values.begin() + i - left_bars;
        std::vector<double>::iterator end = values.begin() + i + right_bars + 1;

        if (i - left_bars < 0)
        {
            start = values.begin();
        }
        if (i + right_bars + 1 > values.size())
        {
            end = values.end();
        }

        if (i < left_bars)
        {
            pivots[i] = (std::min_element(start, end) == values.begin() + i) ? 1 : 0;
        }
        else
        {
            pivots[i] = (std::min_element(start, end) == values.begin() + i) ? 1 : 0;
        }
    }

    return pivots;
}
#ifndef UTILS_INDICATORS_HPP
#define UTILS_INDICATORS_HPP

#include <vector>

/**
 * @brief Calculate the simple moving average (SMA) of a given vector of values.
 *
 * @param values Vector of values.
 * @param period Period or number of values to consider for the SMA calculation.
 * @return std::vector<double> Vector containing the simple moving average values.
 */
std::vector<double> calculate_simple_moving_average(const std::vector<double> &values, int period);

/**
 * @brief Calculate the exponential moving average (EMA) of a given vector of values.
 *
 * @param values Vector of values.
 * @param period Period or number of values to consider for the EMA calculation.
 * @return std::vector<double> Vector containing the exponential moving average values.
 */
std::vector<double> calculate_exponential_moving_average(const std::vector<double> &values, int periods);

/**
 * @brief Calculate the standard deviation of a given vector of values.
 *
 * @param values Vector of values.
 * @param period Period to consider.
 * @return double Standard deviation.
 */
std::vector<double> calculate_standard_deviation(const std::vector<double> &values, int period);

/**
 * @brief Calculate the pivots position of a given vector of values.
 *
 * @param values Vector of values.
 * @param left_bars Number of bars to the left to consider.
 * @param right_bars Number of bars to the right to consider.
 * @return std::vector<double> Vector containing the pivot values represented as 1, otherwise 0.
 */
std::vector<double> calculate_pivots_high(std::vector<double> &values, int left_bars, int right_bars);

/**
 * @brief Calculate the pivots position of a given vector of values.
 *
 * @param values Vector of values.
 * @param left_bars Number of bars to the left to consider.
 * @param right_bars Number of bars to the right to consider.
 * @return std::vector<double> Vector containing the pivot values represented as 1, otherwise 0.
 */
std::vector<double> calculate_pivots_low(std::vector<double> &values, int left_bars, int right_bars);

#endif // UTILS_INDICATORS_HPP
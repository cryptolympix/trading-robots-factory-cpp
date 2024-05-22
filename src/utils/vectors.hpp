#ifndef VECTORS_HPP
#define VECTORS_HPP

#include <vector>

/**
 * @brief Add two vectors element-wise.
 *
 * @param vec1 The first vector.
 * @param vec2 The second vector.
 * @return std::vector<double> The resulting vector after adding vec1 and vec2 element-wise.
 */
std::vector<double> add_vectors(const std::vector<double> &vec1, const std::vector<double> &vec2);

/**
 * @brief Subtract two vectors element-wise.
 *
 * @param vec1 The first vector.
 * @param vec2 The second vector.
 * @return std::vector<double> The resulting vector after subtracting vec2 from vec1.
 */
std::vector<double> subtract_vectors(const std::vector<double> &vec1, const std::vector<double> &vec2);

/**
 * @brief Divide two vectors element-wise.
 *
 * @param vec1 The first vector.
 * @param vec2 The second vector.
 * @return std::vector<double> The resulting vector after dividing vec1 by vec2.
 */
std::vector<double> divide_vectors(const std::vector<double> &vec1, const std::vector<double> &vec2);

/**
 * @brief Normalize a numeric vector to a specified range.
 *
 * @param array The input numeric vector to be normalized.
 * @param current_range The current range of values in the input vector.
 * @param new_range The target range for normalization.
 * @return std::vector<double> The normalized vector.
 */
std::vector<double> normalize_vectors(const std::vector<double> &vector, std::pair<double, double> current_range = std::make_pair(0.0, 0.0), std::pair<double, double> new_range = std::make_pair(0.0, 1.0));

/**
 * @brief Function to calculate the sum of a subvector of size 'period'
 *
 * @param values The input vector of values.
 * @param period The period for which to calculate the sum.
 * @return std::vector<double> The vector of sums of subvectors of size 'period'.
 */
std::vector<double> calculate_sum_subvector(const std::vector<double> &values, int period);

#endif /* VECTORS_HPP */

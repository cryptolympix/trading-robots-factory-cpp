#include <vector>
#include <stdexcept>
#include <algorithm>
#include "vectors.hpp"

/**
 * @brief Add two vectors element-wise.
 *
 * @param vec1 The first vector.
 * @param vec2 The second vector.
 * @return std::vector<double> The resulting vector after adding vec1 and vec2 element-wise.
 */
std::vector<double> add_vectors(const std::vector<double> &vec1, const std::vector<double> &vec2)
{
    // Ensure both vectors have the same size
    if (vec1.size() != vec2.size())
    {
        throw std::invalid_argument("Vectors must have the same size for element-wise addition.");
    }

    std::vector<double> result;
    result.reserve(vec1.size());

    // Add corresponding elements of vec1 and vec2
    for (size_t i = 0; i < vec1.size(); ++i)
    {
        result.push_back(vec1[i] + vec2[i]);
    }

    return result;
}

/**
 * @brief Subtract two vectors element-wise.
 *
 * @param vec1 The first vector.
 * @param vec2 The second vector.
 * @return std::vector<double> The resulting vector after subtracting vec2 from vec1.
 */
std::vector<double> subtract_vectors(const std::vector<double> &vec1, const std::vector<double> &vec2)
{
    // Ensure both vectors have the same size
    if (vec1.size() != vec2.size())
    {
        throw std::invalid_argument("Vectors must have the same size for element-wise subtraction.");
    }

    std::vector<double> result;
    result.reserve(vec1.size());

    // Subtract corresponding elements of vec2 from vec1
    for (size_t i = 0; i < vec1.size(); ++i)
    {
        result.push_back(vec1[i] - vec2[i]);
    }

    return result;
}

/**
 * @brief Divide two vectors element-wise.
 *
 * @param vec1 The first vector.
 * @param vec2 The second vector.
 * @return std::vector<double> The resulting vector after dividing vec1 by vec2.
 */
std::vector<double> divide_vectors(const std::vector<double> &vec1, const std::vector<double> &vec2)
{
    // Ensure both vectors have the same size
    if (vec1.size() != vec2.size())
    {
        throw std::invalid_argument("Vectors must have the same size for element-wise subtraction.");
    }

    std::vector<double> result;
    result.reserve(vec1.size());

    // Subtract corresponding elements of vec2 from vec1
    for (size_t i = 0; i < vec1.size(); ++i)
    {
        if (vec2[i] != 0)
        {
            result.push_back(vec1[i] / vec2[i]);
        }
    }

    return result;
}

/**
 * @brief Normalize a numeric vector to a specified range.
 *
 * @param vector The input numeric vector to be normalized.
 * @param current_range The current range of values in the input vector.
 * @param new_range The target range for normalization.
 * @return std::vector<double> The normalized vector.
 */
std::vector<double> normalize_vector(const std::vector<double> &vector, std::pair<double, double> current_range, std::pair<double, double> new_range)
{
    double current_range_min = current_range.first;
    double current_range_max = current_range.second;
    double new_range_min = new_range.first;
    double new_range_max = new_range.second;

    if (current_range.first == 0.0 && current_range.second == 0.0)
    {
        auto current_range_minmax = std::minmax_element(vector.begin(), vector.end());
        current_range_min = *current_range_minmax.first;
        current_range_max = *current_range_minmax.second;
    }

    if (current_range_min == current_range_max)
    {
        throw std::invalid_argument("The current range of values in the input vector is zero.");
    }

    std::vector<double> normalized_vector(vector.size(), 0.0);
    for (size_t i = 0; i < vector.size(); ++i)
    {
        normalized_vector[i] = new_range_min + (vector[i] - current_range_min) * (new_range_max - new_range_min) / (current_range_max - current_range_min);
    }
    return normalized_vector;
}

/**
 * @brief Function to calculate the sum of a subvector of size 'period'
 *
 * @param values The input vector of values.
 * @param period The period for which to calculate the sum.
 * @return std::vector<double> The vector of sums of subvectors of size 'period'.
 */
std::vector<double> calculate_sum_subvector(const std::vector<double> &values, int period)
{
    std::vector<double> sum(period, 0);
    double current_sum = 0;

    for (int i = 0; i < period; ++i)
    {
        current_sum += values[i];
    }
    sum[period - 1] = current_sum;

    for (size_t i = period; i < values.size(); ++i)
    {
        current_sum += values[i] - values[i - period];
        sum.push_back(current_sum);
    }

    return sum;
}
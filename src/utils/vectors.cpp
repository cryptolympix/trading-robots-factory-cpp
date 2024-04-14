#include <vector>
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
 * @param array The input numeric vector to be normalized.
 * @param current_range The current range of values in the input vector.
 * @param new_range The target range for normalization.
 * @return std::vector<double> The normalized vector.
 */
std::vector<double> normalize_vectors(const std::vector<double> &vector, std::pair<double, double> current_range, std::pair<double, double> new_range)
{
    if (current_range.first == 0.0f && current_range.second == 0.0f)
    {
        auto minmax = std::minmax_element(vector.begin(), vector.end());
        current_range = std::make_pair(*minmax.first, *minmax.second);
    }

    std::vector<double> normalized_vector(vector.size(), 0.0);
    for (size_t i = 0; i < vector.size(); ++i)
    {
        if (current_range.first == current_range.second)
        {
            normalized_vector[i] = new_range.first;
        }
        else
        {
            normalized_vector[i] = new_range.first + (vector[i] - current_range.first) * (new_range.second - new_range.first) / (current_range.second - current_range.first);
        }
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
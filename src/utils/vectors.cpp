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

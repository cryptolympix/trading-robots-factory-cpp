#include <cmath>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>
#include "math.hpp"

/**
 * @brief Round up a floating-point number to the specified precision.
 *
 * @param x The input number.
 * @param precision The number of decimal places to round to.
 * @return float The rounded-up number.
 */
float decimal_ceil(float x, int precision)
{
    int n = pow(10, precision);
    return ceil(x * n) / n;
}

/**
 * @brief Round down a floating-point number to the specified precision.
 *
 * @param x The input number.
 * @param precision The number of decimal places to round to.
 * @return float The rounded-down number.
 */
float decimal_floor(float x, int precision)
{
    int n = pow(10, precision);
    return floor(x * n) / n;
}

/**
 * @brief Round a floating-point number to the specified precision.
 *
 * @param x The input number.
 * @param precision The number of decimal places to round to.
 * @return float The rounded number.
 */
float decimal_round(float x, int precision)
{
    int n = pow(10, precision);
    return round(x * n) / n;
}

/**
 * @brief Count the number of decimal places in a floating-point number.
 *
 * @param number The input number.
 * @return int The number of decimal places.
 */
int count_decimals(float number)
{
    // Convert the float number to a string
    std::stringstream ss;
    ss << number;
    std::string str_number = ss.str();

    // Find the position of the decimal point
    size_t decimal_pos = str_number.find('.');
    if (decimal_pos == std::string::npos)
    {
        // If no decimal point is found, return 0
        return 0;
    }

    // Calculate the number of characters after the decimal point
    return str_number.size() - decimal_pos - 1;
}

/**
 * @brief Normalize a numeric vector to a specified range.
 *
 * @param array The input numeric vector to be normalized.
 * @param current_range The current range of values in the input vector.
 * @param new_range The target range for normalization.
 * @return std::vector<float> The normalized vector.
 */
std::vector<float> normalize(const std::vector<float> &array, std::pair<float, float> current_range, std::pair<float, float> new_range)
{
    if (current_range.first == 0.0f && current_range.second == 0.0f)
    {
        auto minmax = std::minmax_element(array.begin(), array.end());
        current_range = std::make_pair(*minmax.first, *minmax.second);
    }

    std::vector<float> normalized_array(array.size());
    for (size_t i = 0; i < array.size(); ++i)
    {
        normalized_array[i] = new_range.first + (array[i] - current_range.first) * (new_range.second - new_range.first) / (current_range.second - current_range.first);
    }
    return normalized_array;
}

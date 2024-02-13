#ifndef MATH_UTILS_H
#define MATH_UTILS_H

#include <cmath>
#include <vector>
#include <algorithm>

/**
 * @brief Round up a floating-point number to the specified precision.
 *
 * @param x The input number.
 * @param precision The number of decimal places to round to.
 * @return float The rounded-up number.
 */
float decimal_ceil(float x, int precision);

/**
 * @brief Round down a floating-point number to the specified precision.
 *
 * @param x The input number.
 * @param precision The number of decimal places to round to.
 * @return float The rounded-down number.
 */
float decimal_floor(float x, int precision);

/**
 * @brief Round a floating-point number to the specified precision.
 *
 * @param x The input number.
 * @param precision The number of decimal places to round to.
 * @return float The rounded number.
 */
float decimal_round(float x, int precision);

/**
 * @brief Count the number of decimal places in a floating-point number.
 *
 * @param number The input number.
 * @return int The number of decimal places.
 */
int count_decimals(float number);

/**
 * @brief Normalize a numeric vector to a specified range.
 *
 * @param array The input numeric vector to be normalized.
 * @param current_range The current range of values in the input vector.
 * @param new_range The target range for normalization.
 * @return std::vector<float> The normalized vector.
 */
std::vector<float> normalize(const std::vector<float> &array, std::pair<float, float> current_range = std::make_pair(0.0f, 0.0f), std::pair<float, float> new_range = std::make_pair(0.0f, 1.0f));

#endif /* MATH_UTILS_H */

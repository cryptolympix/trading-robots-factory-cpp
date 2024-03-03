#ifndef MATH_UTILS_H
#define MATH_UTILS_H

#include <cmath>
#include <vector>
#include <algorithm>

/**
 * @brief Round up a doubleing-point number to the specified precision.
 *
 * @param x The input number.
 * @param precision The number of decimal places to round to.
 * @return double The rounded-up number.
 */
double decimal_ceil(double x, int precision);

/**
 * @brief Round down a doubleing-point number to the specified precision.
 *
 * @param x The input number.
 * @param precision The number of decimal places to round to.
 * @return double The rounded-down number.
 */
double decimal_floor(double x, int precision);

/**
 * @brief Round a doubleing-point number to the specified precision.
 *
 * @param x The input number.
 * @param precision The number of decimal places to round to.
 * @return double The rounded number.
 */
double decimal_round(double x, int precision);

/**
 * @brief Count the number of decimal places in a doubleing-point number.
 *
 * @param number The input number.
 * @return int The number of decimal places.
 */
int count_decimals(double number);

#endif // MATH_UTILS_H
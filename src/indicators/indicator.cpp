#include <iostream>
#include <regex>
#include <cmath>
#include <stdexcept>
#include <utility>
#include "../utils/vectors.hpp"
#include "../types.hpp"
#include "indicator.hpp"

#include "candle_patterns.hpp"
#include "momentum.hpp"

// Template function to convert string to the desired type
template <typename T>
T convert(const std::string &str);

// Specializations for int, double, and string
template <>
int convert<int>(const std::string &str)
{
    return std::stoi(str);
}

template <>
double convert<double>(const std::string &str)
{
    return std::stod(str);
}

template <>
std::string convert<std::string>(const std::string &str)
{
    return str;
}

/**
 * @brief Check if the ID is valid.
 * @param id The ID to check.
 * @param id_pattern The regex pattern to match the ID.
 * @return True if the ID is valid, false otherwise.
 */
bool is_valid_id(const std::string &id, const std::string &id_pattern)
{
    std::regex pattern(id_pattern);
    return std::regex_match(id, pattern);
}

// *********************************************************************************************************************

/**
 * @brief Construct a new Indicator::Indicator object.
 *
 * @param label The label of the indicator.
 * @param id The id of the indicator.
 * @param id_pattern The id pattern of the indicator.
 * @param offset The offset of the indicator.
 * @param values_range The range of values.
 * @return Indicator The indicator.
 */
Indicator::Indicator(const std::string &label, const std::string &id, std::string id_pattern, int offset, std::pair<double, double> values_range)
    : label(label), id(id), id_pattern(id_pattern), offset(offset), values_range(values_range)
{
    if (!is_valid_id(id, id_pattern))
    {
        throw std::invalid_argument("Invalid ID format for the indicator " + label + " with pattern " + id_pattern + ": " + id);
    }
}

/**
 * @brief Calculate the indicator.
 *
 * @param candles The candles data.
 * @param calculator The calculator function.
 * @param normalize_data Normalize the data.
 * @return std::vector<double> The calculated indicator values.
 */
std::vector<double> Indicator::calculate(const std::vector<Candle> &candles, std::function<std::vector<double>(std::vector<Candle>)> calculator, bool normalize_data) const
{
    std::vector<double> values(candles.size(), 0.0);

    if (this->offset < 0)
    {
        std::cerr << "Offset cannot be negative." << std::endl;
        return values;
    }
    else if (candles.empty())
    {
        std::cerr << "Candles data is empty." << std::endl;
        return values;
    }
    else if (candles.size() < this->offset)
    {
        std::cerr << "Offset is greater than the number of candles." << std::endl;
        return values;
    }
    else if (candles.size() == this->offset)
    {
        std::cerr << "Offset is equal to the number of candles." << std::endl;
        return values;
    }

    // Adjust candles based on the offset
    if (this->offset > 0)
    {
        std::vector<Candle> adjusted_candles(candles.begin(), candles.end() - this->offset);
        // Calculate the indicator
        values = calculator(adjusted_candles);

        // Concatenate the zero values and adjusted_candles
        std::vector<double> zero_values(offset, 0);
        values.insert(values.begin(), zero_values.begin(), zero_values.end());
    }
    else
    {
        // Calculate the indicator
        values = calculator(candles);
    }

    // Normalize the data
    if (normalize_data)
    {
        values = normalize_vector(values, this->values_range);
    }

    // Check that there isn't nan values
    if (std::any_of(values.begin(), values.end(), [](double value)
                    { return std::isnan(value); }))
    {
        std::cerr << "There are nan values in the indicator: " << this->id << std::endl;
        std::exit(1);
    }

    return values;
}

/**
 * @brief Calculate the indicator.
 *
 * @param candles The candles data.
 * @param normalize_data Normalize the data.
 */
std::vector<double> Indicator::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [](std::vector<Candle> candles)
        { return std::vector<double>(candles.size(), 0.0); },
        normalize_data);
}

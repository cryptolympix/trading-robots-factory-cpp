#include <iostream>
#include <regex>
#include <cmath>
#include <stdexcept>
#include <utility>
#include "../utils/vectors.hpp"
#include "indicator.hpp"
#include "../types.hpp"

/**
 * @brief Construct a new Indicator::Indicator object.
 */
Indicator::Indicator(const std::string &label, const std::string &id, int offset, std::pair<double, double> values_range)
    : label(label), id(id), offset(offset), values_range(values_range)
{
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
#include <iostream>
#include <regex>
#include <cmath>
#include <stdexcept>
#include <utility>
#include <algorithm>
#include <functional>
#include <unordered_map>
#include "../utils/vectors.hpp"
#include "../types.hpp"
#include "indicator.hpp"

#include "candle_patterns.hpp"
#include "momentum.hpp"

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
 * @param params The parameters of the indicator.
 * @param values_range The range of values.
 */
Indicator::Indicator(std::string label, std::string id, std::unordered_map<std::string, IndicatorParam> params, std::pair<double, double> values_range)
{
    this->label = label;
    this->id = id;
    this->params = params;
    this->values_range = values_range;

    // Check if the offset parameter is present, otherwise set it to 0
    if (this->params.find("offset") == this->params.end())
    {
        this->params["offset"] = 0;
    }

    this->id_params = this->id;
    this->id_params_pattern = this->id;
    for (const auto &[param_name, param_value] : this->params)
    {
        // Detect if the parameter is a string
        if (std::holds_alternative<std::string>(param_value))
        {
            this->id_params += "-" + std::get<std::string>(param_value);
            this->id_params_pattern += R"(-(\w+))";
        }

        // Detect if the parameter is an int
        else if (std::holds_alternative<int>(param_value))
        {
            this->id_params += "-" + std::to_string(std::get<int>(param_value));
            this->id_params_pattern += R"(-(\d+))";
        }

        // Detect if the parameter is a double
        else if (std::holds_alternative<double>(param_value))
        {
            this->id_params += "-" + std::to_string(std::get<double>(param_value));
            this->id_params_pattern += R"(-(\d+\.\d+))";
        }
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
    int offset = std::get<int>(this->params.at("offset"));

    if (offset < 0)
    {
        std::cerr << "Offset cannot be negative." << std::endl;
        return values;
    }
    else if (candles.empty())
    {
        std::cerr << "Candles data is empty." << std::endl;
        return values;
    }
    else if (candles.size() < offset)
    {
        std::cerr << "Offset is greater than the number of candles." << std::endl;
        return values;
    }
    else if (candles.size() == offset)
    {
        std::cerr << "Offset is equal to the number of candles." << std::endl;
        return values;
    }

    // Adjust candles based on the offset
    if (offset > 0)
    {
        std::vector<Candle> adjusted_candles(candles.begin(), candles.end() - offset);
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

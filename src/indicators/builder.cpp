#include <string>
#include <vector>
#include <functional>
#include <unordered_map>
#include <sstream>
#include <iostream>
#include <regex>
#include <stdexcept>
#include "builder.hpp"
#include "indicator.hpp"

// Include the indicators
#include "candle_patterns.hpp"
#include "candle_signals.hpp"
#include "candle.hpp"
#include "momentum_signals.hpp"
#include "momentum.hpp"
#include "time.hpp"
#include "trend_signals.hpp"
#include "trend.hpp"
#include "volatility_signals.hpp"
#include "volatility.hpp"
#include "volume_signals.hpp"
#include "volume.hpp"

/**
 * @brief Get the indicators map.
 * @return std::map<std::string, std::function<Indicator *(std::vector<IndicatorParam>)>> The indicators map.
 */
std::unordered_map<std::string, std::function<Indicator *(std::vector<IndicatorParam>)>> get_indicators_map()
{
    // Map linking the indicator ID to the constructor function
    std::unordered_map<std::string, std::function<Indicator *(std::vector<IndicatorParam>)>> indicators_map = {};

    indicators_map.insert(candle_patterns_indicators_map.begin(), candle_patterns_indicators_map.end());
    indicators_map.insert(candle_signals_indicators_map.begin(), candle_signals_indicators_map.end());
    indicators_map.insert(candle_indicators_map.begin(), candle_indicators_map.end());
    indicators_map.insert(momentum_signals_indicators_map.begin(), momentum_signals_indicators_map.end());
    indicators_map.insert(momentum_indicators_map.begin(), momentum_indicators_map.end());
    indicators_map.insert(time_indicators_map.begin(), time_indicators_map.end());
    indicators_map.insert(trend_signals_indicators_map.begin(), trend_signals_indicators_map.end());
    indicators_map.insert(trend_indicators_map.begin(), trend_indicators_map.end());
    indicators_map.insert(volatility_signals_indicators_map.begin(), volatility_signals_indicators_map.end());
    indicators_map.insert(volatility_indicators_map.begin(), volatility_indicators_map.end());
    indicators_map.insert(volume_signals_indicators_map.begin(), volume_signals_indicators_map.end());
    indicators_map.insert(volume_indicators_map.begin(), volume_indicators_map.end());

    return indicators_map;
}

/**
 * @brief Extract the parameters from the ID.
 * @param id_params The ID to extract the parameters from.
 * @param id_params_pattern The regex pattern to match the ID.
 * @return std::vector<T> The extracted parameters.
 */
std::vector<IndicatorParam>
extract_parameters(const std::string &id_params, const std::string &id_params_pattern)
{
    std::regex pattern(id_params_pattern);
    std::smatch matches;

    if (std::regex_search(id_params, matches, pattern))
    {
        std::vector<IndicatorParam> parameters;

        for (size_t i = 1; i < matches.size(); i++)
        {
            std::regex intRegex("^-?\\d+$");
            std::regex doubleRegex("^-?\\d*(\\.\\d+)?$");

            if (std::regex_match(matches[i].str(), intRegex))
            {
                parameters.push_back(std::stoi(matches[i].str()));
            }
            else if (std::regex_match(matches[i].str(), doubleRegex))
            {
                parameters.push_back(std::stod(matches[i].str()));
            }
            else
            {
                parameters.push_back(matches[i].str());
            }
        }

        return parameters;
    }
    else
    {
        throw std::invalid_argument("Invalid ID format");
    }
}

/**
 * @brief Build an indicator from the ID.
 *
 * @param id_params The ID of the indicator with parameters.
 * @param params The parameters of the indicator.
 * @return Indicator The indicator.
 */
Indicator *create_indicator_from_id(const std::string &id_params, const std::vector<IndicatorParam> params)
{
    auto starts_with = [](const std::string &str, const std::string &prefix)
    {
        return str.find(prefix) == 0;
    };

    try
    {
        // Get the indicators map
        std::unordered_map<std::string, std::function<Indicator *(std::vector<IndicatorParam>)>> indicators_map = get_indicators_map();

        for (const auto &[indicator_id, indicator_constructor] : indicators_map)
        {
            if (starts_with(id_params, indicator_id))
            {
                // Check if the indicator ID exists in the map
                auto it = indicators_map.find(indicator_id);
                if (it != indicators_map.end())
                {
                    // Call the constructor function to create an instance of the indicator
                    return indicator_constructor(params);
                }
                else
                {
                    throw std::invalid_argument("No constructor found for indicator ID: " + indicator_id);
                }
            }
        }

        throw std::invalid_argument("No indicator found for ID: " + id_params);
    }
    catch (const std::invalid_argument &e)
    {
        std::cerr << e.what() << std::endl;
        return nullptr;
    }
}

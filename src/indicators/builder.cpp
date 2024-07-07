#include <string>
#include <vector>
#include <functional>
#include <map>
#include <sstream>
#include <iostream>
#include <regex>
#include "builder.hpp"
#include "indicator.hpp"

// Include the indicators
#include "momentum.hpp"

// Map linking the indicator ID to the constructor function
const std::map<std::string, std::function<Indicator *(std::vector<IndicatorParam>)>> indicators_map = {
    {"awesome-oscillator", [](std::vector<IndicatorParam> params) -> Indicator *
     {
         return new AwesomeOscillator(std::get<int>(params[0]));
     }},
    {"kama", [](std::vector<IndicatorParam> params) -> Indicator *
     {
         return new KAMA(std::get<int>(params[0]), std::get<double>(params[1]), std::get<double>(params[2]), std::get<double>(params[3]));
     }},
};

/**
 * @brief Extract the parameters from the ID.
 * @param id_params The ID to extract the parameters from.
 * @param id_params_pattern The regex pattern to match the ID.
 * @return std::vector<T> The extracted parameters.
 */
std::vector<IndicatorParam> extract_parameters(const std::string &id_params, const std::string &id_params_pattern)
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
    }
    catch (const std::invalid_argument &e)
    {
        std::cerr << e.what() << std::endl;
        return nullptr;
    }
}

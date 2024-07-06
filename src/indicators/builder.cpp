#include <string>
#include <vector>
#include <functional>
#include <map>
#include <sstream>
#include <iostream>
#include <regex>
#include <variant>
#include "builder.hpp"

// Include the indicators
#include "momentum.hpp"

// Map linking the indicator ID to the constructor function
const std::map<std::string, std::function<Indicator *(std::vector<IndicatorParams>)>> indicators_map = {
    {"awesome-oscillator", [](std::vector<IndicatorParams> params) -> Indicator *
     {
         return new AwesomeOscillator(std::get<int>(params[0]));
     }},
    {"kama", [](std::vector<IndicatorParams> params) -> Indicator *
     {
         return new KAMA(std::get<int>(params[0]), std::get<double>(params[1]), std::get<double>(params[2]), std::get<double>(params[3]));
     }},
};

/**
 * @brief Convert a string to a specified type.
 * @tparam T The type to convert the string to.
 * @param str The string to convert.
 * @return T The converted value.
 */
template <typename T>
T convert(const std::string &str)
{
    std::istringstream iss(str);
    T value;
    iss >> value;
    return value;
}

/**
 * @brief Extract the parameters from the ID.
 * @param id_params The ID to extract the parameters from.
 * @param id_pattern The regex pattern to match the ID.
 * @return std::vector<T> The extracted parameters.
 */
template <typename T>
std::vector<T> extract_parameters(const std::string &id_params, const std::string &id_pattern)
{
    std::regex pattern(id_pattern);
    std::smatch matches;

    if (std::regex_search(id_params, matches, pattern))
    {
        std::vector<T> parameters;
        for (size_t i = 1; i < matches.size(); i++)
        {
            parameters.push_back(convert<T>(matches[i].str()));
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
Indicator *create_indicator_from_id(const std::string &id_params, const std::vector<IndicatorParams> &params)
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

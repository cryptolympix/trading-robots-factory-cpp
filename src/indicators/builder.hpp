#ifndef INDICATORS_BUILDER_HPP
#define INDICATORS_BUILDER_HPP

#include <string>
#include <vector>
#include <functional>
#include <map>
#include <variant>
#include "indicator.hpp"

// Define IndicatorParams as a variant of int, double, and string
using IndicatorParams = std::variant<int, double, std::string>;

/**
 * @brief Extract the parameters from the ID.
 * @param id The ID to extract the parameters from.
 * @param id_pattern The regex pattern to match the ID.
 * @param group_number The group number to extract from the matches.
 * @return std::vector<T> The extracted parameters.
 */
template <typename T>
std::vector<T> extract_parameters(const std::string &id, const std::string &id_pattern);

/**
 * @brief Build an indicator from the ID.
 *
 * @param id The ID of the indicator.
 * @param id_pattern The regex pattern to match the ID.
 * @param params The parameters of the indicator.
 * @return Indicator The indicator.
 */
Indicator *create_indicator_from_id(const std::string &id, const std::string &id_pattern, std::vector<IndicatorParams> params);

#endif // INDICATORS_BUILDER_HPP
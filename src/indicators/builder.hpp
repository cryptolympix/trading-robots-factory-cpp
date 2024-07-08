#ifndef INDICATORS_BUILDER_HPP
#define INDICATORS_BUILDER_HPP

#include <string>
#include <vector>
#include <functional>
#include <unordered_map>
#include "indicator.hpp"

/**
 * @brief Get the indicators map.
 * @return std::map<std::string, std::function<Indicator *(std::vector<IndicatorParam>)>> The indicators map.
 */
std::unordered_map<std::string, std::function<Indicator *(std::vector<IndicatorParam>)>> get_indicators_map();

/**
 * @brief Extract the parameters from the ID.
 * @param id_params The ID to extract the parameters from.
 * @param id_params_pattern The regex pattern to match the ID.
 * @return std::vector<IndicatorParam> The extracted parameters.
 */
std::vector<IndicatorParam> extract_parameters(const std::string &id_params, const std::string &id_params_pattern);

/**
 * @brief Build an indicator from the ID.
 *
 * @param id_params The ID of the indicator with parameters.
 * @param params The parameters of the indicator.
 * @return Indicator The indicator.
 */
Indicator *create_indicator_from_id(const std::string &id_params, const std::vector<IndicatorParam> params);

#endif // INDICATORS_BUILDER_HPP
#ifndef INDICATORS_BUILDER_HPP
#define INDICATORS_BUILDER_HPP

#include <string>
#include <functional>
#include <unordered_map>
#include <typeindex>
#include "indicator.hpp"

/**
 * @brief Get the indicators map.
 * @return std::map<std::string, std::function<Indicator *(std::vector<IndicatorParam>)>> The indicators map.
 */
std::unordered_map<std::string, std::function<Indicator *(std::unordered_map<std::string, IndicatorParam>)>> get_indicators_map();

/**
 * @brief Extract the parameters from the ID.
 * @param id_params The ID to extract the parameters from.
 * @param id_params_pattern The regex pattern to match the ID.
 * @return std::unordered_map<std::string, IndicatorParam> The extracted parameters.
 */
std::unordered_map<std::string, IndicatorParam> extract_parameters(const std::string &id_params, const std::string &id_params_pattern);

/**
 * @brief Check if the parameters are valid with the ID pattern.
 *
 * @param params The parameters.
 * @param required_params The required parameters with their types.
 *
 * @return true If the parameters are valid with the ID pattern.
 */
bool check_params(const std::unordered_map<std::string, IndicatorParam> &params, const std::unordered_map<std::string, std::type_index> &required_params);

/**
 * @brief Build an indicator from the ID.
 *
 * @param id The ID of the indicator.
 * @param params The parameters of the indicator.
 * @return Indicator The indicator.
 */
Indicator *create_indicator_from_id(const std::string &id, const std::unordered_map<std::string, IndicatorParam> params);

#endif // INDICATORS_BUILDER_HPP
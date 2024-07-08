#ifndef INDICATOR_H
#define INDICATOR_H

#include <string>
#include <vector>
#include <functional>
#include <utility>
#include <regex>
#include <variant>
#include <unordered_map>
#include "../types.hpp"

// Define IndicatorParam as a variant of int, double, and string
using IndicatorParam = std::variant<int, double, std::string>;

// Forward declaration
struct Candle;

/**
 * @brief Check if the ID with parameters is valid.
 *
 * @param id_params The ID with parameters.
 * @param id_params_pattern The regex pattern to match the ID with parameters.
 * @return true If the ID with parameters is valid.
 */
bool is_valid_id_with_params(const std::string &id_params, const std::string &id_params_pattern);

/**
 * @brief Check if the parameters are valid with the ID pattern.
 *
 * @param id The ID.
 * @param params The parameters.
 * @param id_params_pattern The regex pattern to match the ID with parameters.
 * @return true If the parameters are valid with the ID pattern.
 */
bool is_valid_params_with_id_params_pattern(const std::string &id, const std::vector<IndicatorParam> &params, const std::string &id_params_pattern);

/**
 * @brief Class for Indicator.
 */
class Indicator
{
public:
    std::string label;                                      // The label of the indicator.
    std::string id;                                         // The id of the indicator.
    std::string id_params;                                  // The id of the indicator with parameters.
    std::string id_params_pattern;                          // The regex pattern to match the ID with parameters.
    std::unordered_map<std::string, IndicatorParam> params; // A map of the parameters.
    std::pair<double, double> values_range;                 // The range of values.

    /**
     * @brief Construct a new Indicator::Indicator object.
     *
     * @param label The label of the indicator.
     * @param id The id of the indicator.
     * @param params The parameters of the indicator.
     * @param values_range The range of values.
     */
    Indicator(std::string label, std::string id, std::unordered_map<std::string, IndicatorParam> params, std::pair<double, double> values_range = std::make_pair(0, 0));

    /**
     * @brief Calculate the indicator.
     *
     * @param candles The candles data.
     * @param calculator The calculator function.
     * @param normalize_data Normalize the data.
     * @param values_range The range of values.
     * @return std::vector<double> The calculated indicator values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, std::function<std::vector<double>(std::vector<Candle>)> calculator, bool normalize_data = false) const;

    /**
     * @brief Calculate the indicator.
     *
     * @param candles The candles data.
     * @param normalize_data Normalize the data.
     * @param values_range The range of values.
     * @return std::vector<double> The calculated indicator values.
     */
    virtual std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const;

protected:
    std::vector<double> values;
};

#endif // INDICATOR_H
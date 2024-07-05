#ifndef INDICATOR_H
#define INDICATOR_H

#include <string>
#include <vector>
#include <functional>
#include <utility>
#include <regex>
#include "../types.hpp"

// Forward declaration
struct Candle;

/**
 * @brief Class for Indicator.
 */
class Indicator
{
public:
    std::string label;                      // The label of the indicator.
    std::string id;                         // The id of the indicator.
    int offset;                             // The offset of the indicator.
    std::string id_pattern;                 // The regex pattern to match the ID.
    std::pair<double, double> values_range; // The range of values.

    /**
     * @brief Construct a new Indicator::Indicator object.
     *
     * @param label The label of the indicator.
     * @param id The id of the indicator.
     * @param id_pattern The id pattern of the indicator.
     * @param offset The offset of the indicator.
     * @param values_range The range of values.
     */
    Indicator(const std::string &label, const std::string &id, std::string id_pattern, int offset = 0, std::pair<double, double> values_range = std::make_pair(0, 0));

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
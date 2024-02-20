#ifndef INDICATOR_H
#define INDICATOR_H

#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <functional>
#include "../types.hpp"

// Forward declaration
class Candle;

/**
 * @brief Class for Indicator.
 */
class Indicator
{
public:
    /**
     * @brief Construct a new Indicator::Indicator object.
     *
     * @param label The label of the indicator.
     * @param id The id of the indicator.
     * @param offset The offset of the indicator.
     */
    Indicator(const std::string &label, const std::string &id, int offset = 0);

    /**
     * @brief Calculate the indicator.
     *
     * @param candles The candles data.
     * @param calculator The calculator function.
     * @param normalize_data Normalize the data.
     * @return std::vector<double> The calculated indicator values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, std::function<std::vector<double>(std::vector<Candle>)> calculator, bool normalize_data = false) const;

    virtual std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data) const;

protected:
    std::string label;
    std::vector<double> values;
    std::string id;
    int offset;
};

#endif // INDICATOR_H
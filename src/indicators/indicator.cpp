#include "../utils/math.hpp"
#include "indicator.hpp"
#include "../types.hpp"

/**
 * @brief Construct a new Indicator::Indicator object.
 */
Indicator::Indicator(const std::string &label, const std::string &id, int offset)
    : label(label), id(id), offset(offset)
{
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
    else if (candles.size() < 2)
    {
        std::cerr << "Candles data is not enough to calculate the indicator." << std::endl;
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

    // Calculate the indicator
    std::vector<double> values = calculator(candles);

    // Normalize the data
    if (normalize_data)
    {
        values = normalize(values);
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
        { return std::vector<double>(); },
        normalize_data);
}
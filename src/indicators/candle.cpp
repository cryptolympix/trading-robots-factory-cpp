#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <ctime>
#include <cmath>
#include "../utils/math.hpp"
#include "../utils/candles_source.hpp"
#include "../types.hpp"
#include "indicator.hpp"
#include "candle.hpp"

/**
 * @brief Construct a new CandleOpen object.
 *
 * @param offset Offset value. Default is 0.
 */
CandleOpen::CandleOpen(int offset) : Indicator("Candle Open", "candle-open-" + std::to_string(offset), offset) {}

/**
 * @brief Calculate the CandleOpen values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> CandleOpen::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles) -> std::vector<double>
        {
                std::vector<double> values;
                for (int i = 0; i < candles.size(); ++i)
                {
                    values.push_back(candles[i].open);
                }
                return values; },
        normalize_data);
}

/**
 * @brief Construct a new CandleHigh object.
 *
 * @param offset Offset value. Default is 0.
 */
CandleHigh::CandleHigh(int offset) : Indicator("Candle High", "candle-high-" + std::to_string(offset), offset) {}

/**
 * @brief Calculate the CandleHigh values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> CandleHigh::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles) -> std::vector<double>
        {
            std::vector<double> values;
            for (int i = 0; i < candles.size(); ++i)
            {
                values.push_back(candles[i].high);
            }
            return values; },

        normalize_data);
}

/**
 * @brief Construct a new CandleLow object.
 *
 * @param offset Offset value. Default is 0.
 */
CandleLow::CandleLow(int offset) : Indicator("Candle Low", "candle-low-" + std::to_string(offset), offset) {}

/**
 * @brief Calculate the CandleLow values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> CandleLow::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles) -> std::vector<double>
        {
            std::vector<double> values;
            for (int i = 0; i < candles.size(); ++i)
            {
                values.push_back(candles[i].low);
            }
            return values; },

        normalize_data);
}

/**
 * @brief Construct a new CandleClose object.
 *
 * @param offset Offset value. Default is 0.
 */
CandleClose::CandleClose(int offset) : Indicator("Candle Close", "candle-close-" + std::to_string(offset), offset) {}

/**
 * @brief Calculate the CandleClose values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> CandleClose::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles) -> std::vector<double>
        {
            std::vector<double> values;
            for (int i = 0; i < candles.size(); ++i)
            {
                values.push_back(candles[i].close);
            }
            return values; },

        normalize_data);
}

/**
 * @brief Construct a new WhiteCandle object.
 *
 * @param offset Offset value. Default is 0.
 */
WhiteCandle::WhiteCandle(int offset) : Indicator("White Candle", "white-candle-" + std::to_string(offset), offset) {}

/**
 * @brief Calculate the WhiteCandle values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> WhiteCandle::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles) -> std::vector<double>
        {
            std::vector<double> values;
            for (int i = 0; i < candles.size(); ++i)
            {
                values.push_back(candles[i].close >= candles[i].open ? 1 : 0);
            }
            return values; },

        normalize_data);
}

/**
 * @brief Construct a new BlackCandle object.
 *
 * @param offset Offset value. Default is 0.
 */
BlackCandle::BlackCandle(int offset) : Indicator("Black Candle", "black-candle-" + std::to_string(offset), offset) {}

/**
 * @brief Calculate the BlackCandle values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> BlackCandle::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles) -> std::vector<double>
        {
            std::vector<double> values;
            for (int i = 0; i < candles.size(); ++i)
            {
                values.push_back(candles[i].close <= candles[i].open ? 1 : 0);
            }
            return values; },

        normalize_data);
}

/**
 * @brief Construct a new CandlePriceChange object.
 *
 * @param offset Offset value. Default is 0.
 */
CandlePriceChange::CandlePriceChange(int offset) : Indicator("Candle Price Change", "candle-price-change-" + std::to_string(offset), offset) {}

/**
 * @brief Calculate the CandlePriceChange values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> CandlePriceChange::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles) -> std::vector<double>
        {
            std::vector<double> values;
            for (size_t i = 0; i < candles.size(); ++i)
            {
                values.push_back((candles[i].close - candles[i].open)/candles[i].open);
            }
            return values; },

        normalize_data);
}

/**
 * @brief Construct a new PivotHigh object.
 *
 * @param source Source of candle data.
 * @param left_bars Number of left bars.
 * @param right_bars Number of right bars.
 * @param offset Offset value. Default is 0.
 */
PivotHigh::PivotHigh(CandleSource source, int left_bars, int right_bars, int offset)
    : Indicator("Pivot High", "pivot-high-" + CandleSourceMap[source] + "-" + std::to_string(left_bars) + "-" + std::to_string(right_bars) + "-" + std::to_string(offset), offset), source(source), left_bars(left_bars), right_bars(right_bars) {}

/**
 * @brief Calculate the PivotHigh values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> PivotHigh::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles) -> std::vector<double>
        {
            std::vector<double> values(candles.size(), 0); // Initialize values vector with size of candles

            std::vector<double> source_candles = get_candles_with_source(candles, source);

            // Ensure source_candles size is at least candles.size()
            if (source_candles.size() < candles.size()) {
                // Handle error or return an empty vector
                return std::vector<double>();
            }

            for (size_t i = 0; i < candles.size() - right_bars; ++i)
            {
                if (i < left_bars)
                {
                    values[i] = (std::max_element(source_candles.begin(), source_candles.begin() + i + right_bars + 1) == source_candles.begin() + i) ? 1 : 0;
                }
                else
                {
                    values[i] = (std::max_element(source_candles.begin() + i - left_bars, source_candles.begin() + i + right_bars + 1) == source_candles.begin() + i) ? 1 : 0;
                }
            }

            if (offset > 0)
            {
                values.insert(values.begin(), offset, std::numeric_limits<int>::quiet_NaN());
            }

            return values; },

        normalize_data);
}

/**
 * @brief Construct a new PivotLow object.
 *
 * @param source Source of candle data.
 * @param left_bars Number of left bars.
 * @param right_bars Number of right bars.
 * @param offset Offset value. Default is 0.
 */
PivotLow::PivotLow(CandleSource source, int left_bars, int right_bars, int offset)
    : Indicator("Pivot Low", "pivot-low-" + CandleSourceMap[source] + "-" + std::to_string(left_bars) + "-" + std::to_string(right_bars) + "-" + std::to_string(offset), offset), source(source), left_bars(left_bars), right_bars(right_bars) {}

/**
 * @brief Calculate the PivotLow values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> PivotLow::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles) -> std::vector<double>
        {
            std::vector<double> values(candles.size(), 0); // Initialize values vector with size of candles

            std::vector<double> source_candles = get_candles_with_source(candles, source);

            // Ensure source_candles size is at least candles.size()
            if (source_candles.size() < candles.size())
            {
                // Handle error or return an empty vector
                return std::vector<double>();
            }

            for (size_t i = 0; i < candles.size() - right_bars; ++i)
            {
                if (i < left_bars)
                {
                    values[i] = (std::min_element(source_candles.begin(), source_candles.begin() + i + right_bars + 1) == source_candles.begin() + i) ? 1 : 0;
                }
                else
                {
                    values[i] = (std::min_element(source_candles.begin() + i - left_bars, source_candles.begin() + i + right_bars + 1) == source_candles.begin() + i) ? 1 : 0;
                }
            }

            if (offset > 0)
            {
                values.insert(values.begin(), offset, std::numeric_limits<int>::quiet_NaN());
            }

            return values; },

        normalize_data);
}

/**
 * @brief Construct a new Lowest object.
 *
 * @param source Source of candle data.
 * @param left_bars Number of left bars.
 * @param right_bars Number of right bars.
 * @param offset Offset value. Default is 0.
 */
Lowest::Lowest(CandleSource source, int left_bars, int right_bars, int offset)
    : Indicator("Lowest", "lowest-" + CandleSourceMap[source] + "-" + std::to_string(left_bars) + "-" + std::to_string(right_bars) + "-" + std::to_string(offset), offset), source(source), left_bars(left_bars), right_bars(right_bars) {}

/**
 * @brief Calculate the Lowest values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> Lowest::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles) -> std::vector<double>
        { 
            std::vector<double> values(candles.size(), 0); // Initialize values vector with size of candles
            PivotLow indicator(CandleSource::Low, left_bars, right_bars, offset);
            std::vector<double> pivots = indicator.calculate(candles, false);
            std::vector<double> source_candles = get_candles_with_source(candles, source);

            int current_pivot_index = 0;
            for (size_t i = 0; i < candles.size() - right_bars; ++i)
            {
                if (i < right_bars)
                {
                    values[i] = source_candles[0];
                }
                if (pivots[i] == 1)
                {
                    current_pivot_index = i;
                }
                values[i + right_bars] = source_candles[current_pivot_index];
            }

            // Adjust values if offset is greater than 0
            if (offset > 0)
            {
                std::vector<double> nan_values(offset, NAN);
                values.insert(values.begin(), nan_values.begin(), nan_values.end());
            }

            return values; },

        normalize_data);
}

/**
 * @brief Construct a new Highest object.
 *
 * @param source Source of candle data.
 * @param left_bars Number of left bars.
 * @param right_bars Number of right bars.
 * @param offset Offset value. Default is 0.
 */
Highest::Highest(CandleSource source, int left_bars, int right_bars, int offset)
    : Indicator("Highest", "highest-" + CandleSourceMap[source] + "-" + std::to_string(left_bars) + "-" + std::to_string(right_bars) + "-" + std::to_string(offset), offset), source(source), left_bars(left_bars), right_bars(right_bars) {}

/**
 * @brief Calculate the Highest values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> Highest::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles) -> std::vector<double>
        { 
            std::vector<double> values(candles.size(), 0); // Initialize values vector with size of candles
            PivotHigh indicator(CandleSource::High, left_bars, right_bars, offset);
            std::vector<double> pivots = indicator.calculate(candles, false);
            std::vector<double> source_candles = get_candles_with_source(candles, source);

            int current_pivot_index = 0;
            for (size_t i = 0; i < candles.size() - right_bars; ++i)
            {
                if (i < right_bars)
                {
                    values[i] = source_candles[0];
                }
                if (pivots[i] == 1)
                {
                    current_pivot_index = i;
                }
                values[i + right_bars] = source_candles[current_pivot_index];
            }

            // Adjust values if offset is greater than 0
            if (offset > 0)
            {
                std::vector<double> nan_values(offset, NAN);
                values.insert(values.begin(), nan_values.begin(), nan_values.end());
            }

            return values; },

        normalize_data);
}

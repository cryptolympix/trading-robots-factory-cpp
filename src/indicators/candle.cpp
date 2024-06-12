#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <ctime>
#include <cmath>
#include "../utils/math.hpp"
#include "../utils/candles_source.hpp"
#include "../types.hpp"
#include "utils.hpp"
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

// *********************************************************************************************

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

// *********************************************************************************************

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

// *********************************************************************************************

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

// *********************************************************************************************

/**
 * @brief Construct a new CandleTickVolume object.
 *
 * @param offset Offset value. Default is 0.
 */
CandleTickVolume::CandleTickVolume(int offset) : Indicator("Candle Tick Volume", "candle-tick-volume-" + std::to_string(offset), offset) {}

/**
 * @brief Calculate the CandleTickVolume values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> CandleTickVolume::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles) -> std::vector<double>
        {
            std::vector<double> values;
            for (int i = 0; i < candles.size(); ++i)
            {
                values.push_back(candles[i].tick_volume);
            }
            return values; },

        normalize_data);
}

// *********************************************************************************************

/**
 * @brief Construct a new CandleVolume object.
 *
 * @param offset Offset value. Default is 0.
 */
CandleVolume::CandleVolume(int offset) : Indicator("Candle Volume", "candle-volume-" + std::to_string(offset), offset) {}

/**
 * @brief Calculate the CandleVolume values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> CandleVolume::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles) -> std::vector<double>
        {
            std::vector<double> values;
            for (int i = 0; i < candles.size(); ++i)
            {
                values.push_back(candles[i].volume);
            }
            return values; },

        normalize_data);
}

// *********************************************************************************************

/**
 * @brief Construct a new WhiteCandle object.
 *
 * @param offset Offset value. Default is 0.
 */
WhiteCandle::WhiteCandle(int offset) : Indicator("White Candle", "white-candle-" + std::to_string(offset), offset, {0, 1}) {}

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

// *********************************************************************************************

/**
 * @brief Construct a new BlackCandle object.
 *
 * @param offset Offset value. Default is 0.
 */
BlackCandle::BlackCandle(int offset) : Indicator("Black Candle", "black-candle-" + std::to_string(offset), offset, {0, 1}) {}

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

// *********************************************************************************************

/**
 * @brief Construct a new CandleBody object.
 *
 * @param offset Offset value. Default is 0.
 */
CandleBody::CandleBody(int offset) : Indicator("Candle Body", "candle-body-" + std::to_string(offset), offset, {0, 1}) {}

/**
 * @brief Calculate the CandleBody values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> CandleBody::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles) -> std::vector<double>
        {
            // Initialize values vector with size of candles
            std::vector<double> values(candles.size(), 0);

            for (int i = 0; i < candles.size(); ++i)
            {
                if (candles[i].high != candles[i].low)
                {
                    // Calculate the ratio of body to the entire candle
                    values[i] = std::abs(candles[i].close - candles[i].open) / (candles[i].high - candles[i].low);
                }
            }
            return values; },

        normalize_data);
}

// *********************************************************************************************

/**
 * @brief Construct a new CandleShadowUpper object.
 *
 * @param offset Offset value. Default is 0.
 */
CandleShadowUpper::CandleShadowUpper(int offset) : Indicator("Candle Shadow Upper", "candle-shadow-upper-" + std::to_string(offset), offset, {0, 1}) {}

/**
 * @brief Calculate the CandleShadowUpper values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> CandleShadowUpper::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles) -> std::vector<double>
        {
            std::vector<double> values;
            for (int i = 0; i < candles.size(); ++i)
            {
                if (candles[i].high != candles[i].low)
                {
                    // Calculate the ratio of upper shadow to the entire candle
                    values.push_back((candles[i].high - std::max(candles[i].open, candles[i].close)) / (candles[i].high - candles[i].low));
                }
            }
            return values; },

        normalize_data);
}

// *********************************************************************************************

/**
 * @brief Construct a new CandleShadowLower object.
 *
 * @param offset Offset value. Default is 0.
 */
CandleShadowLower::CandleShadowLower(int offset) : Indicator("Candle Shadow Lower", "candle-shadow-lower-" + std::to_string(offset), offset, {0, 1}) {}

/**
 * @brief Calculate the CandleShadowLower values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> CandleShadowLower::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles) -> std::vector<double>
        {
            std::vector<double> values;
            for (int i = 0; i < candles.size(); ++i)
            {
                if (candles[i].high != candles[i].low)
                {
                    // Calculate the ratio of lower shadow to the entire candle
                    values.push_back((std::min(candles[i].open, candles[i].close) - candles[i].low) / (candles[i].high - candles[i].low));
                }
            }
            return values; },

        normalize_data);
}

// *********************************************************************************************

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
                values.push_back((candles[i].close - candles[i].open) / candles[i].open);
            }
            return values; },

        normalize_data);
}

// *********************************************************************************************

/**
 * @brief Construct a new PivotHigh object.
 *
 * @param source Source of candle data.
 * @param left_bars Number of left bars.
 * @param right_bars Number of right bars.
 * @param offset Offset value. Default is 0.
 */
PivotHigh::PivotHigh(std::string source, int left_bars, int right_bars, int offset)
    : Indicator("Pivot High", "pivot-high-" + source + "-" + std::to_string(left_bars) + "-" + std::to_string(right_bars) + "-" + std::to_string(offset), offset, {0, 1}), source(source), left_bars(left_bars), right_bars(right_bars) {}

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
                return values;
            }

            values = calculate_pivots_high(source_candles, left_bars, right_bars);

            return values; },

        normalize_data);
}

// *********************************************************************************************

/**
 * @brief Construct a new PivotLow object.
 *
 * @param source Source of candle data.
 * @param left_bars Number of left bars.
 * @param right_bars Number of right bars.
 * @param offset Offset value. Default is 0.
 */
PivotLow::PivotLow(std::string source, int left_bars, int right_bars, int offset)
    : Indicator("Pivot Low", "pivot-low-" + source + "-" + std::to_string(left_bars) + "-" + std::to_string(right_bars) + "-" + std::to_string(offset), offset, {0, 1}), source(source), left_bars(left_bars), right_bars(right_bars) {}

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
                return values;
            }

            values = calculate_pivots_low(source_candles, left_bars, right_bars);

            return values; },

        normalize_data);
}

// *********************************************************************************************

/**
 * @brief Construct a new PivotHighValue object.
 *
 * @param source Source of candle data.
 * @param left_bars Number of left bars.
 * @param right_bars Number of right bars.
 * @param offset Offset value. Default is 0.
 */
PivotHighValue::PivotHighValue(std::string source, int left_bars, int right_bars, int offset)
    : Indicator("Highest", "pivot-high-value-" + source + "-" + std::to_string(left_bars) + "-" + std::to_string(right_bars) + "-" + std::to_string(offset), offset), source(source), left_bars(left_bars), right_bars(right_bars) {}

/**
 * @brief Calculate the PivotHighValue values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> PivotHighValue::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles) -> std::vector<double>
        {
            std::vector<double> values(candles.size(), 0); // Initialize values vector with size of candles
            std::vector<double> pivots = PivotHigh("high", left_bars, right_bars, offset).calculate(candles, false);
            std::vector<double> source_candles = get_candles_with_source(candles, source);

            int current_pivot_index = 0;

            for (size_t i = 0; i < right_bars; ++i)
            {
                if (pivots[i] == 1)
                {
                    current_pivot_index = i;
                }
                values[i] = source_candles[current_pivot_index];
            }

            for (size_t i = 0; i < candles.size() - right_bars; ++i)
            {
                if (pivots[i] == 1)
                {
                    current_pivot_index = i;
                }
                values[i + right_bars] = source_candles[current_pivot_index];
            }

            return values; },

        normalize_data);
}

// *********************************************************************************************

/**
 * @brief Construct a new PivotLowValue object.
 *
 * @param source Source of candle data.
 * @param left_bars Number of left bars.
 * @param right_bars Number of right bars.
 * @param offset Offset value. Default is 0.
 */
PivotLowValue::PivotLowValue(std::string source, int left_bars, int right_bars, int offset)
    : Indicator("Lowest", "pivot-low-value-" + source + "-" + std::to_string(left_bars) + "-" + std::to_string(right_bars) + "-" + std::to_string(offset), offset), source(source), left_bars(left_bars), right_bars(right_bars) {}

/**
 * @brief Calculate the PivotLowValue values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> PivotLowValue::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles) -> std::vector<double>
        {
            std::vector<double> values(candles.size(), 0); // Initialize values vector with size of candles
            std::vector<double> pivots = PivotLow("low", left_bars, right_bars, offset).calculate(candles, false);
            std::vector<double> source_candles = get_candles_with_source(candles, source);

            int current_pivot_index = 0;

            for (size_t i = 0; i < right_bars; ++i)
            {
                if (pivots[i] == 1)
                {
                    current_pivot_index = i;
                }
                values[i] = source_candles[current_pivot_index];
            }

            for (size_t i = 0; i < candles.size() - right_bars; ++i)
            {
                if (pivots[i] == 1)
                {
                    current_pivot_index = i;
                }
                values[i + right_bars] = source_candles[current_pivot_index];
            }

            return values; },

        normalize_data);
}

// *********************************************************************************************

/**
 * @brief Construct a new HighestHigh object.
 *
 * @param left_bars Number of left bars.
 * @param right_bars Number of right bars.
 * @param offset Offset value. Default is 0.
 */
HighestHigh::HighestHigh(int left_bars, int right_bars, int offset) : Indicator("Highest High", "highest-high-" + std::to_string(left_bars) + "-" + std::to_string(right_bars) + "-" + std::to_string(offset), offset), left_bars(left_bars), right_bars(right_bars) {}

/**
 * @brief Calculate the HighestHigh values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> HighestHigh::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles) -> std::vector<double>
        {
            std::vector<double> values(candles.size(), 0); // Initialize values vector with size of candles

            std::vector<double> highs_values = get_candles_with_source(candles, "high");

            for (size_t i = 0; i < candles.size(); ++i)
            {
                // Initialize highest high to lowest possible value
                double highest_high = std::numeric_limits<double>::min();

                // Determine the range to check for highest high
                size_t start_index = (i < left_bars) ? 0 : i - left_bars;
                size_t end_index = (i + right_bars >= candles.size()) ? candles.size() - 1 : i + right_bars;

                // Iterate through the range to find the highest high
                for (size_t j = start_index; j <= end_index; ++j)
                {
                    highest_high = std::max(highest_high, highs_values[j]);
                }

                // Assign the highest high to the corresponding value in the result vector
                values[i] = highest_high;
            }

            return values; },

        normalize_data);
}

// *********************************************************************************************

/**
 * @brief Construct a new LowestLow object.
 *
 * @param left_bars Number of left bars.
 * @param right_bars Number of right bars.
 * @param offset Offset value. Default is 0.
 */
LowestLow::LowestLow(int left_bars, int right_bars, int offset) : Indicator("Lowest Low", "lowest-low-" + std::to_string(left_bars) + "-" + std::to_string(right_bars) + "-" + std::to_string(offset), offset), left_bars(left_bars), right_bars(right_bars) {}

/**
 * @brief Calculate the LowestLow values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> LowestLow::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles) -> std::vector<double>
        {
            std::vector<double> values(candles.size(), 0); // Initialize values vector with size of candles

            std::vector<double> lows_values = get_candles_with_source(candles, "low");

            for (size_t i = 0; i < candles.size(); ++i)
            {
                // Initialize lowest low to highest possible value
                double lowest_low = std::numeric_limits<double>::max();

                // Determine the range to check for lowest low
                size_t start_index = (i < left_bars) ? 0 : i - left_bars;
                size_t end_index = (i + right_bars >= candles.size()) ? candles.size() - 1 : i + right_bars;

                // Iterate through the range to find the lowest low
                for (size_t j = start_index; j <= end_index; ++j)
                {
                    lowest_low = std::min(lowest_low, lows_values[j]);
                }

                // Assign the lowest low to the corresponding value in the result vector
                values[i] = lowest_low;
            }

            return values; },

        normalize_data);
}

// *********************************************************************************************

/**
 * @brief Construct a new PeakDistance object.
 *
 * @param left_bars Number of left bars.
 * @param right_bars Number of right bars.
 * @param offset Offset value. Default is 0.
 */
PeakDistance::PeakDistance(int left_bars, int right_bars, int offset) : Indicator("Peak Distance", "peak-distance-" + std::to_string(left_bars) + "-" + std::to_string(right_bars) + "-" + std::to_string(offset), offset), left_bars(left_bars), right_bars(right_bars) {}

/**
 * @brief Calculate the PeakDistance values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> PeakDistance::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles) -> std::vector<double>
        {
            std::vector<double> values(candles.size(), 0); // Initialize values vector with size of candles

            std::vector<double> highest_highs = HighestHigh(left_bars, right_bars, offset).calculate(candles, false);
            std::vector<double> lowest_lows = LowestLow(left_bars, right_bars, offset).calculate(candles, false);

            for (size_t i = 0; i < candles.size(); ++i)
            {
                // Calculate the distance between the highest high and lowest low
                values[i] = highest_highs[i] / lowest_lows[i];
            }

            return values; },

        normalize_data);
}

// *********************************************************************************************

/**
 * @brief Construct a new PeakCandleDistance object.
 *
 * @param left_bars Number of left bars.
 * @param right_bars Number of right bars.
 * @param offset Offset value. Default is 0.
 */
PeakCandleDistance::PeakCandleDistance(int left_bars, int right_bars, int offset) : Indicator("Peak Candle Distance", "peak-candle-distance-" + std::to_string(left_bars) + "-" + std::to_string(right_bars) + "-" + std::to_string(offset), offset, {0, left_bars + right_bars}), left_bars(left_bars), right_bars(right_bars) {}

/**
 * @brief Calculate the PeakCandleDistance values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> PeakCandleDistance::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles) -> std::vector<double>
        {
            std::vector<double> values(candles.size(), 0); // Initialize values vector with size of candles

            // Find the pivots
            std::vector<double> pivots_high_values = PivotHigh("high", left_bars, right_bars, offset).calculate(candles, false);
            std::vector<double> pivots_low_values = PivotLow("low", left_bars, right_bars, offset).calculate(candles, false);

            for (size_t i = 0; i < candles.size(); ++i)
            {
                // Calculate the distance in number of candles from the previous pivots high or low
                if (pivots_high_values[i] == 1 || pivots_low_values[i] == 1)
                {
                    values[i] = 0;
                }
                else
                {
                    values[i] = values[i - 1] + 1;
                }
            }

            return values; },

        normalize_data);
}

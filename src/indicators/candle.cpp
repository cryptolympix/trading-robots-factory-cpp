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

// *********************************************************************************************

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
                values.push_back((candles[i].close - candles[i].open)/candles[i].open);
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
                return values;
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
                return values;
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
PivotHighValue::PivotHighValue(CandleSource source, int left_bars, int right_bars, int offset)
    : Indicator("Highest", "highest-" + CandleSourceMap[source] + "-" + std::to_string(left_bars) + "-" + std::to_string(right_bars) + "-" + std::to_string(offset), offset), source(source), left_bars(left_bars), right_bars(right_bars) {}

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
PivotLowValue::PivotLowValue(CandleSource source, int left_bars, int right_bars, int offset)
    : Indicator("Lowest", "lowest-" + CandleSourceMap[source] + "-" + std::to_string(left_bars) + "-" + std::to_string(right_bars) + "-" + std::to_string(offset), offset), source(source), left_bars(left_bars), right_bars(right_bars) {}

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

            std::vector<double> highs_values = get_candles_with_source(candles, CandleSource::High);

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

            std::vector<double> lows_values = get_candles_with_source(candles, CandleSource::Low);

            for (size_t i = 0; i < candles.size(); ++i)
            {
                // Initialize lowest low to highest possible value
                double lowest_low = std::numeric_limits<double>::max(); 

                // Determine the range to check for lowest low
                size_t start_index = (i < left_bars) ? 0 : i - left_bars;
                size_t end_index = (i + right_bars >= candles.size()) ? candles.size() - 1 : i + right_bars;

                // Iterate through the range to find the lowesr low
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
 * @brief Construct a new CandlePattern object.
 *
 * @param offset Offset value. Default is 0.
 */
CandlePattern::CandlePattern(int offset) : Indicator("Candle Pattern", "candle-pattern-" + std::to_string(offset), offset) {}

/**
 * @brief Calculate the CandlePattern values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> CandlePattern::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles) -> std::vector<double>
        {
            std::vector<double> values(candles.size(), 0); // Initialize values vector with size of candles

            for (size_t i = 0; i < candles.size(); ++i)
            {
                if (isHammer(candles, i))
                {
                    values[i] = 1;
                }
                else if (isShootingStar(candles, i))
                {
                    values[i] = 2;
                }
                else if (isBullishEngulfing(candles, i))
                {
                    values[i] = 3;
                }
                else if (isBearishEngulfing(candles, i))
                {
                    values[i] = 4;
                }
                else if (isMorningStar(candles, i))
                {
                    values[i] = 5;
                }
                else if (isEveningStar(candles, i))
                {
                    values[i] = 6;
                }
                else if (isDoji(candles, i))
                {
                    values[i] = 7;
                }
                else if (isSpinningTop(candles, i))
                {
                    values[i] = 8;
                }
                else if (isMarubozu(candles, i))
                {
                    values[i] = 9;
                }
                else if (isHarami(candles, i))
                {
                    values[i] = 10;
                }
                else if (isHaramiCross(candles, i))
                {
                    values[i] = 11;
                }
                else if (isThreeWhiteSoldiers(candles, i))
                {
                    values[i] = 12;
                }
                else if (isThreeBlackCrows(candles, i))
                {
                    values[i] = 13;
                }
            }

            return values; },

        normalize_data);
}

/**
 * @brief Detect the presence of a hammer candlestick pattern.
 *
 * @param candles Vector of Candle data.
 * @param index Index of the candle to analyze.
 * @return true if hammer pattern is detected, false otherwise.
 */
bool CandlePattern::isHammer(const std::vector<Candle> &candles, size_t index) const
{
    if (index < 2 || index >= candles.size() - 1)
    {
        return false;
    }

    const Candle &current = candles[index];
    const Candle &previous = candles[index - 1];
    const Candle &prior = candles[index - 2];

    // Check if the candle is a hammer
    bool bodyShort = std::abs(current.close - current.open) < std::abs(current.high - current.low) * 0.25;
    bool lowerShadowLong = (current.low - std::min(current.open, current.close)) > (current.close - current.open) * 2;
    bool upperShadowShort = (std::max(current.open, current.close) - current.high) < (current.close - current.open) * 0.5;
    bool closeAbovePrior = current.close > previous.close;

    return bodyShort && lowerShadowLong && upperShadowShort && closeAbovePrior;
}

/**
 * @brief Detect the presence of a shooting star candlestick pattern.
 *
 * @param candles Vector of Candle data.
 * @param index Index of the candle to analyze.
 * @return true if shooting star pattern is detected, false otherwise.
 */
bool CandlePattern::isShootingStar(const std::vector<Candle> &candles, size_t index) const
{
    if (index < 2 || index >= candles.size() - 1)
    {
        return false;
    }

    const Candle &current = candles[index];
    const Candle &previous = candles[index - 1];
    const Candle &prior = candles[index - 2];

    // Check if the candle is a shooting star
    bool bodyShort = std::abs(current.close - current.open) < std::abs(current.high - current.low) * 0.25;
    bool upperShadowLong = (std::max(current.open, current.close) - current.high) > (current.close - current.open) * 2;
    bool lowerShadowShort = (current.low - std::min(current.open, current.close)) < (current.close - current.open) * 0.5;
    bool closeBelowPrior = current.close < previous.close;

    return bodyShort && upperShadowLong && lowerShadowShort && closeBelowPrior;
}

/**
 * @brief Detect the presence of a bullish engulfing candlestick pattern.
 *
 * @param candles Vector of Candle data.
 * @param index Index of the candle to analyze.
 * @return true if bullish engulfing pattern is detected, false otherwise.
 */
bool CandlePattern::isBullishEngulfing(const std::vector<Candle> &candles, size_t index) const
{
    if (index < 1 || index >= candles.size() - 1)
    {
        return false;
    }

    // Current candle
    const Candle &current = candles[index];
    // Previous candle
    const Candle &previous = candles[index - 1];

    // Check if the current candle is bullish (close > open)
    if (current.close <= current.open)
    {
        return false;
    }

    // Check if the previous candle is bearish (close < open)
    if (previous.close >= previous.open)
    {
        return false;
    }

    // Check if the current candle's body engulfs the previous candle's body
    if (current.open < previous.close && current.close > previous.open)
    {
        return true;
    }

    return false;
}

/**
 * @brief Detect the presence of a bearish engulfing candlestick pattern.
 *
 * @param candles Vector of Candle data.
 * @param index Index of the candle to analyze.
 * @return true if bearish engulfing pattern is detected, false otherwise.
 */
bool CandlePattern::isBearishEngulfing(const std::vector<Candle> &candles, size_t index) const
{
    if (index < 1 || index >= candles.size() - 1)
    {
        return false;
    }

    // Current candle
    const Candle &current = candles[index];
    // Previous candle
    const Candle &previous = candles[index - 1];

    // Check if the current candle is bearish (close < open)
    if (current.close >= current.open)
    {
        return false;
    }

    // Check if the previous candle is bullish (close > open)
    if (previous.close >= previous.open)
    {
        return false;
    }

    // Check if the current candle's body engulfs the previous candle's body
    if (current.open > previous.close && current.close < previous.open)
    {
        return true;
    }

    return false;
}

/**
 * @brief Detect the presence of a morning star candlestick pattern.
 *
 * @param candles Vector of Candle data.
 * @param index Index of the candle to analyze.
 * @return true if morning star pattern is detected, false otherwise.
 */
bool CandlePattern::isMorningStar(const std::vector<Candle> &candles, size_t index) const
{
    if (index < 2 || index >= candles.size() - 1)
    {
        return false;
    }

    // Current candle
    const Candle &current = candles[index];
    // Previous candle
    const Candle &previous = candles[index - 1];
    // Second previous candle
    const Candle &secondPrevious = candles[index - 2];

    // Check if the current candle is bullish
    if (current.close <= current.open)
    {
        return false;
    }

    // Check if the previous candle is bearish
    if (previous.close >= previous.open)
    {
        return false;
    }

    // Check if the second previous candle is bearish
    if (secondPrevious.close >= secondPrevious.open)
    {
        return false;
    }

    // Check if the current candle gaps up from the previous candle
    if (current.open > previous.close)
    {
        // Check if the second previous candle is a doji or a small-bodied candle
        if (secondPrevious.open < secondPrevious.close && abs(secondPrevious.open - secondPrevious.close) < (secondPrevious.high - secondPrevious.low) * 0.5)
        {
            return true; // Morning star pattern detected
        }
    }

    return false; // No morning star pattern found
}

/**
 * @brief Detect the presence of an evening star candlestick pattern.
 *
 * @param candles Vector of Candle data.
 * @param index Index of the candle to analyze.
 * @return true if evening star pattern is detected, false otherwise.
 */
bool CandlePattern::isEveningStar(const std::vector<Candle> &candles, size_t index) const
{
    if (index < 2 || index >= candles.size() - 1)
    {
        return false;
    }

    // Current candle
    const Candle &current = candles[index];
    // Previous candle
    const Candle &previous = candles[index - 1];
    // Second previous candle
    const Candle &secondPrevious = candles[index - 2];

    // Check if the current candle is bearish
    if (current.close >= current.open)
    {
        return false;
    }

    // Check if the previous candle is bullish
    if (previous.close <= previous.open)
    {
        return false;
    }

    // Check if the second previous candle is bullish
    if (secondPrevious.close <= secondPrevious.open)
    {
        return false;
    }

    // Check if the current candle gaps down from the previous candle
    if (current.open < previous.close)
    {
        // Check if the second previous candle is a doji or a small-bodied candle
        if (secondPrevious.open > secondPrevious.close && abs(secondPrevious.open - secondPrevious.close) < (secondPrevious.high - secondPrevious.low) * 0.5)
        {
            return true; // Evening star pattern detected
        }
    }

    return false; // No evening star pattern found
}

/**
 * @brief Detect the presence of a doji candlestick pattern.
 *
 * @param candles Vector of Candle data.
 * @param index Index of the candle to analyze.
 * @return true if doji pattern is detected, false otherwise.
 */
bool CandlePattern::isDoji(const std::vector<Candle> &candles, size_t index) const
{
    if (index >= candles.size())
    {
        return false;
    }

    const Candle &current = candles[index];

    // Check if the candle has a small body
    if (abs(current.open - current.close) < (current.high - current.low) * 0.1)
    {
        return true; // Doji pattern detected
    }

    return false; // No doji pattern found
}

/**
 * @brief Detect the presence of a spinning top candlestick pattern.
 *
 * @param candles Vector of Candle data.
 * @param index Index of the candle to analyze.
 * @return true if spinning top pattern is detected, false otherwise.
 */
bool CandlePattern::isSpinningTop(const std::vector<Candle> &candles, size_t index) const
{
    if (index >= candles.size())
    {
        return false;
    }

    const Candle &current = candles[index];

    // Calculate the size of the body and the range of the candle
    double body_size = abs(current.open - current.close);
    double candle_range = current.high - current.low;

    // Check if the body size is small relative to the candle's range
    if (body_size < candle_range * 0.3)
    {
        return true; // Spinning top pattern detected
    }

    return false; // No spinning top pattern found
}

/**
 * @brief Detect the presence of a marubozu candlestick pattern.
 *
 * @param candles Vector of Candle data.
 * @param index Index of the candle to analyze.
 * @return true if marubozu pattern is detected, false otherwise.
 */
bool CandlePattern::isMarubozu(const std::vector<Candle> &candles, size_t index) const
{
    if (index >= candles.size())
    {
        return false;
    }

    const Candle &current = candles[index];

    // Check if the candle has no upper and lower wicks
    if (current.high == current.close && current.low == current.open)
    {
        return true; // Marubozu pattern detected
    }

    return false; // No marubozu pattern found
}

/**
 * @brief Detect the presence of a harami candlestick pattern.
 *
 * @param candles Vector of Candle data.
 * @param index Index of the candle to analyze.
 * @return true if harami pattern is detected, false otherwise.
 */
bool CandlePattern::isHarami(const std::vector<Candle> &candles, size_t index) const
{
    if (index >= candles.size() || index < 1)
    {
        return false;
    }

    const Candle &current = candles[index];
    const Candle &previous = candles[index - 1];

    // Check if the previous candle is bullish
    bool bullishPrevious = previous.close > previous.open;

    // Check if the current candle is bearish
    bool bearishCurrent = current.close < current.open;

    // Check if the previous candle's body engulfs the current candle's body
    bool haramiPattern = previous.open > current.close && previous.close < current.open;

    return bullishPrevious && bearishCurrent && haramiPattern;
}

/**
 * @brief Detect the presence of a harami cross candlestick pattern.
 *
 * @param candles Vector of Candle data.
 * @param index Index of the candle to analyze.
 * @return true if harami cross pattern is detected, false otherwise.
 */
bool CandlePattern::isHaramiCross(const std::vector<Candle> &candles, size_t index) const
{
    if (index >= candles.size() || index < 1)
    {
        return false;
    }

    const Candle &current = candles[index];
    const Candle &previous = candles[index - 1];

    // Check if the previous candle is bullish
    bool bullishPrevious = previous.close > previous.open;

    // Check if the current candle is bearish
    bool bearishCurrent = current.close < current.open;

    // Check if the previous candle's body engulfs the current candle's body
    bool haramiPattern = previous.open > current.close && previous.close < current.open;

    // Check if the previous candle's body is small compared to its range
    double previousBodySize = std::abs(previous.open - previous.close);
    double previousRange = previous.high - previous.low;
    bool smallBody = previousBodySize <= 0.5 * previousRange;

    // Check if the current candle is a doji
    bool dojiCurrent = std::abs(current.open - current.close) <= 0.1 * (current.high - current.low);

    return bullishPrevious && bearishCurrent && haramiPattern && smallBody && dojiCurrent;
}

/**
 * @brief Detect the presence of a three white soldiers candlestick pattern.
 *
 * @param candles Vector of Candle data.
 * @param index Index of the candle to analyze.
 * @return true if three white soldiers pattern is detected, false otherwise.
 */
bool CandlePattern::isThreeWhiteSoldiers(const std::vector<Candle> &candles, size_t index) const
{
    if (index >= candles.size() || index < 2)
    {
        return false;
    }

    const Candle &candle1 = candles[index - 2];
    const Candle &candle2 = candles[index - 1];
    const Candle &candle3 = candles[index];

    // Check if the candles are bullish
    bool bullish1 = candle1.close > candle1.open;
    bool bullish2 = candle2.close > candle2.open;
    bool bullish3 = candle3.close > candle3.open;

    // Check if each candle's close is higher than the previous one's close
    bool higherClose1 = candle1.close < candle2.close;
    bool higherClose2 = candle2.close < candle3.close;

    return bullish1 && bullish2 && bullish3 && higherClose1 && higherClose2;
}

/**
 * @brief Detect the presence of a three black crows candlestick pattern.
 *
 * @param candles Vector of Candle data.
 * @param index Index of the candle to analyze.
 * @return true if three black crows pattern is detected, false otherwise.
 */
bool CandlePattern::isThreeBlackCrows(const std::vector<Candle> &candles, size_t index) const
{
    if (index >= candles.size() || index < 2)
    {
        return false;
    }

    const Candle &candle1 = candles[index - 2];
    const Candle &candle2 = candles[index - 1];
    const Candle &candle3 = candles[index];

    // Check if the candles are bearish
    bool bearish1 = candle1.close < candle1.open;
    bool bearish2 = candle2.close < candle2.open;
    bool bearish3 = candle3.close < candle3.open;

    // Check if each candle's close is lower than the previous one's close
    bool lowerClose1 = candle1.close > candle2.close;
    bool lowerClose2 = candle2.close > candle3.close;

    return bearish1 && bearish2 && bearish3 && lowerClose1 && lowerClose2;
}

/**
 * @brief Detect the presence of a three inside up candlestick pattern.
 *
 * @param candles Vector of Candle data.
 * @param index Index of the candle to analyze.
 * @return true if three inside up pattern is detected, false otherwise.
 */
bool CandlePattern::isThreeInsideUp(const std::vector<Candle> &candles, size_t index) const
{
    if (index >= candles.size() || index < 2)
    {
        return false;
    }

    const Candle &candle1 = candles[index - 2];
    const Candle &candle2 = candles[index - 1];
    const Candle &candle3 = candles[index];

    // Check if the first two candles are bearish and the last one is bullish
    bool bearish1 = candle1.close < candle1.open;
    bool bearish2 = candle2.close < candle2.open;
    bool bullish3 = candle3.close > candle3.open;

    // Check if the third candle's body is engulfed by the second candle's body
    bool engulfed = candle3.open < candle2.close && candle3.close > candle2.open;

    return bearish1 && bearish2 && bullish3 && engulfed;
}

/**
 * @brief Detect the presence of a three inside down candlestick pattern.
 *
 * @param candles Vector of Candle data.
 * @param index Index of the candle to analyze.
 * @return true if three inside down pattern is detected, false otherwise.
 */
bool CandlePattern::isThreeInsideDown(const std::vector<Candle> &candles, size_t index) const
{
    if (index >= candles.size() || index < 2)
    {
        return false;
    }

    const Candle &candle1 = candles[index - 2];
    const Candle &candle2 = candles[index - 1];
    const Candle &candle3 = candles[index];

    // Check if the first two candles are bullish and the last one is bearish
    bool bullish1 = candle1.close > candle1.open;
    bool bullish2 = candle2.close > candle2.open;
    bool bearish3 = candle3.close < candle3.open;

    // Check if the third candle's body is engulfed by the second candle's body
    bool engulfed = candle3.open > candle2.close && candle3.close < candle2.open;

    return bullish1 && bullish2 && bearish3 && engulfed;
}

/**
 * @brief Detect the presence of a three outside up candlestick pattern.
 *
 * @param candles Vector of Candle data.
 * @param index Index of the candle to analyze.
 * @return true if three outside up pattern is detected, false otherwise.
 */
bool CandlePattern::isThreeOutsideUp(const std::vector<Candle> &candles, size_t index) const
{
    if (index >= candles.size() || index < 2)
    {
        return false;
    }

    const Candle &candle1 = candles[index - 2];
    const Candle &candle2 = candles[index - 1];
    const Candle &candle3 = candles[index];

    // Check if the last candle's body engulfs the previous two candles
    bool engulfed = candle3.open < std::min(candle1.open, candle2.open) &&
                    candle3.close > std::max(candle1.close, candle2.close);

    return engulfed;
}

/**
 * @brief Detect the presence of a three outside down candlestick pattern.
 *
 * @param candles Vector of Candle data.
 * @param index Index of the candle to analyze.
 * @return true if three outside down pattern is detected, false otherwise.
 */
bool CandlePattern::isThreeOutsideDown(const std::vector<Candle> &candles, size_t index) const
{
    if (index >= candles.size() || index < 2)
    {
        return false;
    }

    const Candle &candle1 = candles[index - 2];
    const Candle &candle2 = candles[index - 1];
    const Candle &candle3 = candles[index];

    // Check if the last candle's body engulfs the previous two candles
    bool engulfed = candle3.open > std::max(candle1.open, candle2.open) &&
                    candle3.close < std::min(candle1.close, candle2.close);

    return engulfed;
}

/**
 * @brief Detect the presence of a three stars in the south candlestick pattern.
 *
 * @param candles Vector of Candle data.
 * @param index Index of the candle to analyze.
 * @return true if three stars in the south pattern is detected, false otherwise.
 */
bool CandlePattern::isThreeStarsInTheSouth(const std::vector<Candle> &candles, size_t index) const
{
    if (index >= candles.size() || index < 2)
    {
        return false;
    }

    const Candle &candle1 = candles[index - 2];
    const Candle &candle2 = candles[index - 1];
    const Candle &candle3 = candles[index];

    // Check if the last three candles are small-bodied with decreasing closes
    bool smallBodied = candle1.close > candle1.open &&
                       candle2.close > candle2.open &&
                       candle3.close > candle3.open &&
                       candle1.close > candle2.close &&
                       candle2.close > candle3.close;

    return smallBodied;
}

/**
 * @brief Detect the presence of a three advancing white soldiers candlestick pattern.
 *
 * @param candles Vector of Candle data.
 * @param index Index of the candle to analyze.
 * @return true if three advancing white soldiers pattern is detected, false otherwise.
 */
bool CandlePattern::isThreeAdvancingWhiteSoldiers(const std::vector<Candle> &candles, size_t index) const
{
    if (index >= candles.size() || index < 2)
    {
        return false;
    }

    const Candle &candle1 = candles[index - 2];
    const Candle &candle2 = candles[index - 1];
    const Candle &candle3 = candles[index];

    // Check if the last three candles are bullish with increasing closes
    bool bullish = candle1.close < candle1.open &&
                   candle2.close < candle2.open &&
                   candle3.close < candle3.open &&
                   candle1.close < candle2.close &&
                   candle2.close < candle3.close;

    return bullish;
}

/**
 * @brief Detect the presence of a three identical crows candlestick pattern.
 *
 * @param candles Vector of Candle data.
 * @param index Index of the candle to analyze.
 * @return true if three identical crows pattern is detected, false otherwise.
 */
bool CandlePattern::isThreeIdenticalCrows(const std::vector<Candle> &candles, size_t index) const
{
    if (index >= candles.size() || index < 2)
    {
        return false;
    }

    const Candle &candle1 = candles[index - 2];
    const Candle &candle2 = candles[index - 1];
    const Candle &candle3 = candles[index];

    // Check if the last three candles are bearish with almost identical opens and closes
    bool bearish = candle1.close > candle1.open &&
                   candle2.close > candle2.open &&
                   candle3.close > candle3.open &&
                   std::abs(candle1.open - candle1.close) < 0.001 &&
                   std::abs(candle2.open - candle2.close) < 0.001 &&
                   std::abs(candle3.open - candle3.close) < 0.001;

    return bearish;
}

/**
 * @brief Detect the presence of a abandoned baby candlestick pattern.
 *
 * @param candles Vector of Candle data.
 * @param index Index of the candle to analyze.
 * @return true if abandoned baby pattern is detected, false otherwise.
 */
bool CandlePattern::isAbandonedBaby(const std::vector<Candle> &candles, size_t index) const
{
    if (index < 2 || index >= candles.size() - 1)
    {
        return false;
    }

    const Candle &candle1 = candles[index - 2];
    const Candle &candle2 = candles[index - 1];
    const Candle &candle3 = candles[index];

    // Check for a bullish gap between the first and second candlesticks
    bool bullishGap = candle2.open < candle1.low && candle2.close > candle1.high;

    // Check for a bearish gap between the second and third candlesticks
    bool bearishGap = candle3.open > candle2.high && candle3.close < candle2.low;

    // Check if the third candlestick is a doji with a gap between its high and low
    bool isDoji = std::abs(candle3.open - candle3.close) < 0.001;
    bool hasGap = candle3.low > candle2.high && candle3.high < candle2.low;

    // Check if the close of the third candlestick is near the mid-price of the first and second candlesticks
    double midPrice = (candle1.high + candle1.low) / 2.0;
    bool closeNearMidPrice = std::abs(midPrice - candle3.close) < std::abs(midPrice - candle2.close);

    return bullishGap && bearishGap && isDoji && hasGap && closeNearMidPrice;
}

/**
 * @brief Detect the presence of a dark cloud cover candlestick pattern.
 *
 * @param candles Vector of Candle data.
 * @param index Index of the candle to analyze.
 * @return true if dark cloud cover pattern is detected, false otherwise.
 */
bool CandlePattern::isDarkCloudCover(const std::vector<Candle> &candles, size_t index) const
{
    if (index < 1 || index >= candles.size() - 1)
    {
        return false;
    }

    const Candle &candle1 = candles[index - 1];
    const Candle &candle2 = candles[index];

    // Check for a bearish gap between the first and second candlesticks
    bool bearishGap = candle2.open > candle1.high && candle2.close < candle1.low;

    // Check if the second candlestick closes below the midpoint of the first candlestick's body
    double bodyMidpoint = (candle1.open + candle1.close) / 2.0;
    bool closesBelowMidpoint = candle2.close < bodyMidpoint;

    return bearishGap && closesBelowMidpoint;
}

/**
 * @brief Detect the presence of a evening doji star candlestick pattern.
 *
 * @param candles Vector of Candle data.
 * @param index Index of the candle to analyze.
 * @return true if evening doji star pattern is detected, false otherwise.
 */
bool CandlePattern::isEveningDojiStar(const std::vector<Candle> &candles, size_t index) const
{
    if (index < 2 || index >= candles.size() - 1)
    {
        return false;
    }

    const Candle &candle1 = candles[index - 2];
    const Candle &candle2 = candles[index - 1];
    const Candle &candle3 = candles[index];

    // Check if the first candlestick is bullish and the third candlestick is bearish
    bool bullishFirst = candle1.close > candle1.open;
    bool bearishThird = candle3.close < candle3.open;

    // Check if the second candlestick is a doji
    double dojiBodySize = std::abs(candle2.close - candle2.open);
    double dojiUpperShadow = candle2.high - std::max(candle2.open, candle2.close);
    double dojiLowerShadow = std::min(candle2.open, candle2.close) - candle2.low;
    bool isDoji = dojiBodySize < (0.1 * (candle2.high - candle2.low)) && dojiUpperShadow > 2 * dojiBodySize && dojiLowerShadow > 2 * dojiBodySize;

    // Check if the third candlestick closes well within the body of the first candlestick
    bool closesWithinBody = candle3.close < candle1.open && candle3.close > candle1.close;

    return bullishFirst && isDoji && bearishThird && closesWithinBody;
}

// *********************************************************************************************

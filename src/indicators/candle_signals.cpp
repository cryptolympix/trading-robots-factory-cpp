#include "../types.hpp"
#include "../utils/candles_source.hpp"
#include "indicator.hpp"
#include "candle.hpp"
#include "candle_signals.hpp"

/**
 * @brief Construct a New High Signal object.
 *
 * @param left_bars Number of left bars.
 * @param right_bars Number of right bars.
 * @param offset Offset value. Default is 0.
 */
NewHighSignal::NewHighSignal(int left_bars, int right_bars, int offset) : Indicator("New High Signal", "new-high-signal-" + std::to_string(left_bars) + "-" + std::to_string(right_bars) + "-" + std::to_string(offset), offset), left_bars(left_bars), right_bars(right_bars) {}

/**
 * @brief Calculate the New High Signal values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> NewHighSignal::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles)
        {
            std::vector<double> values(candles.size(), 0);
            std::vector<double> highest_highs = PivotHighValue(CandleSource::High, left_bars, right_bars).calculate(candles, false);
            std::vector<double> highs = get_candles_with_source(candles, CandleSource::High);

            for (size_t i = std::max(left_bars + right_bars, 1); i < candles.size() - right_bars; ++i)
            {
                // Check if the current high is the highest high in the last left_bars + right_bars
                if (highs[i] == highest_highs[i + right_bars] && highs[i] != highs[i - 1])
                {
                    values[i] = 1;
                }
            }

            return values; },

        normalize_data);
};

// *********************************************************************************************

/**
 * @brief Construct a New Low Signal object.
 *
 * @param left_bars Number of left bars.
 * @param right_bars Number of right bars.
 * @param offset Offset value. Default is 0.
 */
NewLowSignal::NewLowSignal(int left_bars, int right_bars, int offset) : Indicator("New Low Signal", "new-low-signal-" + std::to_string(left_bars) + "-" + std::to_string(right_bars) + "-" + std::to_string(offset), offset), left_bars(left_bars), right_bars(right_bars) {}

/**
 * @brief Calculate the New Low Signal values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> NewLowSignal::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles)
        {
            std::vector<double> values(candles.size(), 0);
            std::vector<double> lowest_lows = PivotLowValue(CandleSource::Low, left_bars, right_bars).calculate(candles, false);
            std::vector<double> lows = get_candles_with_source(candles, CandleSource::Low);

            for (size_t i = std::max(left_bars + right_bars, 1); i < candles.size() - right_bars; ++i)
            {
                // Check if the current low is the lowest low in the last left_bars + right_bars
                if (lows[i] == lowest_lows[i + right_bars] && lows[i] != lows[i - 1])
                {
                    values[i] = 1;
                }
            }

            return values; },

        normalize_data);
}

// *********************************************************************************************

/**
 * @brief Construct a new High Break Signal object.
 *
 * @param left_bars Number of left bars.
 * @param right_bars Number of right bars.
 * @param offset Offset value. Default is 0.
 */
HighBreakSignal::HighBreakSignal(int left_bars, int right_bars, int offset) : Indicator("High Break Signal", "high-break-signal-" + std::to_string(left_bars) + "-" + std::to_string(right_bars) + "-" + std::to_string(offset), offset), left_bars(left_bars), right_bars(right_bars) {}

/**
 * @brief Calculate the High Break Signal values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> HighBreakSignal::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles)
        {
            std::vector<double> values(candles.size(), 0);
            std::vector<double> highest_highs = PivotHighValue(CandleSource::High, left_bars, right_bars).calculate(candles, false);
            std::vector<double> closes = get_candles_with_source(candles, CandleSource::Close);

            for (size_t i = 1; i < candles.size(); ++i)
            {
                if (closes[i] > highest_highs[i - 1]) 
                {
                    values[i] = 1;
                }
            }

            return values; },

        normalize_data);
}

// *********************************************************************************************

/**
 * @brief Construct a new Low Break Signal object.
 *
 * @param left_bars Number of left bars.
 * @param right_bars Number of right bars.
 * @param offset Offset value. Default is 0.
 */
LowBreakSignal::LowBreakSignal(int left_bars, int right_bars, int offset) : Indicator("Low Break Signal", "low-break-signal-" + std::to_string(left_bars) + "-" + std::to_string(right_bars) + "-" + std::to_string(offset), offset), left_bars(left_bars), right_bars(right_bars) {}

/**
 * @brief Calculate the Low Break Signal values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> LowBreakSignal::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles)
        {
            std::vector<double> values(candles.size(), 0);
            std::vector<double> lowest_lows = PivotLowValue(CandleSource::Low, left_bars, right_bars).calculate(candles, false);
            std::vector<double> closes = get_candles_with_source(candles, CandleSource::Close);

            for (size_t i = 1; i < candles.size(); ++i)
            {
                if (closes[i] < lowest_lows[i - 1])
                {
                    values[i] = 1;
                }
            }

            return values; },

        normalize_data);
}
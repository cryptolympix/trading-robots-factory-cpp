#include <vector>
#include <cmath>
#include "../types.hpp"
#include "candle_patterns.hpp"
#include "indicator.hpp"

/**
 * @brief Construct a new DojiCandlePattern object.
 *
 * @param pattern_name Name of the pattern.
 * @param offset Offset value. Default is 0.
 */
DojiCandlePattern::DojiCandlePattern(int offset) : Indicator("Doji Candle Pattern", "doji-candle-pattern", {{"offset", offset}}, {0, 4}) {}

/**
 * @brief Calculate the DojiCandlePattern values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double>
DojiCandlePattern::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles) -> std::vector<double>
        {
            std::vector<double> values(candles.size(), 0); // Initialize values vector with size of candles

            for (size_t i = 0; i < candles.size(); ++i)
            {
                if (isClassicDoji(candles, i))
                {
                    values[i] = 1;
                }
                else if (isLongLeggedDoji(candles, i))
                {
                    values[i] = 2;
                }
                else if (isDragonflyDoji(candles, i))
                {
                    values[i] = 3;
                }
                else if (isGravestoneDoji(candles, i))
                {
                    values[i] = 4;
                }
            }

            return values; },

        normalize_data);
}

/**
 * @brief Detect the presence of a doji candlestick pattern.
 *
 * @param candles Vector of Candle data.
 * @param index Index of the candle to analyze.
 * @return true if doji pattern is detected, false otherwise.
 */
bool DojiCandlePattern::isClassicDoji(const std::vector<Candle> &candles, size_t index) const
{
    if (index >= candles.size())
    {
        return false;
    }

    const Candle &current = candles[index];

    // Check if the candle is a classic doji
    double candleRange = current.high - current.low;
    double dojiBodySize = std::abs(current.close - current.open);
    double upperShadow = current.high - std::max(current.open, current.close);
    double lowerShadow = std::min(current.open, current.close) - current.low;

    return upperShadow <= 2 * dojiBodySize && lowerShadow <= 2 * dojiBodySize;
}

/**
 * @brief Detect the presence of a long legged doji candlestick pattern.
 *
 * @param candles Vector of Candle data.
 * @param index Index of the candle to analyze.
 * @return true if long legged doji pattern is detected, false otherwise.
 */
bool DojiCandlePattern::isLongLeggedDoji(const std::vector<Candle> &candles, size_t index) const
{
    if (index >= candles.size())
    {
        return false;
    }

    const Candle &current = candles[index];

    // Check if the candle is a Long Legged Doji
    double candleRange = current.high - current.low;
    double dojiBodySize = std::abs(current.close - current.open);
    double upperShadow = current.high - std::max(current.open, current.close);
    double lowerShadow = std::min(current.open, current.close) - current.low;

    return upperShadow > 2 * dojiBodySize && lowerShadow > 2 * dojiBodySize;
}

/**
 * @brief Detect the presence of a dragonfly doji candlestick pattern.
 *
 * @param candles Vector of Candle data.
 * @param index Index of the candle to analyze.
 * @return true if dragonfly doji pattern is detected, false otherwise.
 */
bool DojiCandlePattern::isDragonflyDoji(const std::vector<Candle> &candles, size_t index) const
{
    if (index >= candles.size())
    {
        return false;
    }

    const Candle &current = candles[index];

    // Check if the candle is a Dragonfly Doji
    double candleRange = current.high - current.low;
    double dojiBodySize = std::abs(current.close - current.open);
    double upperShadow = current.high - std::max(current.open, current.close);
    double lowerShadow = std::min(current.open, current.close) - current.low;

    return dojiBodySize < 0.1 * candleRange && upperShadow < 0.1 * candleRange && lowerShadow > 2 * dojiBodySize;
}

/**
 * @brief Detect the presence of a gravestone doji candlestick pattern.
 *
 * @param candles Vector of Candle data.
 * @param index Index of the candle to analyze.
 * @return true if gravestone doji pattern is detected, false otherwise.
 */
bool DojiCandlePattern::isGravestoneDoji(const std::vector<Candle> &candles, size_t index) const
{
    if (index >= candles.size())
    {
        return false;
    }

    const Candle &current = candles[index];

    // Check if the candle is a Gravestone Doji
    double candleRange = current.high - current.low;
    double dojiBodySize = std::abs(current.close - current.open);
    double upperShadow = current.high - std::max(current.open, current.close);
    double lowerShadow = std::min(current.open, current.close) - current.low;

    return dojiBodySize < 0.1 * candleRange && upperShadow > 2 * dojiBodySize && lowerShadow < 0.1 * candleRange;
}

// *********************************************************************************************

/**
 * @brief Construct a new SingleCandlePattern object.
 *
 * @param offset Offset value. Default is 0.
 */
SingleCandlePattern::SingleCandlePattern(int offset) : Indicator("Single Candle Pattern", "single-candle-pattern", {{"offset", offset}}, {-4, 4}) {}

/**
 * @brief Calculate the SingleCandlePattern values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> SingleCandlePattern::calculate(const std::vector<Candle> &candles, bool normalize_data) const
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
                else if (isInvertedHammer(candles, i))
                {
                    values[i] = 2;
                }
                else if (isShootingStar(candles, i))
                {
                    values[i] = -1;
                }
                else if (isHangingMan(candles, i))
                {
                    values[i] = -2;
                }
                else if (isBullishSpinningTop(candles, i))
                {
                    values[i] = 3;
                }
                else if (isBearishSpinningTop(candles, i))
                {
                    values[i] = -3;
                }
                else if (isBullishMarubozu(candles, i))
                {
                    values[i] = 4;
                }
                else if (isBearishMarubozu(candles, i))
                {
                    values[i] = -4;
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
bool SingleCandlePattern::isHammer(const std::vector<Candle> &candles, size_t index) const
{
    if (index >= candles.size())
    {
        return false;
    }

    const Candle &current = candles[index];

    if (current.close <= current.open)
    {
        return false;
    }

    double candleRange = current.high - current.low;
    double dojiBodySize = std::abs(current.close - current.open);
    double upperShadow = current.high - std::max(current.open, current.close);
    double lowerShadow = std::min(current.open, current.close) - current.low;

    // Check if the candle is a hammer
    bool bodyShort = dojiBodySize < candleRange * 0.25;
    bool lowerShadowLong = lowerShadow > dojiBodySize * 2;
    bool upperShadowShort = upperShadow < dojiBodySize * 0.5;

    return bodyShort && lowerShadowLong && upperShadowShort;
}

/**
 * @brief Detect the presence of an inverted hammer candlestick pattern.
 *
 * @param candles Vector of Candle data.
 * @param index Index of the candle to analyze.
 * @return true if inverted hammer pattern is detected, false otherwise.
 */
bool SingleCandlePattern::isInvertedHammer(const std::vector<Candle> &candles, size_t index) const
{
    if (index >= candles.size())
    {
        return false;
    }

    const Candle &current = candles[index];

    if (current.close <= current.open)
    {
        return false;
    }

    double candleRange = current.high - current.low;
    double dojiBodySize = std::abs(current.close - current.open);
    double upperShadow = current.high - std::max(current.open, current.close);
    double lowerShadow = std::min(current.open, current.close) - current.low;

    // Check if the candle is an inverted hammer
    bool bodyShort = dojiBodySize < candleRange * 0.25;
    bool upperShadowLong = upperShadow > dojiBodySize * 2;
    bool lowerShadowShort = lowerShadow < dojiBodySize * 0.5;

    return bodyShort && upperShadowLong && lowerShadowShort;
}

/**
 * @brief Detect the presence of a shooting star candlestick pattern.
 *
 * @param candles Vector of Candle data.
 * @param index Index of the candle to analyze.
 * @return true if shooting star pattern is detected, false otherwise.
 */
bool SingleCandlePattern::isShootingStar(const std::vector<Candle> &candles, size_t index) const
{
    if (index >= candles.size())
    {
        return false;
    }

    const Candle &current = candles[index];

    if (current.close > current.open)
    {
        return false;
    }

    double candleRange = current.high - current.low;
    double dojiBodySize = std::abs(current.close - current.open);
    double upperShadow = current.high - std::max(current.open, current.close);
    double lowerShadow = std::min(current.open, current.close) - current.low;

    // Check if the candle is a shooting star
    bool bodyShort = dojiBodySize < candleRange * 0.25;
    bool upperShadowLong = upperShadow > dojiBodySize * 2;
    bool lowerShadowShort = lowerShadow < dojiBodySize * 0.5;

    return bodyShort && upperShadowLong && lowerShadowShort;
}

/**
 * @brief Detect the presence of a hanging man candlestick pattern.
 *
 * @param candles Vector of Candle data.
 * @param index Index of the candle to analyze.
 * @return true if hanging man pattern is detected, false otherwise.
 */
bool SingleCandlePattern::isHangingMan(const std::vector<Candle> &candles, size_t index) const
{
    if (index >= candles.size())
    {
        return false;
    }

    const Candle &current = candles[index];

    if (current.close > current.open)
    {
        return false;
    }

    double candleRange = current.high - current.low;
    double dojiBodySize = std::abs(current.close - current.open);
    double upperShadow = current.high - std::max(current.open, current.close);
    double lowerShadow = std::min(current.open, current.close) - current.low;

    // Check if the candle is a hanging man
    bool bodyShort = dojiBodySize < candleRange * 0.25;
    bool upperShadowShort = upperShadow < dojiBodySize * 0.5;
    bool lowerShadowLong = lowerShadow > dojiBodySize * 2;

    return bodyShort && upperShadowShort && lowerShadowLong;
}

/**
 * @brief Detect the presence of a spinning top candlestick pattern.
 *
 * @param candles Vector of Candle data.
 * @param index Index of the candle to analyze.
 * @return true if spinning top pattern is detected, false otherwise.
 */
bool SingleCandlePattern::isBullishSpinningTop(const std::vector<Candle> &candles, size_t index) const
{
    if (index >= candles.size())
    {
        return false;
    }

    const Candle &current = candles[index];

    if (current.close <= current.open)
    {
        return false;
    }

    // Calculate the size of the body and the range of the candle
    double cande_range = current.high - current.low;
    double body_size = std::abs(current.open - current.close);
    double upper_shadow_size = current.high - std::max(current.open, current.close);
    double lower_shadow_size = std::min(current.open, current.close) - current.low;

    // Check if the body size is small relative to the candle's range and if it's a spinning top
    return body_size < upper_shadow_size && body_size < lower_shadow_size && body_size < 0.3 * cande_range;
}

/**
 * @brief Detect the presence of a bearish spinning top candlestick pattern.
 *
 * @param candles Vector of Candle data.
 * @param index Index of the candle to analyze.
 * @return true if bearish spinning top pattern is detected, false otherwise.
 */
bool SingleCandlePattern::isBearishSpinningTop(const std::vector<Candle> &candles, size_t index) const
{
    if (index >= candles.size())
    {
        return false;
    }

    const Candle &current = candles[index];

    if (current.close >= current.open)
    {
        return false;
    }

    // Calculate the size of the body and the range of the candle
    double cande_range = current.high - current.low;
    double body_size = std::abs(current.open - current.close);
    double upper_shadow_size = current.high - std::max(current.open, current.close);
    double lower_shadow_size = std::min(current.open, current.close) - current.low;

    // Check if the body size is small relative to the candle's range and if it's a spinning top
    return body_size < upper_shadow_size && body_size < lower_shadow_size && body_size < 0.3 * cande_range;
}

/**
 * @brief Detect the presence of a bullish Marubozu candlestick pattern.
 *
 * @param candles Vector of Candle data.
 * @param index Index of the candle to analyze.
 * @return true if bullish Marubozu pattern is detected, false otherwise.
 */
bool SingleCandlePattern::isBullishMarubozu(const std::vector<Candle> &candles, size_t index) const
{
    if (index >= candles.size())
    {
        return false;
    }

    const Candle &current = candles[index];

    if (current.close <= current.open)
    {
        return false;
    }

    // Check if the candle has no upper wick and a lower wick that is less than 10% of the candle's range
    double candle_range = current.high - current.low;
    double body_size = std::abs(current.open - current.close);
    double lower_shadow = std::min(current.open, current.close) - current.low;
    return lower_shadow <= 0.1 * candle_range && body_size >= 0.8 * candle_range;
}

/**
 * @brief Detect the presence of a bearish Marubozu candlestick pattern.
 *
 * @param candles Vector of Candle data.
 * @param index Index of the candle to analyze.
 * @return true if bearish Marubozu pattern is detected, false otherwise.
 */
bool SingleCandlePattern::isBearishMarubozu(const std::vector<Candle> &candles, size_t index) const
{
    if (index >= candles.size())
    {
        return false;
    }

    const Candle &current = candles[index];

    if (current.close >= current.open)
    {
        return false;
    }

    // Check if the candle has no lower wick and an upper wick that is less than 10% of the candle's range
    double candle_range = current.high - current.low;
    double body_size = std::abs(current.open - current.close);
    double upper_shadow = current.high - std::max(current.open, current.close);
    return upper_shadow <= 0.1 * candle_range && body_size >= 0.8 * candle_range;
}

// *********************************************************************************************

/**
 * @brief Construct a new DoubleCandlePattern object.
 *
 * @param offset Offset value. Default is 0.
 */
DoubleCandlePattern::DoubleCandlePattern(int offset) : Indicator("Double Candle Pattern", "double-candle-pattern", {{"offset", offset}}, {-2, 2}) {}

/**
 * @brief Calculate the DoubleCandlePattern values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> DoubleCandlePattern::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles) -> std::vector<double>
        {
            std::vector<double> values(candles.size(), 0); // Initialize values vector with size of candles

            for (size_t i = 0; i < candles.size(); ++i)
            {
                if (isBullishEngulfing(candles, i))
                {
                    values[i] = 1;
                }
                else if (isBearishEngulfing(candles, i))
                {
                    values[i] = -1;
                }
                else if (isPiercingLine(candles, i))
                {
                    values[i] = 2;
                }
                else if (isDarkCloudCover(candles, i))
                {
                    values[i] = -2;
                }
            }

            return values; },

        normalize_data);
}

/**
 * @brief Detect the presence of a bullish engulfing candlestick pattern.
 *
 * @param candles Vector of Candle data.
 * @param index Index of the candle to analyze.
 * @return true if bullish engulfing pattern is detected, false otherwise.
 */
bool DoubleCandlePattern::isBullishEngulfing(const std::vector<Candle> &candles, size_t index) const
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
bool DoubleCandlePattern::isBearishEngulfing(const std::vector<Candle> &candles, size_t index) const
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
    if (previous.close <= previous.open)
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
 * @brief Detect the presence of a piercing line candlestick pattern.
 *
 * @param candles Vector of Candle data.
 * @param index Index of the candle to analyze.
 * @return true if piercing line pattern is detected, false otherwise.
 */
bool DoubleCandlePattern::isPiercingLine(const std::vector<Candle> &candles, size_t index) const
{
    if (index < 1 || index >= candles.size())
    {
        return false;
    }

    const Candle &previous = candles[index - 1];
    const Candle &current = candles[index];

    // Check if the previous candle is bearish and the current candle is bullish
    if (previous.close < previous.open && current.close > current.open)
    {
        // Calculate the midpoint of the previous candle
        double previousMidpoint = (previous.open + previous.close) / 2.0;

        // Check if the current candle opens below the midpoint of the previous candle
        if (current.open < previousMidpoint)
        {
            // Calculate the penetration of the previous candle's body by the current candle
            double penetration = (current.close - previous.low) / (previous.high - previous.low);

            // Check if the current candle closes above the midpoint of the previous candle
            return current.close > previousMidpoint && penetration > 0.5;
        }
    }
    return false;
}

/**
 * @brief Detect the presence of a dark cloud cover candlestick pattern.
 *
 * @param candles Vector of Candle data.
 * @param index Index of the candle to analyze.
 * @return true if dark cloud cover pattern is detected, false otherwise.
 */
bool DoubleCandlePattern::isDarkCloudCover(const std::vector<Candle> &candles, size_t index) const
{
    if (index < 1 || index >= candles.size())
    {
        return false;
    }

    const Candle &previous = candles[index - 1];
    const Candle &current = candles[index];

    // Check if the previous candle is bullish and the current candle is bearish
    if (previous.close > previous.open && current.close < current.open)
    {
        // Calculate the midpoint of the previous candle
        double previousMidpoint = (previous.open + previous.close) / 2.0;

        // Check if the current candle opens above the midpoint of the previous candle
        if (current.open > previousMidpoint)
        {
            // Calculate the penetration of the previous candle's body by the current candle
            double penetration = (current.open - previous.low) / (previous.high - previous.low);

            // Check if the current candle closes below the midpoint of the previous candle
            return current.close < previousMidpoint && penetration > 0.5;
        }
    }
    return false;
}

// *********************************************************************************************

/**
 * @brief Construct a new TripleCandlePattern object.
 *
 * @param offset Offset value. Default is 0.
 */
TripleCandlePattern::TripleCandlePattern(int offset) : Indicator("Triple Candle Pattern", "triple-candle-pattern", {{"offset", offset}}, {-6, 6}) {}

/**
 * @brief Calculate the TripleCandlePattern values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> TripleCandlePattern::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles) -> std::vector<double>
        {
            std::vector<double> values(candles.size(), 0); // Initialize values vector with size of candles

            for (size_t i = 0; i < candles.size(); ++i)
            {
                if (isThreeWhiteSoldiers(candles, i))
                {
                    values[i] = 1;
                }
                else if (isThreeBlackCrows(candles, i))
                {
                    values[i] = -1;
                }
                else if (isMorningStar(candles, i))
                {
                    values[i] = 2;
                }
                else if (isEveningStar(candles, i))
                {
                    values[i] = -2;
                }
                else if (isMorningDojiStar(candles, i))
                {
                    values[i] = 3;
                }
                else if (isEveningDojiStar(candles, i))
                {
                    values[i] = -3;
                }
                else if (isThreeInsideUp(candles, i))
                {
                    values[i] = 4;
                }
                else if (isThreeInsideDown(candles, i))
                {
                    values[i] = -4;
                }
                else if (isThreeOutsideUp(candles, i))
                {
                    values[i] = 5;
                }
                else if (isThreeOutsideDown(candles, i))
                {
                    values[i] = -5;
                }
                else if (isThreeStarsInTheSouth(candles, i))
                {
                    values[i] = 6;
                }
                else if (isThreeStarsInTheNorth(candles, i))
                {
                    values[i] = -6;
                }
            }

            return values; },

        normalize_data);
}

/**
 * @brief Detect the presence of a three white soldiers candlestick pattern.
 *
 * @param candles Vector of Candle data.
 * @param index Index of the candle to analyze.
 * @return true if three white soldiers pattern is detected, false otherwise.
 */
bool TripleCandlePattern::isThreeWhiteSoldiers(const std::vector<Candle> &candles, size_t index) const
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
bool TripleCandlePattern::isThreeBlackCrows(const std::vector<Candle> &candles, size_t index) const
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
 * @brief Detect the presence of a morning star candlestick pattern.
 *
 * @param candles Vector of Candle data.
 * @param index Index of the candle to analyze.
 * @return true if morning star pattern is detected, false otherwise.
 */
bool TripleCandlePattern::isMorningStar(const std::vector<Candle> &candles, size_t index) const
{
    if (index < 2 || index >= candles.size())
    {
        return false;
    }

    const Candle &first = candles[index - 2];
    const Candle &second = candles[index - 1];
    const Candle &third = candles[index];

    // Check conditions for the morning star pattern
    bool firstRed = first.close < first.open;                               // First candle is bearish
    bool secondSmall = second.close > second.open;                          // Second candle is bullish and small
    bool thirdGreen = third.close > third.open;                             // Third candle is bullish
    bool gapUp = second.open < first.close;                                 // Gap up between first and second candle
    bool engulfed = third.open < second.close && third.close > second.open; // Third candle engulfs the second

    return firstRed && secondSmall && thirdGreen && gapUp && engulfed;
}

/**
 * @brief Detect the presence of an evening star candlestick pattern.
 *
 * @param candles Vector of Candle data.
 * @param index Index of the candle to analyze.
 * @return true if evening star pattern is detected, false otherwise.
 */
bool TripleCandlePattern::isEveningStar(const std::vector<Candle> &candles, size_t index) const
{
    if (index < 2 || index >= candles.size())
    {
        return false;
    }

    const Candle &first = candles[index - 2];
    const Candle &second = candles[index - 1];
    const Candle &third = candles[index];

    // Check conditions for the evening star pattern
    bool firstGreen = first.close > first.open;                             // First candle is bullish
    bool secondSmall = second.close < second.open;                          // Second candle is bearish and small
    bool thirdRed = third.close < third.open;                               // Third candle is bearish
    bool gapDown = second.open > first.close;                               // Gap down between first and second candle
    bool engulfed = third.open > second.close && third.close < second.open; // Third candle engulfs the second

    return firstGreen && secondSmall && thirdRed && gapDown && engulfed;
}

/**
 * @brief Detect the presence of a morning doji star candlestick pattern.
 *
 * @param candles Vector of Candle data.
 * @param index Index of the candle to analyze.
 * @return true if morning doji star pattern is detected, false otherwise.
 */
bool TripleCandlePattern::isMorningDojiStar(const std::vector<Candle> &candles, size_t index) const
{
    if (index < 2 || index >= candles.size())
    {
        return false;
    }

    const Candle &candle1 = candles[index - 2];
    const Candle &candle2 = candles[index - 1];
    const Candle &candle3 = candles[index];

    // Check if the first candlestick is bearish and the third candlestick is bullish
    bool bearishFirst = candle1.close < candle1.open;
    bool bullishThird = candle3.close > candle3.open;

    // Check if the second candlestick is a doji
    double dojiBodySize = std::abs(candle2.close - candle2.open);
    double dojiUpperShadow = candle2.high - std::max(candle2.open, candle2.close);
    double dojiLowerShadow = std::min(candle2.open, candle2.close) - candle2.low;
    bool isDoji = dojiBodySize < (0.1 * (candle2.high - candle2.low)) && dojiUpperShadow > 2 * dojiBodySize && dojiLowerShadow > 2 * dojiBodySize;

    // Check if the third candlestick closes well within the body of the first candlestick
    bool closesWithinBody = candle3.close < candle1.open && candle3.close > candle1.close;

    return bearishFirst && isDoji && bullishThird && closesWithinBody;
}

/**
 * @brief Detect the presence of a evening doji star candlestick pattern.
 *
 * @param candles Vector of Candle data.
 * @param index Index of the candle to analyze.
 * @return true if evening doji star pattern is detected, false otherwise.
 */
bool TripleCandlePattern::isEveningDojiStar(const std::vector<Candle> &candles, size_t index) const
{
    if (index < 2 || index >= candles.size())
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
    bool closesWithinBody = candle3.close > candle1.open && candle3.close < candle1.close;

    return bullishFirst && isDoji && bearishThird && closesWithinBody;
}

/**
 * @brief Detect the presence of a three inside up candlestick pattern.
 *
 * @param candles Vector of Candle data.
 * @param index Index of the candle to analyze.
 * @return true if three inside up pattern is detected, false otherwise.
 */
bool TripleCandlePattern::isThreeInsideUp(const std::vector<Candle> &candles, size_t index) const
{
    if (index >= candles.size() || index < 2)
    {
        return false;
    }

    const Candle &first = candles[index - 2];
    const Candle &second = candles[index - 1];
    const Candle &third = candles[index];

    // Check if the first candle is black (down) with a large real body
    bool blackCandle = first.close < first.open;
    bool largeRealBody1 = abs(first.close - first.open) > 0.5 * (first.high - first.low);

    // Check if the second candle is white (up) with a small real body that opens and closes within the real body of the first candle
    bool whiteCandle = second.close > second.open;
    bool smallRealBody2 = abs(second.close - second.open) < 0.5 * (second.high - second.low);
    bool withinRealBody = second.open >= first.close && second.close <= first.open;

    // Check if the third candle is white (up) and closes above the close of the second candle and the first candle
    bool whiteCandle2 = third.close > third.open;
    bool closesAboveSecond = third.close > second.close;
    bool closesAboveFirst = third.close > first.open;

    return blackCandle && largeRealBody1 && whiteCandle && smallRealBody2 && withinRealBody && whiteCandle2 && closesAboveSecond && closesAboveFirst;
}

/**
 * @brief Detect the presence of a three inside down candlestick pattern.
 *
 * @param candles Vector of Candle data.
 * @param index Index of the candle to analyze.
 * @return true if three inside down pattern is detected, false otherwise.
 */
bool TripleCandlePattern::isThreeInsideDown(const std::vector<Candle> &candles, size_t index) const
{
    if (index >= candles.size() || index < 2)
    {
        return false;
    }

    const Candle &first = candles[index - 2];
    const Candle &second = candles[index - 1];
    const Candle &third = candles[index];

    // Check if the first candle is white (up) with a large real body
    bool whiteCandle = first.close > first.open;
    bool largeRealBody1 = abs(first.close - first.open) > 0.5 * (first.high - first.low);

    // Check if the second candle is black (down) with a small real body that opens and closes within the real body of the first candle
    bool blackCandle = second.close < second.open;
    bool smallRealBody2 = abs(second.close - second.open) < 0.5 * (second.high - second.low);
    bool withinRealBody = second.open <= first.close && second.close >= first.open;

    // Check if the third candle is black (down) and closes below the close of the second candle and the first candle
    bool blackCandle2 = third.close < third.open;
    bool closesBelowSecond = third.close < second.close;
    bool closesBelowFirst = third.close < first.open;

    return whiteCandle && largeRealBody1 && blackCandle && smallRealBody2 && withinRealBody && blackCandle2 && closesBelowSecond;
}

/**
 * @brief Detect the presence of a three outside up candlestick pattern.
 *
 * @param candles Vector of Candle data.
 * @param index Index of the candle to analyze.
 * @return true if three outside up pattern is detected, false otherwise.
 */
bool TripleCandlePattern::isThreeOutsideUp(const std::vector<Candle> &candles, size_t index) const
{
    if (index >= candles.size() || index < 2)
    {
        return false;
    }

    const Candle &first = candles[index - 2];
    const Candle &second = candles[index - 1];
    const Candle &third = candles[index];

    // Check if the first candle is black (down) with a large real body
    bool blackCandle = first.close < first.open;
    bool largeRealBody1 = abs(first.close - first.open) > 0.5 * (first.high - first.low);

    // Check if the second candle is white (up) with a small real body that opens and closes without the real body of the first candle
    bool whiteCandle = second.close > second.open;
    bool smallRealBody2 = abs(second.close - second.open) < 0.5 * (second.high - second.low);
    bool withoutRealBody = second.open < first.close && second.close <= first.open;

    // Check if the third candle is white (up) and closes above the close of the second candle and the first candle
    bool whiteCandle2 = third.close > third.open;
    bool closesAboveSecond = third.close > second.close;
    bool closesAboveFirst = third.close > first.open;

    return blackCandle && largeRealBody1 && whiteCandle && smallRealBody2 && withoutRealBody && whiteCandle2 && closesAboveSecond && closesAboveFirst;
}

/**
 * @brief Detect the presence of a three outside down candlestick pattern.
 *
 * @param candles Vector of Candle data.
 * @param index Index of the candle to analyze.
 * @return true if three outside down pattern is detected, false otherwise.
 */
bool TripleCandlePattern::isThreeOutsideDown(const std::vector<Candle> &candles, size_t index) const
{
    if (index >= candles.size() || index < 2)
    {
        return false;
    }

    const Candle &first = candles[index - 2];
    const Candle &second = candles[index - 1];
    const Candle &third = candles[index];

    // Check if the first candle is white (up) with a large real body
    bool whiteCandle = first.close > first.open;
    bool largeRealBody1 = abs(first.close - first.open) > 0.5 * (first.high - first.low);

    // Check if the second candle is black (down) with a small real body that opens and closes within the real body of the first candle
    bool blackCandle = second.close < second.open;
    bool smallRealBody2 = abs(second.close - second.open) < 0.5 * (second.high - second.low);
    bool withoutRealBody = second.open > first.close && second.close >= first.open;

    // Check if the third candle is black (down) and closes below the close of the second candle and the first candle
    bool blackCandle2 = third.close < third.open;
    bool closesBelowSecond = third.close < second.close;
    bool closesBelowFirst = third.close < first.open;

    return whiteCandle && largeRealBody1 && blackCandle && smallRealBody2 && withoutRealBody && blackCandle2 && closesBelowSecond;
}

/**
 * @brief Detect the presence of a Three Stars in the South candlestick pattern.
 *
 * @param candles Vector of Candle data.
 * @param index Index of the candle to analyze.
 * @return true if Three Stars in the South pattern is detected, false otherwise.
 */
bool TripleCandlePattern::isThreeStarsInTheSouth(const std::vector<Candle> &candles, size_t index) const
{
    if (index < 2 || index >= candles.size())
    {
        return false;
    }

    const Candle &first = candles[index - 2];
    const Candle &second = candles[index - 1];
    const Candle &third = candles[index];

    // Check if the pattern occurs during a downtrend
    if (first.close > first.open || second.close > second.open || third.close > third.open)
    {
        return false;
    }

    // Check if the first candle is long and black with a long lower shadow and no upper shadow
    bool firstCondition = (first.high - first.low) >= 3 * (first.open - first.close) && // Long body
                          (first.open >= first.close) &&                                // Black candle
                          (first.close - first.low) >= 2 * (first.high - first.open) && // Long lower shadow
                          (first.high == first.open || first.high == first.close);      // No upper shadow

    // Check if the second candle is black and shorter than the first candle, with its low above the low of the first candle
    bool secondCondition = (second.open > second.close) &&                            // Black candle
                           ((second.high - second.low) < (first.high - first.low)) && // Shorter than first candle
                           (second.low > first.low);                                  // Low above the low of the first candle

    // Check if the third candle is short and black (Marubozu) with no upper and lower shadows,
    // and its open within the high-low range of the second candle
    bool thirdCondition = (third.low == third.close) &&  // No lower shadow
                          (third.high == third.open) &&  // No upper shadow
                          (third.open > third.close) &&  // Black candle
                          (third.open <= second.high) && // Open within the high-low range of the second candle
                          (third.open >= second.low);

    return firstCondition && secondCondition && thirdCondition;
}

/**
 * @brief Detect the presence of a Three Stars in the North candlestick pattern.
 *
 * @param candles Vector of Candle data.
 * @param index Index of the candle to analyze.
 * @return true if Three Stars in the North pattern is detected, false otherwise.
 */
bool TripleCandlePattern::isThreeStarsInTheNorth(const std::vector<Candle> &candles, size_t index) const
{
    if (index < 2 || index >= candles.size())
    {
        return false;
    }

    const Candle &first = candles[index - 2];
    const Candle &second = candles[index - 1];
    const Candle &third = candles[index];

    // Check if the pattern occurs during an uptrend
    if (first.close < first.open || second.close < second.open || third.close < third.open)
    {
        return false;
    }

    // Check if the first candle is long and white with a long upper shadow and no lower shadow
    bool firstCondition = (first.high - first.low) >= 3 * (first.close - first.open) && // Long body
                          (first.open <= first.close) &&                                // White candle
                          (first.high - first.close) >= 2 * (first.open - first.low) && // Long upper shadow
                          (first.low == first.open || first.low == first.close);        // No lower shadow

    // Check if the second candle is white and shorter than the first candle, with its high below the high of the first candle
    bool secondCondition = (second.close > second.open) &&                            // White candle
                           ((second.high - second.low) < (first.high - first.low)) && // Shorter than first candle
                           (second.high < first.high);                                // High below the high of the first candle

    // Check if the third candle is short and white (Marubozu) with no upper and lower shadows,
    // and its close within the high-low range of the second candle
    bool thirdCondition = (third.high == third.close) &&  // No upper shadow
                          (third.low == third.open) &&    // No lower shadow
                          (third.close > third.open) &&   // White candle
                          (third.close <= second.high) && // Close within the high-low range of the second candle
                          (third.close >= second.low);

    return firstCondition;
}

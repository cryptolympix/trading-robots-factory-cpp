#ifndef CANDLE_PATTERN_HPP
#define CANDLE_PATTERN_HPP

#include <vector>
#include "../types.hpp"
#include "indicator.hpp"

/**
 * @brief Class for DojiCandlePattern indicator.
 */
class DojiCandlePattern : public Indicator
{
public:
    /**
     * @brief Construct a new DojiCandlePattern object.
     *
     * @param pattern_name Name of the pattern.
     * @param offset Offset value. Default is 0.
     */
    DojiCandlePattern(int offset = 0);

    /**
     * @brief Calculate the DojiCandlePattern values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;

private:
    /**
     * @brief Detect the presence of a doji candlestick pattern.
     *
     * @param candles Vector of Candle data.
     * @param index Index of the candle to analyze.
     * @return true if doji pattern is detected, false otherwise.
     */
    bool isClassicDoji(const std::vector<Candle> &candles, size_t index) const;

    /**
     * @brief Detect the presence of a long legged doji candlestick pattern.
     *
     * @param candles Vector of Candle data.
     * @param index Index of the candle to analyze.
     * @return true if long legged doji pattern is detected, false otherwise.
     */
    bool isLongLeggedDoji(const std::vector<Candle> &candles, size_t index) const;

    /**
     * @brief Detect the presence of a dragonfly doji candlestick pattern.
     *
     * @param candles Vector of Candle data.
     * @param index Index of the candle to analyze.
     * @return true if dragonfly doji pattern is detected, false otherwise.
     */
    bool isDragonflyDoji(const std::vector<Candle> &candles, size_t index) const;

    /**
     * @brief Detect the presence of a gravestone doji candlestick pattern.
     *
     * @param candles Vector of Candle data.
     * @param index Index of the candle to analyze.
     * @return true if gravestone doji pattern is detected, false otherwise.
     */
    bool isGravestoneDoji(const std::vector<Candle> &candles, size_t index) const;
};

/**
 * @brief Class for SingleCandlePattern indicator.
 */
class SingleCandlePattern : public Indicator
{
public:
    /**
     * @brief Construct a new SingleCandlePattern object.
     *
     * @param pattern_name Name of the pattern.
     * @param offset Offset value. Default is 0.
     */
    SingleCandlePattern(int offset = 0);

    /**
     * @brief Calculate the SingleCandlePattern values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;

private:
    /**
     * @brief Detect the presence of a hammer candlestick pattern.
     *
     * @param candles Vector of Candle data.
     * @param index Index of the candle to analyze.
     * @return true if hammer pattern is detected, false otherwise.
     */
    bool isHammer(const std::vector<Candle> &candles, size_t index) const;

    /**
     * @brief Detect the presence of an inverted hammer candlestick pattern.
     *
     * @param candles Vector of Candle data.
     * @param index Index of the candle to analyze.
     * @return true if inverted hammer pattern is detected, false otherwise.
     */
    bool isInvertedHammer(const std::vector<Candle> &candles, size_t index) const;

    /**
     * @brief Detect the presence of a shooting star candlestick pattern.
     *
     * @param candles Vector of Candle data.
     * @param index Index of the candle to analyze.
     * @return true if shooting star pattern is detected, false otherwise.
     */
    bool isShootingStar(const std::vector<Candle> &candles, size_t index) const;

    /**
     * @brief Detect the presence of a hanging man candlestick pattern.
     *
     * @param candles Vector of Candle data.
     * @param index Index of the candle to analyze.
     * @return true if hanging man pattern is detected, false otherwise.
     */
    bool isHangingMan(const std::vector<Candle> &candles, size_t index) const;

    /**
     * @brief Detect the presence of a spinning top candlestick pattern.
     *
     * @param candles Vector of Candle data.
     * @param index Index of the candle to analyze.
     * @return true if spinning top pattern is detected, false otherwise.
     */
    bool isBullishSpinningTop(const std::vector<Candle> &candles, size_t index) const;

    /**
     * @brief Detect the presence of a bearish spinning top candlestick pattern.
     *
     * @param candles Vector of Candle data.
     * @param index Index of the candle to analyze.
     * @return true if bearish spinning top pattern is detected, false otherwise.
     */
    bool isBearishSpinningTop(const std::vector<Candle> &candles, size_t index) const;

    /**
     * @brief Detect the presence of a bullish Marubozu candlestick pattern.
     *
     * @param candles Vector of Candle data.
     * @param index Index of the candle to analyze.
     * @return true if bullish Marubozu pattern is detected, false otherwise.
     */
    bool isBullishMarubozu(const std::vector<Candle> &candles, size_t index) const;

    /**
     * @brief Detect the presence of a bearish Marubozu candlestick pattern.
     *
     * @param candles Vector of Candle data.
     * @param index Index of the candle to analyze.
     * @return true if bearish Marubozu pattern is detected, false otherwise.
     */
    bool isBearishMarubozu(const std::vector<Candle> &candles, size_t index) const;
};

/**
 * @brief Class for QuadrupleCandlePattern indicator.
 */
class DoubleCandlePattern : public Indicator
{
public:
    /**
     * @brief Construct a new DoubleCandlePattern object.
     *
     * @param pattern_name Name of the pattern.
     * @param offset Offset value. Default is 0.
     */
    DoubleCandlePattern(int offset = 0);

    /**
     * @brief Calculate the DoubleCandlePattern values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;

private:
    /**
     * @brief Detect the presence of a bullish engulfing candlestick pattern.
     *
     * @param candles Vector of Candle data.
     * @param index Index of the candle to analyze.
     * @return true if bullish engulfing pattern is detected, false otherwise.
     */
    bool isBullishEngulfing(const std::vector<Candle> &candles, size_t index) const;

    /**
     * @brief Detect the presence of a bearish engulfing candlestick pattern.
     *
     * @param candles Vector of Candle data.
     * @param index Index of the candle to analyze.
     * @return true if bearish engulfing pattern is detected, false otherwise.
     */
    bool isBearishEngulfing(const std::vector<Candle> &candles, size_t index) const;

    /**
     * @brief Detect the presence of a piercing line candlestick pattern.
     *
     * @param candles Vector of Candle data.
     * @param index Index of the candle to analyze.
     * @return true if piercing line pattern is detected, false otherwise.
     */
    bool isPiercingLine(const std::vector<Candle> &candles, size_t index) const;

    /**
     * @brief Detect the presence of a dark cloud cover candlestick pattern.
     *
     * @param candles Vector of Candle data.
     * @param index Index of the candle to analyze.
     * @return true if dark cloud cover pattern is detected, false otherwise.
     */
    bool isDarkCloudCover(const std::vector<Candle> &candles, size_t index) const;
};

/**
 * @brief Class for TripleCandlePattern indicator.
 */
class TripleCandlePattern : public Indicator
{
public:
    /**
     * @brief Construct a new TripleCandlePattern object.
     *
     * @param pattern_name Name of the pattern.
     * @param offset Offset value. Default is 0.
     */
    TripleCandlePattern(int offset = 0);

    /**
     * @brief Calculate the TripleCandlePattern values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;

private:
    /**
     * @brief Detect the presence of a three white soldiers candlestick pattern.
     *
     * @param candles Vector of Candle data.
     * @param index Index of the candle to analyze.
     * @return true if three white soldiers pattern is detected, false otherwise.
     */
    bool isThreeWhiteSoldiers(const std::vector<Candle> &candles, size_t index) const;

    /**
     * @brief Detect the presence of a three black crows candlestick pattern.
     *
     * @param candles Vector of Candle data.
     * @param index Index of the candle to analyze.
     * @return true if three black crows pattern is detected, false otherwise.
     */
    bool isThreeBlackCrows(const std::vector<Candle> &candles, size_t index) const;

    /**
     * @brief Detect the presence of a morning star candlestick pattern.
     *
     * @param candles Vector of Candle data.
     * @param index Index of the candle to analyze.
     * @return true if morning star pattern is detected, false otherwise.
     */
    bool isMorningStar(const std::vector<Candle> &candles, size_t index) const;

    /**
     * @brief Detect the presence of an evening star candlestick pattern.
     *
     * @param candles Vector of Candle data.
     * @param index Index of the candle to analyze.
     * @return true if evening star pattern is detected, false otherwise.
     */
    bool isEveningStar(const std::vector<Candle> &candles, size_t index) const;

    /**
     * @brief Detect the presence of a morning doji star candlestick pattern.
     *
     * @param candles Vector of Candle data.
     * @param index Index of the candle to analyze.
     * @return true if morning doji star pattern is detected, false otherwise.
     */
    bool isMorningDojiStar(const std::vector<Candle> &candles, size_t index) const;

    /**
     * @brief Detect the presence of a evening doji star candlestick pattern.
     *
     * @param candles Vector of Candle data.
     * @param index Index of the candle to analyze.
     * @return true if evening doji star pattern is detected, false otherwise.
     */
    bool isEveningDojiStar(const std::vector<Candle> &candles, size_t index) const;

    /**
     * @brief Detect the presence of a three inside up candlestick pattern.
     *
     * @param candles Vector of Candle data.
     * @param index Index of the candle to analyze.
     * @return true if three inside up pattern is detected, false otherwise.
     */
    bool isThreeInsideUp(const std::vector<Candle> &candles, size_t index) const;

    /**
     * @brief Detect the presence of a three inside down candlestick pattern.
     *
     * @param candles Vector of Candle data.
     * @param index Index of the candle to analyze.
     * @return true if three inside down pattern is detected, false otherwise.
     */
    bool isThreeInsideDown(const std::vector<Candle> &candles, size_t index) const;

    /**
     * @brief Detect the presence of a three outside up candlestick pattern.
     *
     * @param candles Vector of Candle data.
     * @param index Index of the candle to analyze.
     * @return true if three outside up pattern is detected, false otherwise.
     */
    bool isThreeOutsideUp(const std::vector<Candle> &candles, size_t index) const;

    /**
     * @brief Detect the presence of a three outside down candlestick pattern.
     *
     * @param candles Vector of Candle data.
     * @param index Index of the candle to analyze.
     * @return true if three outside down pattern is detected, false otherwise.
     */
    bool isThreeOutsideDown(const std::vector<Candle> &candles, size_t index) const;

    /**
     * @brief Detect the presence of a Three Stars in the South candlestick pattern.
     *
     * @param candles Vector of Candle data.
     * @param index Index of the candle to analyze.
     * @return true if Three Stars in the South pattern is detected, false otherwise.
     */
    bool isThreeStarsInTheSouth(const std::vector<Candle> &candles, size_t index) const;

    /**
     * @brief Detect the presence of a Three Stars in the North candlestick pattern.
     *
     * @param candles Vector of Candle data.
     * @param index Index of the candle to analyze.
     * @return true if Three Stars in the North pattern is detected, false otherwise.
     */
    bool isThreeStarsInTheNorth(const std::vector<Candle> &candles, size_t index) const;
};

#endif // CANDLE_PATTERN_HPP
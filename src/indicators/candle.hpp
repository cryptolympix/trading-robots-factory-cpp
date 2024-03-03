#ifndef CANDLE_INDICATORS_H
#define CANDLE_INDICATORS_H

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <ctime>
#include <cmath>
#include "../types.hpp"
#include "../utils/math.hpp"
#include "../utils/candles_source.hpp"
#include "indicator.hpp"

/**
 * @brief Class for CandleOpen indicator.
 */
class CandleOpen : public Indicator
{
public:
    /**
     * @brief Construct a new CandleOpen object.
     *
     * @param offset Offset value. Default is 0.
     */
    CandleOpen(int offset = 0);

    /**
     * @brief Calculate the CandleOpen values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;
};

/**
 * Class for CandleHigh indicator.
 */
class CandleHigh : public Indicator
{
public:
    /**
     * @brief Construct a new CandleHigh object.
     *
     * @param offset Offset value. Default is 0.
     */
    CandleHigh(int offset = 0);

    /**
     * @brief Calculate the CandleHigh values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;
};

/**
 * @brief Class for CandleLow indicator.
 */
class CandleLow : public Indicator
{
public:
    /**
     * @brief Construct a new CandleLow object.
     *
     * @param offset Offset value. Default is 0.
     */
    CandleLow(int offset = 0);

    /**
     * @brief Calculate the CandleLow values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;
};

/**
 * @brief Class for CandleClose indicator.
 */
class CandleClose : public Indicator
{
public:
    /**
     * @brief Construct a new CandleClose object.
     *
     * @param offset Offset value. Default is 0.
     */
    CandleClose(int offset = 0);

    /**
     * @brief Calculate the CandleClose values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;
};

/**
 * @brief Class for CandleVolume indicator.
 */
class CandleVolume : public Indicator
{
public:
    /**
     * @brief Construct a new CandleVolume object.
     *
     * @param offset Offset value. Default is 0.
     */
    CandleVolume(int offset = 0);

    /**
     * @brief Calculate the CandleVolume values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;
};

/**
 * @brief Class for WhiteCandle indicator.
 */
class WhiteCandle : public Indicator
{
public:
    /**
     * @brief Construct a new WhiteCandle object.
     *
     * @param offset Offset value. Default is 0.
     */
    WhiteCandle(int offset = 0);

    /**
     * @brief Calculate the WhiteCandle values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;
};

/**
 * @brief Class for BlackCandle indicator.
 */
class BlackCandle : public Indicator
{
public:
    /**
     * @brief Construct a new BlackCandle object.
     *
     * @param offset Offset value. Default is 0.
     */
    BlackCandle(int offset = 0);

    /**
     * @brief Calculate the BlackCandle values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;
};

/**
 * @brief Class for CandlePriceChange indicator.
 */
class CandlePriceChange : public Indicator
{
public:
    /**
     * @brief Construct a new CandlePriceChange object.
     *
     * @param offset Offset value. Default is 0.
     */
    CandlePriceChange(int offset = 0);

    /**
     * @brief Calculate the CandlePriceChange values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;
};

/**
 * @brief Class for PivotHigh indicator.
 */
class PivotHigh : public Indicator
{
public:
    /**
     * @brief Construct a new PivotHigh object.
     *
     * @param source Source of candle data.
     * @param left_bars Number of left bars.
     * @param right_bars Number of right bars.
     * @param offset Offset value. Default is 0.
     */
    PivotHigh(CandleSource source, int left_bars, int right_bars, int offset = 0);

    /**
     * @brief Calculate the PivotHigh values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;

private:
    CandleSource source;
    int left_bars;
    int right_bars;
};

/**
 * @brief Class for PivotLow indicator.
 */
class PivotLow : public Indicator
{
public:
    /**
     * @brief Construct a new PivotLow object.
     *
     * @param source Source of candle data.
     * @param left_bars Number of left bars.
     * @param right_bars Number of right bars.
     * @param offset Offset value. Default is 0.
     */
    PivotLow(CandleSource source, int left_bars, int right_bars, int offset = 0);

    /**
     * @brief Calculate the PivotLow values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;

private:
    CandleSource source;
    int left_bars;
    int right_bars;
};

/**
 * @brief Class for PivotHighValue indicator.
 */
class PivotHighValue : public Indicator
{
public:
    /**
     * @brief Construct a new PivotHighValue object.
     *
     * @param source Source of candle data.
     * @param left_bars Number of left bars.
     * @param right_bars Number of right bars.
     * @param offset Offset value. Default is 0.
     */
    PivotHighValue(CandleSource source, int left_bars, int right_bars, int offset = 0);

    /**
     * @brief Calculate the PivotHighValue values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;

private:
    CandleSource source;
    int left_bars;
    int right_bars;
};

/**
 * @brief Class for PivotLowValue indicator.
 */
class PivotLowValue : public Indicator
{
public:
    /**
     * @brief Construct a new PivotLowValue object.
     *
     * @param source Source of candle data.
     * @param left_bars Number of left bars.
     * @param right_bars Number of right bars.
     * @param offset Offset value. Default is 0.
     */
    PivotLowValue(CandleSource source, int left_bars, int right_bars, int offset = 0);

    /**
     * @brief Calculate the PivotLowValue values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;

private:
    CandleSource source;
    int left_bars;
    int right_bars;
};

/**
 * @brief Class for HighestHigh indicator.
 */
class HighestHigh : public Indicator
{
public:
    /**
     * @brief Construct a new HighestHigh object.
     *
     * @param left_bars Number of left bars.
     * @param right_bars Number of right bars.
     * @param offset Offset value. Default is 0.
     */
    HighestHigh(int left_bars, int right_bars, int offset = 0);

    /**
     * @brief Calculate the HighestHigh values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;

private:
    int left_bars;
    int right_bars;
};

/**
 * @brief Class for LowestLow indicator.
 */
class LowestLow : public Indicator
{
public:
    /**
     * @brief Construct a new LowestLow object.
     *
     * @param left_bars Number of left bars.
     * @param right_bars Number of right bars.
     * @param offset Offset value. Default is 0.
     */
    LowestLow(int left_bars, int right_bars, int offset = 0);

    /**
     * @brief Calculate the LowestLow values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;

private:
    int left_bars;
    int right_bars;
};

/**
 * @brief Class for CandlePattern indicator.
 */
class CandlePattern : public Indicator
{
public:
    /**
     * @brief Construct a new CandlePattern object.
     *
     * @param pattern_name Name of the pattern.
     * @param offset Offset value. Default is 0.
     */
    CandlePattern(int offset = 0);

    /**
     * @brief Calculate the CandlePattern values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    virtual std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;

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
     * @brief Detect the presence of a shooting star candlestick pattern.
     *
     * @param candles Vector of Candle data.
     * @param index Index of the candle to analyze.
     * @return true if shooting star pattern is detected, false otherwise.
     */
    bool isShootingStar(const std::vector<Candle> &candles, size_t index) const;

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
     * @brief Detect the presence of a doji candlestick pattern.
     *
     * @param candles Vector of Candle data.
     * @param index Index of the candle to analyze.
     * @return true if doji pattern is detected, false otherwise.
     */
    bool isDoji(const std::vector<Candle> &candles, size_t index) const;

    /**
     * @brief Detect the presence of a spinning top candlestick pattern.
     *
     * @param candles Vector of Candle data.
     * @param index Index of the candle to analyze.
     * @return true if spinning top pattern is detected, false otherwise.
     */
    bool isSpinningTop(const std::vector<Candle> &candles, size_t index) const;

    /**
     * @brief Detect the presence of a marubozu candlestick pattern.
     *
     * @param candles Vector of Candle data.
     * @param index Index of the candle to analyze.
     * @return true if marubozu pattern is detected, false otherwise.
     */
    bool isMarubozu(const std::vector<Candle> &candles, size_t index) const;

    /**
     * @brief Detect the presence of a harami candlestick pattern.
     *
     * @param candles Vector of Candle data.
     * @param index Index of the candle to analyze.
     * @return true if harami pattern is detected, false otherwise.
     */
    bool isHarami(const std::vector<Candle> &candles, size_t index) const;

    /**
     * @brief Detect the presence of a harami cross candlestick pattern.
     *
     * @param candles Vector of Candle data.
     * @param index Index of the candle to analyze.
     * @return true if harami cross pattern is detected, false otherwise.
     */
    bool isHaramiCross(const std::vector<Candle> &candles, size_t index) const;

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
     * @brief Detect the presence of a three stars in the south candlestick pattern.
     *
     * @param candles Vector of Candle data.
     * @param index Index of the candle to analyze.
     * @return true if three stars in the south pattern is detected, false otherwise.
     */
    bool isThreeStarsInTheSouth(const std::vector<Candle> &candles, size_t index) const;

    /**
     * @brief Detect the presence of a three advancing white soldiers candlestick pattern.
     *
     * @param candles Vector of Candle data.
     * @param index Index of the candle to analyze.
     * @return true if three advancing white soldiers pattern is detected, false otherwise.
     */
    bool isThreeAdvancingWhiteSoldiers(const std::vector<Candle> &candles, size_t index) const;

    /**
     * @brief Detect the presence of a three identical crows candlestick pattern.
     *
     * @param candles Vector of Candle data.
     * @param index Index of the candle to analyze.
     * @return true if three identical crows pattern is detected, false otherwise.
     */
    bool isThreeIdenticalCrows(const std::vector<Candle> &candles, size_t index) const;

    /**
     * @brief Detect the presence of a abandoned baby candlestick pattern.
     *
     * @param candles Vector of Candle data.
     * @param index Index of the candle to analyze.
     * @return true if abandoned baby pattern is detected, false otherwise.
     */
    bool isAbandonedBaby(const std::vector<Candle> &candles, size_t index) const;

    /**
     * @brief Detect the presence of a dark cloud cover candlestick pattern.
     *
     * @param candles Vector of Candle data.
     * @param index Index of the candle to analyze.
     * @return true if dark cloud cover pattern is detected, false otherwise.
     */
    bool isDarkCloudCover(const std::vector<Candle> &candles, size_t index) const;

    /**
     * @brief Detect the presence of a evening doji star candlestick pattern.
     *
     * @param candles Vector of Candle data.
     * @param index Index of the candle to analyze.
     * @return true if evening doji star pattern is detected, false otherwise.
     */
    bool isEveningDojiStar(const std::vector<Candle> &candles, size_t index) const;
};

#endif /* CANDLE_INDICATORS_H */

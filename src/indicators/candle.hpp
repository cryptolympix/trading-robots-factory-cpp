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
 * @brief Class for CandleTickVolume indicator.
 */
class CandleTickVolume : public Indicator
{
public:
    /**
     * @brief Construct a new CandleTickVolume object.
     *
     * @param offset Offset value. Default is 0.
     */
    CandleTickVolume(int offset = 0);

    /**
     * @brief Calculate the CandleTickVolume values.
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
 *
 * This indicator calculates the white candlestick pattern.
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
 *
 * This indicator calculates the black candlestick pattern.
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
 * @brief Class for CandleBody indicator.
 *
 * This indicator calculates the body ratio of the candlestick.
 */
class CandleBody : public Indicator
{
public:
    /**
     * @brief Construct a new CandleBody object.
     *
     * @param offset Offset value. Default is 0.
     */
    CandleBody(int offset = 0);

    /**
     * @brief Calculate the CandleBody values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;
};

/**
 * @brief Class for CandleShadowUpper indicator.
 *
 * This indicator calculates the upper shadow ratio of the candlestick.
 */
class CandleShadowUpper : public Indicator
{
public:
    /**
     * @brief Construct a new CandleShadowUpper object.
     *
     * @param offset Offset value. Default is 0.
     */
    CandleShadowUpper(int offset = 0);

    /**
     * @brief Calculate the CandleShadowUpper values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;
};

/**
 * @brief Class for CandleShadowLower indicator.
 *
 * This indicator calculates the lower shadow ratio of the candlestick.
 */
class CandleShadowLower : public Indicator
{
public:
    /**
     * @brief Construct a new CandleShadowLower object.
     *
     * @param offset Offset value. Default is 0.
     */
    CandleShadowLower(int offset = 0);

    /**
     * @brief Calculate the CandleShadowLower values.
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
    PivotHigh(std::string source, int left_bars, int right_bars, int offset = 0);

    /**
     * @brief Calculate the PivotHigh values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;
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
    PivotLow(std::string source, int left_bars, int right_bars, int offset = 0);

    /**
     * @brief Calculate the PivotLow values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;
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
    PivotHighValue(std::string source, int left_bars, int right_bars, int offset = 0);

    /**
     * @brief Calculate the PivotHighValue values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;
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
    PivotLowValue(std::string source, int left_bars, int right_bars, int offset = 0);

    /**
     * @brief Calculate the PivotLowValue values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;
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
};

/**
 * @brief Class for PeakDistance indicator.
 *
 * This indicator calculates the distance between the highest high and lowest low in a given range.
 */
class PeakDistance : public Indicator
{
public:
    /**
     * @brief Construct a new PeakDistance object.
     *
     * @param left_bars Number of left bars.
     * @param right_bars Number of right bars.
     * @param offset Offset value. Default is 0.
     */
    PeakDistance(int left_bars, int right_bars, int offset = 0);

    /**
     * @brief Calculate the PeakDistance values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;
};

/**
 * @brief Class for PeakCandleDistance indicator.
 *
 * This indicator calculates the distance from the previous pivot high or pivot low in a given range in terms of time.
 */
class PeakCandleDistance : public Indicator
{
public:
    /**
     * @brief Construct a new PeakCandleDistance object.
     *
     * @param left_bars Number of left bars.
     * @param right_bars Number of right bars.
     * @param offset Offset value. Default is 0.
     */
    PeakCandleDistance(int left_bars, int right_bars, int offset = 0);

    /**
     * @brief Calculate the PeakCandleDistance values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;
};

#endif /* CANDLE_INDICATORS_H */

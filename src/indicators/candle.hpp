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
    std::string label = "Candle Open";
    std::string id = "candle-open";
    std::string id_pattern = R"(candle-open-(\d+))";

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
    std::string label = "Candle High";
    std::string id = "candle-high";
    std::string id_pattern = R"(candle-high-(\d+))";

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
    std::string label = "Candle Low";
    std::string id = "candle-low";
    std::string id_pattern = R"(candle-low-(\d+))";

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
    std::string label = "Candle Close";
    std::string id = "candle-close";
    std::string id_pattern = R"(candle-close-(\d+))";

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
    std::string label = "Candle Tick Volume";
    std::string id = "candle-tick-volume";
    std::string id_pattern = R"(candle-tick-volume-(\d+))";

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
    std::string label = "Candle Volume";
    std::string id = "candle-volume";
    std::string id_pattern = R"(candle-volume-(\d+))";

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
    std::string label = "White Candle";
    std::string id = "white-candle";
    std::string id_pattern = R"(white-candle-(\d+))";

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
    std::string label = "Black Candle";
    std::string id = "black-candle";
    std::string id_pattern = R"(black-candle-(\d+))";

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
    std::string label = "Candle Body";
    std::string id = "candle-body";
    std::string id_pattern = R"(candle-body-(\d+))";

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
    std::string label = "Candle Shadow Upper";
    std::string id = "candle-shadow-upper";
    std::string id_pattern = R"(candle-shadow-upper-(\d+))";

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
    std::string label = "Candle Shadow Lower";
    std::string id = "candle-shadow-lower";
    std::string id_pattern = R"(candle-shadow-lower-(\d+))";

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
    std::string label = "Candle Price Change";
    std::string id = "candle-price-change";
    std::string id_pattern = R"(candle-price-change-(\d+))";

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
    std::string label = "Pivot High";
    std::string id = "pivot-high";
    std::string id_pattern = R"(pivot-high-(\w+)-(\d+)-(\d+)-(\d+))";

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

private:
    std::string source;
    int left_bars;
    int right_bars;
};

/**
 * @brief Class for PivotLow indicator.
 */
class PivotLow : public Indicator
{
public:
    std::string label = "Pivot Low";
    std::string id = "pivot-low";
    std::string id_pattern = R"(pivot-low-(\w+)-(\d+)-(\d+)-(\d+))";

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

private:
    std::string source;
    int left_bars;
    int right_bars;
};

/**
 * @brief Class for PivotHighValue indicator.
 */
class PivotHighValue : public Indicator
{
public:
    std::string label = "Pivot High Value";
    std::string id = "pivot-high-value";
    std::string id_pattern = R"(pivot-high-value-(\w+)-(\d+)-(\d+)-(\d+))";

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

private:
    std::string source;
    int left_bars;
    int right_bars;
};

/**
 * @brief Class for PivotLowValue indicator.
 */
class PivotLowValue : public Indicator
{
public:
    std::string label = "Pivot Low Value";
    std::string id = "pivot-low-value";
    std::string id_pattern = R"(pivot-low-value-(\w+)-(\d+)-(\d+)-(\d+))";

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

private:
    std::string source;
    int left_bars;
    int right_bars;
};

/**
 * @brief Class for HighestHigh indicator.
 */
class HighestHigh : public Indicator
{
public:
    std::string label = "Highest High";
    std::string id = "highest-high";
    std::string id_pattern = R"(highest-high-(\d+)-(\d+)-(\d+))";

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
    std::string label = "Lowest Low";
    std::string id = "lowest-low";
    std::string id_pattern = R"(lowest-low-(\d+)-(\d+)-(\d+))";

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
 * @brief Class for PeakDistance indicator.
 *
 * This indicator calculates the distance between the highest high and lowest low in a given range.
 */
class PeakDistance : public Indicator
{
public:
    std::string label = "Peak Distance";
    std::string id = "peak-distance";
    std::string id_pattern = R"(peak-distance-(\d+)-(\d+)-(\d+))";

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

private:
    int left_bars;
    int right_bars;
};

/**
 * @brief Class for PeakCandleDistance indicator.
 *
 * This indicator calculates the distance from the previous pivot high or pivot low in a given range in terms of time.
 */
class PeakCandleDistance : public Indicator
{
public:
    std::string label = "Peak Candle Distance";
    std::string id = "peak-candle-distance";
    std::string id_pattern = R"(peak-candle-distance-(\d+)-(\d+)-(\d+))";

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

private:
    int left_bars;
    int right_bars;
};

#endif /* CANDLE_INDICATORS_H */

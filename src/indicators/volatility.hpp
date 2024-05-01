#ifndef INDICATORS_VOLATILITY_HPP
#define INDICATORS_VOLATILITY_HPP

#include "indicator.hpp"

/**
 * Average True Range (ATR)
 *
 * The indicator provide an indication of the degree of price volatility. Strong moves, in either direction, are often accompanied by large ranges, or large True Ranges.
 *
 * http://stockcharts.com/school/doku.php?id=chart_school:technical_indicators:average_true_range_atr
 */
class ATR : public Indicator
{
public:
    /**
     * @brief Construct a new ATR object.
     *
     * @param period Period value. Default is 14.
     * @param offset Offset value. Default is 0.
     */
    ATR(int period = 14, int offset = 0);

    /**
     * @brief Calculate the Average True Range values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;

private:
    /**
     * @brief Calculate the True Range for a specific candle.
     *
     * @param candles Vector of Candle data.
     * @return std::vector<double> The calculated True Range values.
     */
    std::vector<double> calculateTrueRange(const std::vector<Candle> &candles) const;

    int period;
};

// *****************************************************************************

class BollingerChannelHighBand : public Indicator
{
public:
    /**
     * @brief Construct a new BollingerChannelHighBand object.
     *
     * @param period Period value. Default is 20.
     * @param multiplier Multiplier value. Default is 2.0.
     * @param offset Offset value. Default is 0.
     */
    BollingerChannelHighBand(int period = 20, double multiplier = 2.0, int offset = 0);

    /**
     * @brief Calculate the Bollinger Channel High Band values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;

private:
    int period;
    double multiplier;
};

// *****************************************************************************

class BollingerChannelLowBand : public Indicator
{
public:
    /**
     * @brief Construct a new BollingerChannelLowBand object.
     *
     * @param period Period value. Default is 20.
     * @param multiplier Multiplier value. Default is 2.0.
     * @param offset Offset value. Default is 0.
     */
    BollingerChannelLowBand(int period = 20, double multiplier = 2.0, int offset = 0);

    /**
     * @brief Calculate the Bollinger Channel Low Band values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;

private:
    int period;
    double multiplier;
};

// *****************************************************************************

class BollingerChannelMiddleBand : public Indicator
{
public:
    /**
     * @brief Construct a new BollingerChannelMiddleBand object.
     *
     * @param period Period value. Default is 20.
     * @param offset Offset value. Default is 0.
     */
    BollingerChannelMiddleBand(int period = 20, int offset = 0);

    /**
     * @brief Calculate the Bollinger Channel Middle Band values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;

private:
    int period;
};

// *****************************************************************************

/**
 * Bollinger Band Percent (BB %B) quantifies a symbol's price relative to the upper and lower Bollinger Band. There are six basic relationship levels:
 * - %B equals 1 when price is at the upper band
 * - %B equals 0 when price is at the lower band
 * - %B is above 1 when price is above the upper band
 * - %B is below 0 when price is below the lower band
 * - %B is above .50 when price is above the middle band (20-day SMA)
 * - %B is below .50 when price is below the middle band (20-day SMA)
 *
 * @see https://www.barchart.com/education/technical-indicators/bollinger_bands_percent
 */
class BollingerChannelPercentageBand : public Indicator
{
public:
    /**
     * @brief Construct a new BollingerChannelPercentageBand object.
     *
     * @param period Period value. Default is 20.
     * @param multiplier Multiplier value. Default is 2.0.
     * @param offset Offset value. Default is 0.
     */
    BollingerChannelPercentageBand(int period = 20, double multiplier = 2.0, int offset = 0);

    /**
     * @brief Calculate the Bollinger Channel Percentage Band values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;

private:
    int period;
    double multiplier;
};

// *****************************************************************************

class BollingerChannelWidthBand : public Indicator
{
public:
    /**
     * @brief Construct a new BollingerChannelWidthBand object.
     *
     * @param period Period value. Default is 20.
     * @param multiplier Multiplier value. Default is 2.0.
     * @param offset Offset value. Default is 0.
     */
    BollingerChannelWidthBand(int period = 20, double multiplier = 2.0, int offset = 0);

    /**
     * @brief Calculate the Bollinger Channel Width Band values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;

private:
    int period;
    double multiplier;
};

// *****************************************************************************

class DonchianChannelHighBand : public Indicator
{
public:
    /**
     * @brief Construct a new DonchianChannelHighBand object.
     *
     * @param period Period value. Default is 20.
     * @param offset Offset value. Default is 0.
     */
    DonchianChannelHighBand(int period = 20, int offset = 0);

    /**
     * @brief Calculate the Donchian Channel High Band values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;

private:
    int period;
};

// *****************************************************************************

class DonchianChannelLowBand : public Indicator
{
public:
    /**
     * @brief Construct a new DonchianChannelLowBand object.
     *
     * @param period Period value. Default is 20.
     * @param offset Offset value. Default is 0.
     */
    DonchianChannelLowBand(int period = 20, int offset = 0);

    /**
     * @brief Calculate the Donchian Channel Low Band values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;

private:
    int period;
};

// *****************************************************************************

class DonchianChannelMiddleBand : public Indicator
{
public:
    /**
     * @brief Construct a new DonchianChannelMiddleBand object.
     *
     * @param period Period value. Default is 20.
     * @param offset Offset value. Default is 0.
     */
    DonchianChannelMiddleBand(int period = 20, int offset = 0);

    /**
     * @brief Calculate the Donchian Channel Middle Band values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;

private:
    int period;
};

// *****************************************************************************

class KeltnerChannelHighBand : public Indicator
{
public:
    /**
     * @brief Construct a new KeltnerChannelHighBand object.
     *
     * @param period Period value. Default is 20.
     * @param atr_period ATR period value. Default is 10.
     * @param multiplier Multiplier value. Default is 2.0.
     * @param offset Offset value. Default is 0.
     */
    KeltnerChannelHighBand(int period = 20, int atr_period = 10, double multiplier = 2.0, int offset = 0);

    /**
     * @brief Calculate the Keltner Channel High Band values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;

private:
    int period;
    int atr_period;
    double multiplier;
};

// *****************************************************************************

class KeltnerChannelLowBand : public Indicator
{
public:
    /**
     * @brief Construct a new KeltnerChannelLowBand object.
     *
     * @param period Period value. Default is 20.
     * @param atr_period ATR period value. Default is 10.
     * @param multiplier Multiplier value. Default is 2.0.
     * @param offset Offset value. Default is 0.
     */
    KeltnerChannelLowBand(int period = 20, int atr_period = 10, double multiplier = 2.0, int offset = 0);

    /**
     * @brief Calculate the Keltner Channel Low Band values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;

private:
    int period;
    int atr_period;
    double multiplier;
};

// *****************************************************************************

class KeltnerChannelMiddleBand : public Indicator
{
public:
    /**
     * @brief Construct a new KeltnerChannelMiddleBand object.
     *
     * @param period Period value. Default is 20.
     * @param offset Offset value. Default is 0.
     */
    KeltnerChannelMiddleBand(int period = 20, int offset = 0);

    /**
     * @brief Calculate the Keltner Channel Middle Band values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;

private:
    int period;
};

// *****************************************************************************

/**
 * The Ulcer Index (UI) is a technical indicator that measures downside risk in terms of price declines both in magnitude and duration.
 */
class UI : public Indicator
{
public:
    /**
     * @brief Construct a new Ulcer Index object.
     *
     * @param period Period value. Default is 14.
     * @param offset Offset value. Default is 0.
     */
    UI(int period = 14, int offset = 0);

    /**
     * @brief Calculate the Ulcer Index values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;

private:
    int period;
};

// *****************************************************************************

class StandardDeviation : public Indicator
{
public:
    /**
     * @brief Construct a new Standard Deviation object.
     *
     * @param period Period value. Default is 14.
     * @param offset Offset value. Default is 0.
     */
    StandardDeviation(int period = 14, int offset = 0);

    /**
     * @brief Calculate the Standard Deviation values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;

private:
    int period;
};

// *****************************************************************************

class AveragePriceChange : public Indicator
{
public:
    /**
     * @brief Construct a new Average Price Change object.
     *
     * @param period Period value. Default is 14.
     * @param offset Offset value. Default is 0.
     */
    AveragePriceChange(int period = 14, int offset = 0);

    /**
     * @brief Calculate the Average Price Change values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;

private:
    int period;
};

#endif // INDICATORS_VOLATILITY_HPP
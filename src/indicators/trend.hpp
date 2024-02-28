#ifndef TREND_INDICATORS_HPP
#define TREND_INDICATORS_HPP

#include <vector>
#include <stdexcept>
#include "../types.hpp"
#include "../utils/math.hpp"
#include "../utils/candles_source.hpp"
#include "momentum.hpp"
#include "indicator.hpp"

/**
 * Average Directional Movement Index (ADX)
 *
 * The Plus Directional Indicator (+DI) and Minus Directional Indicator (-DI) are derived from smoothed averages of these differences, and measure trend direction over time. These two indicators are often referred to collectively as the Directional Movement Indicator (DMI).
 *
 * The Average Directional Index (ADX) is in turn derived from the smoothed averages of the difference between +DI and -DI, and measures the strength of the trend (regardless of direction) over time.
 *
 * Using these three indicators together, chartists can determine both the direction and strength of the trend.
 */
class ADX : public Indicator
{
public:
    /**
     * @brief Construct a new Average Directional Movement Index object.
     *
     * @param adx_period Period value. Default is 14.
     * @param offset Offset value. Default is 0.
     */
    ADX(int adx_period = 14, int offset = 0);

    /**
     * @brief Calculate the Average Directional Movement Index values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;

private:
    int adx_period; // Period value

    /**
     * @brief Calculate the smoothed values.
     *
     * @param values Vector containing the values to be smoothed.
     * @return std::vector<double> Vector containing the smoothed values.
     */
    std::vector<double> calculate_smoothed(const std::vector<double> &values) const;

    /**
     * @brief Calculate the directional index values.
     *
     * @param smoothed_dm Vector containing smoothed directional movement values.
     * @param smoothed_tr Vector containing smoothed true range values.
     * @return std::vector<double> Vector containing the calculated directional index values.
     */
    std::vector<double> calculate_directional_index(const std::vector<double> &smoothed_dm, const std::vector<double> &smoothed_tr) const;

    /**
     * @brief Calculate the DX values.
     *
     * @param di_plus Vector containing Plus Directional Indicator values.
     * @param di_minus Vector containing Minus Directional Indicator values.
     * @return std::vector<double> Vector containing the calculated DX values.
     */
    std::vector<double> calculate_dx(const std::vector<double> &di_plus, const std::vector<double> &di_minus) const;

    /**
     * @brief Calculate the ADX values.
     *
     * @param dx_values Vector containing DX values.
     * @return std::vector<double> Vector containing the calculated ADX values.
     */
    std::vector<double> calculate_adx(const std::vector<double> &dx_values) const;
};

// *********************************************************************************************

/**
 * Aroon Indicator
 *
 * The Aroon indicator is used to determine the strength of a trend and the likelihood that it will continue. It is made up of two lines: one line is called "Aroon up", which measures the strength of the uptrend, and the other line is called "Aroon down", which measures the strength of the downtrend.
 */
class AroonTrend : public Indicator
{
public:
    /**
     * @brief Construct a new AroonTrend object.
     *
     * @param period Period value. Default is 14.
     * @param offset Offset value. Default is 0.
     */
    AroonTrend(int period = 14, int offset = 0);

    /**
     * @brief Calculate the Aroon values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;

private:
    int period;
};

// *********************************************************************************************

/**
 * Commodity Channel Index (CCI)
 *
 * CCI measures the difference between a security’s price change and its average price change. High positive readings indicate that prices are well above their average, which is a show of strength. Low negative readings indicate that prices are well below their average, which is a show of weakness.
 */
class CCI : public Indicator
{
public:
    /**
     * @brief Construct a new CCI object.
     *
     * @param period Period value. Default is 20.
     * @param offset Offset value. Default is 0.
     */
    CCI(int period = 20, int offset = 0);

    /**
     * @brief Calculate the CCI values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;

private:
    int period;

    /**
     * @brief Calculate the mean deviation of typical prices.
     *
     * @param typical_prices Vector containing typical prices.
     * @param sma_values Vector containing simple moving average values.
     * @return std::vector<double> Vector containing mean deviation values.
     */
    std::vector<double> calculate_mean_deviation(const std::vector<double> &typical_prices, const std::vector<double> &sma_values) const;
};

// *********************************************************************************************

/**
 * Detrended Price Oscillator (DPO)
 *
 * Is an indicator designed to remove trend from price and make it easier to identify cycles.
 */
class DPO : public Indicator
{
public:
    /**
     * @brief Construct a new DPO object.
     *
     * @param period Period value. Default is 20.
     * @param offset Offset value. Default is 0.
     */
    DPO(int period = 20, int offset = 0);

    /**
     * @brief Calculate the DPO values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;

private:
    int period;
};

// *********************************************************************************************

/**
 * EMA - Exponential Moving Average
 *
 * Is a type of moving average that places a greater weight and significance on the most recent data points.

 */
class EMA : public Indicator
{
public:
    /**
     * @brief Construct a new EMA object.
     *
     * @param source Candle source.
     * @param period Period value.
     * @param offset Offset value. Default is 0.
     */
    EMA(CandleSource source, int period, int offset = 0);

    /**
     * @brief Calculate the EMA values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;

private:
    CandleSource source;
    int period;
};

// *********************************************************************************************

/**
 * KST (Know Sure Thing) Oscillator
 *
 * It is useful to identify major stock market cycle junctures because its formula is weighed to be more greatly influenced by the longer and more dominant time spans, in order to better reflect the primary swings of stock market cycle.
 */
class KST : public Indicator
{
public:
    /**
     * @brief Construct a new KST Oscillator object.
     *
     * @param roc_periods1 Periods for Rate of Change (ROC) 1. Default is 10.
     * @param roc_periods2 Periods for Rate of Change (ROC) 2. Default is 15.
     * @param roc_periods3 Periods for Rate of Change (ROC) 3. Default is 20.
     * @param roc_periods4 Periods for Rate of Change (ROC) 4. Default is 30.
     * @param sma_periods1 Periods for Simple Moving Average (SMA) 1. Default is 10.
     * @param sma_periods2 Periods for Simple Moving Average (SMA) 2. Default is 10.
     * @param sma_periods3 Periods for Simple Moving Average (SMA) 3. Default is 10.
     * @param sma_periods4 Periods for Simple Moving Average (SMA) 4. Default is 15.
     * @param signal_periods Periods for the signal line. Default is 9.
     * @param offset Offset value. Default is 0.
     */
    KST(int roc_periods1 = 10, int roc_periods2 = 15, int roc_periods3 = 20, int roc_periods4 = 30,
        int sma_periods1 = 10, int sma_periods2 = 10, int sma_periods3 = 10, int sma_periods4 = 15,
        int signal_periods = 9, int offset = 0);

    /**
     * @brief Calculate the KST Oscillator values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;

private:
    int roc_periods1, roc_periods2, roc_periods3, roc_periods4;
    int sma_periods1, sma_periods2, sma_periods3, sma_periods4;
    int signal_periods;
};

// *********************************************************************************************

/**
 * MACD (Moving Average Convergence Divergence)
 *
 * Is a trend-following momentum indicator that shows the relationship between two moving averages of a security’s price.
 */
class MACD : public Indicator
{
public:
    /**
     * @brief Construct a new MACD object.
     *
     * @param short_period Short EMA period. Default is 12.
     * @param long_period Long EMA period. Default is 26.
     * @param signal_period Signal EMA period. Default is 9.
     * @param offset Offset value. Default is 0.
     */
    MACD(int short_period = 12, int long_period = 26, int signal_period = 9, int offset = 0);

    /**
     * @brief Calculate the MACD line and signal line.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing MACD line.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;

private:
    int short_period;  // Short EMA period
    int long_period;   // Long EMA period
    int signal_period; // Signal EMA period

    /**
     * @brief Calculate the MACD line values.
     *
     * @param closes Vector containing close prices.
     * @return std::vector<double> Vector containing the calculated MACD line values.
     */
    std::vector<double> calculate_macd_line(const std::vector<double> &closes) const;

    /**
     * @brief Calculate the signal line values.
     *
     * @param macd_line Vector containing MACD line values.
     * @return std::vector<double> Vector containing the calculated signal line values.
     */
    std::vector<double> calculate_signal_line(const std::vector<double> &macd_line) const;
};

// *********************************************************************************************

/**
 * Mass Index (MI)
 *
 * It uses the high-low range to identify trend reversals based on range expansions. It identifies range bulges that can foreshadow a reversal of the current trend.
 */
class MI : public Indicator
{
public:
    /**
     * @brief Construct a new MassIndex object.
     *
     * @param offset Offset value. Default is 0.
     */
    MI(int offset = 0);

    /**
     * @brief Calculate the Mass Index values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;
};

// *********************************************************************************************

/**
 * @brief Class to calculate the Parabolic Stop and Reverse (SAR) indicator.
 *
 * The Parabolic SAR is a technical indicator used to determine the price direction of an asset, as well as draw attention to when the price direction is changing.
 */
class ParabolicSAR : public Indicator
{
public:
    /**
     * @brief Construct a new Parabolic SAR object.
     *
     * @param acceleration_factor_initial Initial acceleration factor value. Default is 0.02.
     * @param acceleration_factor_maximum Maximum acceleration factor value. Default is 0.2.
     * @param offset Offset value. Default is 0.
     */
    ParabolicSAR(double acceleration_factor_initial = 0.02, double acceleration_factor_maximum = 0.2, int offset = 0);

    /**
     * @brief Calculate the Parabolic SAR values.
     *
     * @param candles Vector of Candle data.
     * @return std::vector<double> Vector containing calculated Parabolic SAR values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;

private:
    double acceleration_factor_initial; // Initial acceleration factor value.
    double acceleration_factor_maximum; // Maximum acceleration factor value.
    int offset;                         // Offset value.
};

// *********************************************************************************************

/**
 * @brief Class to calculate the Simple Moving Average (SMA) indicator.
 *
 * The Simple Moving Average (SMA) is the average of the closing prices for a specified number of periods.
 */
class SMA : public Indicator
{

public:
    /**
     * @brief Construct a new SMA object.
     *
     * @param source Candle source.
     * @param period Period for the SMA calculation.
     * @param offset Offset value. Default is 0.
     */
    SMA(CandleSource source, int period, int offset = 0);

    /**
     * @brief Calculate the Simple Moving Average (SMA).
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing the calculated SMA values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;

private:
    CandleSource source; // Candle source
    int period;          // Period for the SMA calculation
};

// *********************************************************************************************

/**
 * @brief CLass to calculate the Schaff Trend Cycle (STC) indicator.
 *
 * The Schaff Trend Cycle (STC) is a technical indicator that is used to identify market trends and provide buy and sell signals to traders.
 */
class STC : public Indicator
{
public:
    /**
     * @brief Construct a new STC object.
     *
     * @param short_length The period for the shorter-term EMA (default: 23).
     * @param long_length The period for the longer-term EMA (default: 50).
     * @param cycle_length The length of the cycle (default: 10).
     * @param offset Offset value. Default is 0.
     */
    STC(int short_length = 23, int long_length = 50, int cycle_length = 10, int offset = 0);

    /**
     * @brief Calculate the Schaff Trend Cycle (STC) values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated STC values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;

private:
    int short_length; // The period for the shorter-term EMA.
    int long_length;  // The period for the longer-term EMA.
    int cycle_length; // The length of the cycle.

    /**
     * @brief Calculate the Stochastic Oscillator values.
     *
     * @param values Vector containing the values to calculate the Stochastic Oscillator.
     * @param period Period for the Stochastic Oscillator calculation.
     * @return std::vector<double> Vector containing the calculated Stochastic Oscillator values.
     */
    std::vector<double> calculate_stochastic_oscillator(const std::vector<double> &values, int period) const;
};

// *********************************************************************************************

/**
 * @brief Class to calculate the TRIX indicator.
 *
 * TRIX is a momentum oscillator that displays the percent rate of change of a triple exponentially smoothed moving average.
 */
class TRIX : public Indicator
{
private:
    int period;

public:
    /**
     * @brief Construct a new TRIX object.
     *
     * @param period Period for TRIX calculation. Default is 15.
     * @param offset Offset value. Default is 0.
     */
    TRIX(int period = 15, int offset = 0);

    /**
     * @brief Calculate the TRIX values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;
};

// *********************************************************************************************

class InstitutionalBias : public Indicator
{
private:
    size_t short_period;
    size_t long_period;

public:
    /**
     * @brief Construct a new InstitutionalBias object.
     *
     * @param short_period Period for short EMA calculation.
     * @param long_period Period for long EMA calculation.
     * @param offset Offset value. Default is 0.
     */
    InstitutionalBias(int short_period = 9, int long_period = 18, int offset = 0);

    /**
     * @brief Calculate the Institutional Bias values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values. 1 for short EMA > long EMA, -1 otherwise.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;
};

// *********************************************************************************************

class EMADifference : public Indicator
{
private:
    int short_period;
    int long_period;

public:
    /**
     * @brief Construct a new EMADifference object.
     *
     * @param short_period Period for short EMA calculation. Default is 9.
     * @param long_period Period for long EMA calculation. Default is 18.
     * @param offset Offset value. Default is 0.
     */
    EMADifference(int short_period = 9, int long_period = 18, int offset = 0);

    /**
     * @brief Calculate the difference between two exponential moving averages.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing the calculated difference values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;
};

// *********************************************************************************************

/**
 * @brief Class to calculate the Ichimoku Cloud Trend indicator.
 *
 * The Ichimoku Cloud, also known as Ichimoku Kinko Hyo, is a versatile indicator that defines support and resistance, identifies trend direction, gauges momentum and provides trading signals.
 */
class IchimokuCloudTrend : public Indicator
{
public:
    /**
     * @brief Construct a new Ichimoku Cloud Trend object.
     *
     * @param conversion_period The period for the Conversion Line (Tenkan-sen).
     * @param base_period The period for the Base Line (Kijun-sen).
     * @param lagging_period The period for the Lagging Span (Chikou Span).
     * @param leading_period The period for the Leading Span (Senkou Span).
     * @param offset Offset value. Default is 0.
     */
    IchimokuCloudTrend(int conversion_period = 9, int base_period = 26, int lagging_period = 26, int leading_period = 52, int offset = 0);

    /**
     * @brief Calculate the Ichimoku Cloud Trend values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated Ichimoku Cloud Trend values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;

private:
    int conversion_period; // The period for the Conversion Line (Tenkan-sen).
    int base_period;       // The period for the Base Line (Kijun-sen).
    int lagging_period;    // The period for the Lagging Span (Chikou Span).
    int leading_period;    // The period for the Leading Span (Senkou Span).
};

// *********************************************************************************************

/**
 * @brief Class to calculate the Ichimoku Kijun Tenkan Trend indicator.
 *
 * The Ichimoku Cloud, also known as Ichimoku Kinko Hyo, is a versatile indicator that defines support and resistance, identifies trend direction, gauges momentum and provides trading signals.
 */
class IchimokuKijunTenkanTrend : public Indicator
{
public:
    /**
     * @brief Construct a new Ichimoku Kijun Tenkan Trend object.
     *
     * @param conversion_period The period for the Conversion Line (Tenkan-sen).
     * @param base_period The period for the Base Line (Kijun-sen).
     * @param lagging_period The period for the Lagging Span (Chikou Span).
     * @param leading_period The period for the Leading Span (Senkou Span).
     * @param offset Offset value. Default is 0.
     */
    IchimokuKijunTenkanTrend(int conversion_period = 9, int base_period = 26, int lagging_period = 26, int leading_period = 52, int offset = 0);

    /**
     * @brief Calculate the Ichimoku Kijun Tenkan Trend values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated Ichimoku Kijun Tenkan Trend values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;

private:
    int conversion_period; // The period for the Conversion Line (Tenkan-sen).
    int base_period;       // The period for the Base Line (Kijun-sen).
    int lagging_period;    // The period for the Lagging Span (Chikou Span).
    int leading_period;    // The period for the Leading Span (Senkou Span).
};

// *********************************************************************************************

class SMASlope : public Indicator
{
public:
    /**
     * @brief Construct a new Simple Moving Average Slope object.
     *
     * @param period The period for the simple moving average.
     * @param source The source of data (e.g., close, high, low).
     * @param offset Offset value. Default is 0.
     */
    SMASlope(int period, CandleSource source, int offset = 0);

    /**
     * @brief Calculate the slope of the simple moving average.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated slope values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;

private:
    int period;          // The period for the simple moving average.
    CandleSource source; // The source of data (e.g., close, high, low).
};

// *********************************************************************************************

class EMASlope : public Indicator
{
public:
    /**
     * @brief Construct a new Exponential Moving Average Slope object.
     *
     * @param period The period for the exponential moving average.
     * @param source The source of data (e.g., close, high, low).
     * @param offset Offset value. Default is 0.
     */
    EMASlope(int period, CandleSource source, int offset = 0);

    /**
     * @brief Calculate the slope of the exponential moving average.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated slope values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;

private:
    int period;          // The period for the exponential moving average.
    CandleSource source; // The source of data (e.g., close, high, low).
};

#endif // TREND_INDICATORS_HPP

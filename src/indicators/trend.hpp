#ifndef TREND_INDICATORS_HPP
#define TREND_INDICATORS_HPP

#include <vector>
#include <stdexcept>
#include <functional>
#include <unordered_map>
#include <string>
#include <iostream>
#include "../types.hpp"
#include "../utils/math.hpp"
#include "../utils/candles_source.hpp"
#include "momentum.hpp"
#include "indicator.hpp"

/**
 * Average Directional Index (ADX)
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
     * @param period Period value. Default is 14.
     * @param offset Offset value. Default is 0.
     */
    ADX(int period = 14, int offset = 0);

    /**
     * @brief Calculate the Average Directional Movement Index values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;

private:
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
 * Aroon Up Indicator
 *
 * The Aroon indicator is used to determine the strength of a trend and the likelihood that it will continue. It is made up of two lines: one line is called "Aroon up", which measures the strength of the uptrend, and the other line is called "Aroon down", which measures the strength of the downtrend.
 */
class AroonUp : public Indicator
{
public:
    /**
     * @brief Construct a new Aroon Up object.
     *
     * @param period Period value. Default is 14.
     * @param offset Offset value. Default is 0.
     */
    AroonUp(int period = 14, int offset = 0);

    /**
     * @brief Calculate the Aroon Up values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;
};

// *********************************************************************************************

/**
 * Aroon Down Indicator
 *
 * The Aroon indicator is used to determine the strength of a trend and the likelihood that it will continue. It is made up of two lines: one line is called "Aroon up", which measures the strength of the uptrend, and the other line is called "Aroon down", which measures the strength of the downtrend.
 */
class AroonDown : public Indicator
{
public:
    /**
     * @brief Construct a new Aroon Down object.
     *
     * @param period Period value. Default is 14.
     * @param offset Offset value. Default is 0.
     */
    AroonDown(int period = 14, int offset = 0);

    /**
     * @brief Calculate the Aroon Down values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;
};

/**
 * Commodity Channel Index (CCI)
 *
 * CCI measures the difference between a security�s price change and its average price change. High positive readings indicate that prices are well above their average, which is a show of strength. Low negative readings indicate that prices are well below their average, which is a show of weakness.
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
     * @param source Candle source. Default is "close".
     * @param period Period value. Default is 20.
     * @param offset Offset value. Default is 0.
     */
    EMA(std::string source = "close", int period = 20, int offset = 0);

    /**
     * @brief Calculate the EMA values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;
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
};

// *********************************************************************************************

/**
 * MACD (Moving Average Convergence Divergence)
 *
 * Is a trend-following momentum indicator that shows the relationship between two moving averages of a security�s price.
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
    SMA(std::string source, int period, int offset = 0);

    /**
     * @brief Calculate the Simple Moving Average (SMA).
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing the calculated SMA values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;
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

class Vortex : public Indicator
{
public:
    /**
     * @brief Construct a new Vortex Indicator object.
     *
     * @param period The period for calculating Vortex Indicator (default: 14).
     * @param offset Offset value. Default is 0.
     */
    Vortex(int period = 14, int offset = 0);

    /**
     * @brief Calculate the Vortex Indicator values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated Vortex Indicator values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;
};

// *********************************************************************************************

class InstitutionalBias : public Indicator
{
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
};

// *********************************************************************************************

/**
 * @brief Class to calculate the Ichimoku Senkou Span Trend indicator.
 *
 * Calculate the slope of the simple moving average.
 */
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
    SMASlope(int period, std::string source, int offset = 0);

    /**
     * @brief Calculate the slope of the simple moving average.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated slope values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;
};

// *********************************************************************************************

/**
 * @brief Class to calculate the EMA Slope indicator.
 *
 * Calculate the slope of the exponential moving average.
 */
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
    EMASlope(int period, std::string source, int offset = 0);

    /**
     * @brief Calculate the slope of the exponential moving average.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated slope values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;
};

// *********************************************************************************************

/**
 * @brief Class to calculate the Zigzag indicator.
 *
 * The Zigzag indicator is used to eliminate movements that are less than a specified percentage.
 */
class Zigzag : public Indicator
{
public:
    /**
     * @brief Construct a new Zigzag object.
     *
     * @param deviation The deviation value for the Zigzag calculation.
     * @param offset Offset value. Default is 0.
     */
    Zigzag(double deviation = 0.05, int offset = 0);

    /**
     * @brief Calculate the Zigzag values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated Zigzag values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;
};

const std::unordered_map<std::string, std::function<Indicator *(std::unordered_map<std::string, IndicatorParam>)>> trend_indicators_map = {
    {"adx", [](std::unordered_map<std::string, IndicatorParam> params) -> Indicator *
     {
         try
         {
             if (check_params(params, {{"period", typeid(int)}, {"offset", typeid(int)}}))
             {
                 int period = std::get<int>(params["period"]);
                 int offset = std::get<int>(params["offset"]);
                 return new ADX(period, offset);
             }
         }
         catch (const std::exception &e)
         {
             std::cerr << "Error creating ADX: " << e.what() << std::endl;
             std::exit(1);
         }

         return nullptr;
     }},
    {"aroon-up", [](std::unordered_map<std::string, IndicatorParam> params) -> Indicator *
     {
         try
         {
             if (check_params(params, {{"period", typeid(int)}, {"offset", typeid(int)}}))
             {
                 int period = std::get<int>(params["period"]);
                 int offset = std::get<int>(params["offset"]);
                 return new AroonUp(period, offset);
             }
         }
         catch (const std::exception &e)
         {
             std::cerr << "Error creating AroonUp: " << e.what() << std::endl;
             std::exit(1);
         }

         return nullptr;
     }},
    {"aroon-down", [](std::unordered_map<std::string, IndicatorParam> params) -> Indicator *
     {
         try
         {
             if (check_params(params, {{"period", typeid(int)}, {"offset", typeid(int)}}))
             {
                 int period = std::get<int>(params["period"]);
                 int offset = std::get<int>(params["offset"]);
                 return new AroonDown(period, offset);
             }
         }
         catch (const std::exception &e)
         {
             std::cerr << "Error creating AroonDown: " << e.what() << std::endl;
             std::exit(1);
         }

         return nullptr;
     }},
    {"cci", [](std::unordered_map<std::string, IndicatorParam> params) -> Indicator *
     {
         try
         {
             if (check_params(params, {{"period", typeid(int)}, {"offset", typeid(int)}}))
             {
                 int period = std::get<int>(params["period"]);
                 int offset = std::get<int>(params["offset"]);
                 return new CCI(period, offset);
             }
         }
         catch (const std::exception &e)
         {
             std::cerr << "Error creating CCI: " << e.what() << std::endl;
             std::exit(1);
         }

         return nullptr;
     }},
    {"dpo", [](std::unordered_map<std::string, IndicatorParam> params) -> Indicator *
     {
         try
         {
             if (check_params(params, {{"period", typeid(int)}, {"offset", typeid(int)}}))
             {
                 int period = std::get<int>(params["period"]);
                 int offset = std::get<int>(params["offset"]);
                 return new DPO(period, offset);
             }
         }
         catch (const std::exception &e)
         {
             std::cerr << "Error creating DPO: " << e.what() << std::endl;
             std::exit(1);
         }

         return nullptr;
     }},
    {"ema", [](std::unordered_map<std::string, IndicatorParam> params) -> Indicator *
     {
         try
         {
             if (check_params(params, {{"source", typeid(std::string)}, {"period", typeid(int)}, {"offset", typeid(int)}}))
             {
                 std::string source = std::get<std::string>(params["source"]);
                 int period = std::get<int>(params["period"]);
                 int offset = std::get<int>(params["offset"]);
                 return new EMA(source, period, offset);
             }
         }
         catch (const std::exception &e)
         {
             std::cerr << "Error creating EMA: " << e.what() << std::endl;
             std::exit(1);
         }

         return nullptr;
     }},
    {"kst", [](std::unordered_map<std::string, IndicatorParam> params) -> Indicator *
     {
         try
         {
             if (check_params(params, {{"roc_periods1", typeid(int)}, {"roc_periods2", typeid(int)}, {"roc_periods3", typeid(int)}, {"roc_periods4", typeid(int)}, {"sma_periods1", typeid(int)}, {"sma_periods2", typeid(int)}, {"sma_periods3", typeid(int)}, {"sma_periods4", typeid(int)}, {"signal_periods", typeid(int)}, {"offset", typeid(int)}}))
             {
                 int roc_periods1 = std::get<int>(params["roc_periods1"]);
                 int roc_periods2 = std::get<int>(params["roc_periods2"]);
                 int roc_periods3 = std::get<int>(params["roc_periods3"]);
                 int roc_periods4 = std::get<int>(params["roc_periods4"]);
                 int sma_periods1 = std::get<int>(params["sma_periods1"]);
                 int sma_periods2 = std::get<int>(params["sma_periods2"]);
                 int sma_periods3 = std::get<int>(params["sma_periods3"]);
                 int sma_periods4 = std::get<int>(params["sma_periods4"]);
                 int signal_periods = std::get<int>(params["signal_periods"]);
                 int offset = std::get<int>(params["offset"]);
                 return new KST(roc_periods1, roc_periods2, roc_periods3, roc_periods4, sma_periods1, sma_periods2, sma_periods3, sma_periods4, signal_periods, offset);
             }
         }
         catch (const std::exception &e)
         {
             std::cerr << "Error creating KST: " << e.what() << std::endl;
             std::exit(1);
         }

         return nullptr;
     }},
    {"macd", [](std::unordered_map<std::string, IndicatorParam> params) -> Indicator *
     {
         try
         {
             if (check_params(params, {{"short_period", typeid(int)}, {"long_period", typeid(int)}, {"signal_period", typeid(int)}, {"offset", typeid(int)}}))
             {
                 int short_period = std::get<int>(params["short_period"]);
                 int long_period = std::get<int>(params["long_period"]);
                 int signal_period = std::get<int>(params["signal_period"]);
                 int offset = std::get<int>(params["offset"]);
                 return new MACD(short_period, long_period, signal_period, offset);
             }
         }
         catch (const std::exception &e)
         {
             std::cerr << "Error creating MACD: " << e.what() << std::endl;
             std::exit(1);
         }

         return nullptr;
     }},
    {"mi", [](std::unordered_map<std::string, IndicatorParam> params) -> Indicator *
     {
         try
         {
             if (check_params(params, {{"offset", typeid(int)}}))
             {
                 int offset = std::get<int>(params["offset"]);
                 return new MI(offset);
             }
         }
         catch (const std::exception &e)
         {
             std::cerr << "Error creating MI: " << e.what() << std::endl;
             std::exit(1);
         }

         return nullptr;
     }},
    {"psar", [](std::unordered_map<std::string, IndicatorParam> params) -> Indicator *
     {
         try
         {
             if (check_params(params, {{"acceleration_factor_initial", typeid(double)}, {"acceleration_factor_maximum", typeid(double)}, {"offset", typeid(int)}}))
             {
                 double acceleration_factor_initial = std::get<double>(params["acceleration_factor_initial"]);
                 double acceleration_factor_maximum = std::get<double>(params["acceleration_factor_maximum"]);
                 int offset = std::get<int>(params["offset"]);
                 return new ParabolicSAR(acceleration_factor_initial, acceleration_factor_maximum, offset);
             }
         }
         catch (const std::exception &e)
         {
             std::cerr << "Error creating ParabolicSAR: " << e.what() << std::endl;
             std::exit(1);
         }

         return nullptr;
     }},
    {"sma", [](std::unordered_map<std::string, IndicatorParam> params) -> Indicator *
     {
         try
         {
             if (check_params(params, {{"source", typeid(std::string)}, {"period", typeid(int)}, {"offset", typeid(int)}}))
             {
                 std::string source = std::get<std::string>(params["source"]);
                 int period = std::get<int>(params["period"]);
                 int offset = std::get<int>(params["offset"]);
                 return new SMA(source, period, offset);
             }
         }
         catch (const std::exception &e)
         {
             std::cerr << "Error creating SMA: " << e.what() << std::endl;
             std::exit(1);
         }

         return nullptr;
     }},
    {"stc", [](std::unordered_map<std::string, IndicatorParam> params) -> Indicator *
     {
         try
         {
             if (check_params(params, {{"short_length", typeid(int)}, {"long_length", typeid(int)}, {"cycle_length", typeid(int)}, {"offset", typeid(int)}}))
             {
                 int short_length = std::get<int>(params["short_length"]);
                 int long_length = std::get<int>(params["long_length"]);
                 int cycle_length = std::get<int>(params["cycle_length"]);
                 int offset = std::get<int>(params["offset"]);
                 return new STC(short_length, long_length, cycle_length, offset);
             }
         }
         catch (const std::exception &e)
         {
             std::cerr << "Error creating STC: " << e.what() << std::endl;
             std::exit(1);
         }

         return nullptr;
     }},
    {"trix", [](std::unordered_map<std::string, IndicatorParam> params) -> Indicator *
     {
         try
         {
             if (check_params(params, {{"period", typeid(int)}, {"offset", typeid(int)}}))
             {
                 int period = std::get<int>(params["period"]);
                 int offset = std::get<int>(params["offset"]);
                 return new TRIX(period, offset);
             }
         }
         catch (const std::exception &e)
         {
             std::cerr << "Error creating TRIX: " << e.what() << std::endl;
             std::exit(1);
         }

         return nullptr;
     }},
    {"vortex", [](std::unordered_map<std::string, IndicatorParam> params) -> Indicator *
     {
         try
         {
             if (check_params(params, {{"period", typeid(int)}, {"offset", typeid(int)}}))
             {
                 int period = std::get<int>(params["period"]);
                 int offset = std::get<int>(params["offset"]);
                 return new Vortex(period, offset);
             }
         }
         catch (const std::exception &e)
         {
             std::cerr << "Error creating Vortex: " << e.what() << std::endl;
             std::exit(1);
         }

         return nullptr;
     }},
    {"institutional-bias", [](std::unordered_map<std::string, IndicatorParam> params) -> Indicator *
     {
         try
         {
             if (check_params(params, {{"short_period", typeid(int)}, {"long_period", typeid(int)}, {"offset", typeid(int)}}))
             {
                 int short_period = std::get<int>(params["short_period"]);
                 int long_period = std::get<int>(params["long_period"]);
                 int offset = std::get<int>(params["offset"]);
                 return new InstitutionalBias(short_period, long_period, offset);
             }
         }
         catch (const std::exception &e)
         {
             std::cerr << "Error creating InstitutionalBias: " << e.what() << std::endl;
             std::exit(1);
         }

         return nullptr;
     }},
    {"ema-difference", [](std::unordered_map<std::string, IndicatorParam> params) -> Indicator *
     {
         try
         {
             if (check_params(params, {{"short_period", typeid(int)}, {"long_period", typeid(int)}, {"offset", typeid(int)}}))
             {
                 int short_period = std::get<int>(params["short_period"]);
                 int long_period = std::get<int>(params["long_period"]);
                 int offset = std::get<int>(params["offset"]);
                 return new EMADifference(short_period, long_period, offset);
             }
         }
         catch (const std::exception &e)
         {
             std::cerr << "Error creating EMADifference: " << e.what() << std::endl;
             std::exit(1);
         }

         return nullptr;
     }},
    {"aroon-trend", [](std::unordered_map<std::string, IndicatorParam> params) -> Indicator *
     {
         try
         {
             if (check_params(params, {{"period", typeid(int)}, {"offset", typeid(int)}}))
             {
                 int period = std::get<int>(params["period"]);
                 int offset = std::get<int>(params["offset"]);
                 return new AroonTrend(period, offset);
             }
         }
         catch (const std::exception &e)
         {
             std::cerr << "Error creating AroonTrend: " << e.what() << std::endl;
             std::exit(1);
         }

         return nullptr;
     }},
    {"ichimoku-cloud-trend", [](std::unordered_map<std::string, IndicatorParam> params) -> Indicator *
     {
         try
         {
             if (check_params(params, {{"conversion_period", typeid(int)}, {"base_period", typeid(int)}, {"lagging_period", typeid(int)}, {"leading_period", typeid(int)}, {"offset", typeid(int)}}))
             {
                 int conversion_period = std::get<int>(params["conversion_period"]);
                 int base_period = std::get<int>(params["base_period"]);
                 int lagging_period = std::get<int>(params["lagging_period"]);
                 int leading_period = std::get<int>(params["leading_period"]);
                 int offset = std::get<int>(params["offset"]);
                 return new IchimokuCloudTrend(conversion_period, base_period, lagging_period, leading_period, offset);
             }
         }
         catch (const std::exception &e)
         {
             std::cerr << "Error creating IchimokuCloudTrend: " << e.what() << std::endl;
             std::exit(1);
         }

         return nullptr;
     }},
    {"ichimoku-kijun-tenkan-trend", [](std::unordered_map<std::string, IndicatorParam> params) -> Indicator *
     {
         try
         {
             if (check_params(params, {{"conversion_period", typeid(int)}, {"base_period", typeid(int)}, {"lagging_period", typeid(int)}, {"leading_period", typeid(int)}, {"offset", typeid(int)}}))
             {
                 int conversion_period = std::get<int>(params["conversion_period"]);
                 int base_period = std::get<int>(params["base_period"]);
                 int lagging_period = std::get<int>(params["lagging_period"]);
                 int leading_period = std::get<int>(params["leading_period"]);
                 int offset = std::get<int>(params["offset"]);
                 return new IchimokuKijunTenkanTrend(conversion_period, base_period, lagging_period, leading_period, offset);
             }
         }
         catch (const std::exception &e)
         {
             std::cerr << "Error creating IchimokuKijunTenkanTrend: " << e.what() << std::endl;
             std::exit(1);
         }

         return nullptr;
     }},
    {"sma-slope", [](std::unordered_map<std::string, IndicatorParam> params) -> Indicator *
     {
         try
         {
             if (check_params(params, {{"period", typeid(int)}, {"source", typeid(std::string)}, {"offset", typeid(int)}}))
             {
                 int period = std::get<int>(params["period"]);
                 std::string source = std::get<std::string>(params["source"]);
                 int offset = std::get<int>(params["offset"]);
                 return new SMASlope(period, source, offset);
             }
         }
         catch (const std::exception &e)
         {
             std::cerr << "Error creating SMASlope: " << e.what() << std::endl;
             std::exit(1);
         }

         return nullptr;
     }},
    {"ema-slope", [](std::unordered_map<std::string, IndicatorParam> params) -> Indicator *
     {
         try
         {
             if (check_params(params, {{"period", typeid(int)}, {"source", typeid(std::string)}, {"offset", typeid(int)}}))
             {
                 int period = std::get<int>(params["period"]);
                 std::string source = std::get<std::string>(params["source"]);
                 int offset = std::get<int>(params["offset"]);
                 return new EMASlope(period, source, offset);
             }
         }
         catch (const std::exception &e)
         {
             std::cerr << "Error creating EMASlope: " << e.what() << std::endl;
             std::exit(1);
         }

         return nullptr;
     }},
    {"zigzag", [](std::unordered_map<std::string, IndicatorParam> params) -> Indicator *
     {
         try
         {
             if (check_params(params, {{"deviation", typeid(double)}, {"offset", typeid(int)}}))
             {
                 double deviation = std::get<double>(params["deviation"]);
                 int offset = std::get<int>(params["offset"]);
                 return new Zigzag(deviation, offset);
             }
         }
         catch (const std::exception &e)
         {
             std::cerr << "Error creating Zigzag: " << e.what() << std::endl;
             std::exit(1);
         }

         return nullptr;
     }},

};

#endif // TREND_INDICATORS_HPP

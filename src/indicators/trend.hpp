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
    int period;

    std::vector<double> smooth_directional_movement(const std::vector<double> &directional_movement) const;
    std::vector<double> smooth_true_range(const std::vector<double> &true_range_values) const;
    std::vector<double> calculate_positive_directional_index(const std::vector<double> &smoothed_directional_movement_plus, const std::vector<double> &smoothed_true_range) const;
    std::vector<double> calculate_negative_directional_index(const std::vector<double> &smoothed_directional_movement_minus, const std::vector<double> &smoothed_true_range) const;
    std::vector<double> calculate_directional_movement_index(const std::vector<double> &positive_directional_index, const std::vector<double> &negative_directional_index) const;
    std::vector<double> calculate_adx(const std::vector<double> &directional_movement_index) const;
};

// *********************************************************************************************

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
 * KST Oscillator (KST Signal)
 *
 * It is useful to identify major stock market cycle junctures because its formula is weighed to be more greatly influenced by the longer and more dominant time spans, in order to better reflect the primary swings of stock market cycle.
 */
class KSTOscillator : public Indicator
{
public:
    /**
     * @brief Construct a new KST Oscillator object.
     *
     * @param roc_periods1 Periods for Rate of Change (ROC) 1.
     * @param roc_periods2 Periods for Rate of Change (ROC) 2.
     * @param roc_periods3 Periods for Rate of Change (ROC) 3.
     * @param roc_periods4 Periods for Rate of Change (ROC) 4.
     * @param sma_periods1 Periods for Simple Moving Average (SMA) 1.
     * @param sma_periods2 Periods for Simple Moving Average (SMA) 2.
     * @param sma_periods3 Periods for Simple Moving Average (SMA) 3.
     * @param sma_periods4 Periods for Simple Moving Average (SMA) 4.
     * @param signal_periods Periods for the signal line.
     * @param offset Offset value. Default is 0.
     */
    KSTOscillator(int roc_periods1, int roc_periods2, int roc_periods3, int roc_periods4,
                  int sma_periods1, int sma_periods2, int sma_periods3, int sma_periods4,
                  int signal_periods, int offset = 0);

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
 */
class PSAR : public Indicator
{

public:
    /**
     * @brief Construct a new ParabolicSAR object.
     *
     * @param acceleration_factor Initial acceleration factor value. Default is 0.02.
     * @param acceleration_factor_max Maximum acceleration factor value. Default is 0.2.
     * @param offset Offset value. Default is 0.
     */
    PSAR(double acceleration_factor = 0.02, double acceleration_factor_max = 0.2, int offset = 0);

    /**
     * @brief Calculate the Parabolic SAR values.
     *
     * @param candles Vector of Candle data.
     * @return std::vector<double> Vector containing the Parabolic SAR values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;

private:
    double acceleration_factor;
    double acceleration_factor_max;
};

// *********************************************************************************************

/**
 * @brief Class to calculate the Simple Moving Average (SMA) indicator.
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
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data) const override;

private:
    int period; // Period for the SMA calculation
};

#endif // TREND_INDICATORS_HPP

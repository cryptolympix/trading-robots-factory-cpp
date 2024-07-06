#ifndef MOMENTUM_INDICATORS_HPP
#define MOMENTUM_INDICATORS_HPP

#include <variant>
#include <map>
#include <functional>
#include <vector>
#include <string>
#include "../types.hpp"
#include "indicator.hpp"

/**
 * Awesome Oscillator
 *
 * From: https://www.tradingview.com/wiki/Awesome_Oscillator_(AO)
 *
 * The Awesome Oscillator is an indicator used to measure market momentum. AO calculates the difference of a 34 Period and 5 Period Simple Moving Averages. The Simple Moving Averages that are used are not calculated using closing price but rather each bar�s midpoints. AO is generally used to affirm trends or to anticipate possible reversals.
 *
 * From: https://www.ifcm.co.uk/ntx-indicators/awesome-oscillator
 *
 * Awesome Oscillator is a 34-period simple moving average, plotted through the central points of the bars (H+L)/2, and subtracted from the 5-period simple moving average, graphed across the central points of the bars (H+L)/2.
 *
 * MEDIAN PRICE = (HIGH+LOW)/2
 *
 * AO = SMA(MEDIAN PRICE, 5)-SMA(MEDIAN PRICE, 34)
 *
 * where
 *
 * SMA � Simple Moving Average.
 */
class AwesomeOscillator : public Indicator
{
public:
    std::string label = "Awesome Oscillator";
    std::string id = "awesome-oscillator";
    std::string id_pattern = R"(awesome-oscillator-(\d+))";

    /**
     * @brief Construct a new AwesomeOscillator object.
     *
     * @param offset Offset value. Default is 0.
     */
    AwesomeOscillator(int offset = 0);

    /**
     * @brief Calculate the AwesomeOscillator values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;
};

// *********************************************************************************************

/**
 * @brief Class to calculate Kaufman's Adaptive Moving Average (KAMA) indicator.
 */
class KAMA : public Indicator
{
private:
    int er_period;         // Period for the Efficiency Ratio (ER)
    int fastest_sc_period; // Period for the fastest Smoothing Constant (SC)
    int slowest_sc_period; // Period for the slowest Smoothing Constant (SC)

public:
    std::string label = "Kaufman's Adaptive Moving Average";
    std::string id = "kama";
    std::string id_pattern = R"(kama-(\d+)-(\d+)-(\d+)-(\d+))";

    /**
     * @brief Construct a new KAMA object.
     *
     * @param er_period Period for the Efficiency Ratio (ER). Default is 10.
     * @param fastest_sc_period Period for the fastest Smoothing Constant (SC). Default is 2.
     * @param slowest_sc_period Period for the slowest Smoothing Constant (SC). Default is 30.
     * @param offset Offset value. Default is 0.
     */
    KAMA(int er_period = 10, int fastest_sc_period = 2, int slowest_sc_period = 30, int offset = 0);

    /**
     * @brief Calculate Kaufman's Adaptive Moving Average (KAMA).
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing the calculated KAMA values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;

private:
    /**
     * @brief Calculate the Efficiency Ratio (ER).
     *
     * @param closes Vector of closing prices.
     * @return std::vector<double> Vector containing the calculated ER values.
     */
    std::vector<double> calculate_er(const std::vector<double> &closes) const;

    /**
     * @brief Calculate the Smoothing Constant (SC).
     *
     * @param er_values Vector of Efficiency Ratio (ER) values.
     * @return std::vector<double> Vector containing the calculated SC values.
     */
    std::vector<double> calculate_sc(const std::vector<double> &er_values) const;

    /**
     * @brief Calculate the initial KAMA (Simple Moving Average).
     *
     * @param closes Vector of closing prices.
     * @return double Initial KAMA value.
     */
    double calculate_initial_kama(const std::vector<double> &closes) const;
};

// *********************************************************************************************

/**
 * MFI (Money Flow Index)
 *
 * Is a momentum indicator that measures the inflow and outflow of money into an asset over a specific period of time.
 */
class MFI : public Indicator
{
public:
    std::string label = "Money Flow Index";
    std::string id = "mfi";
    std::string id_pattern = R"(mfi-(\d+)-(\d+))";

    /**
     * @brief Construct a new Money Flow Index object.
     *
     * @param period The period for calculating Money Flow Index (MFI). Default is 14.
     * @param offset Offset value. Default is 0.
     */
    MFI(int period = 14, int offset = 0);

    /**
     * @brief Calculate the Money Flow Index (MFI) values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated MFI values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;

private:
    int period; // The period for calculating Money Flow Index (MFI).
};

// *********************************************************************************************

/**
 * The Percentage Price Oscillator (PPO) is a momentum oscillator that measures the difference between two moving averages as a percentage of the larger moving average.
 *
 * https://school.stockcharts.com/doku.php?id=technical_indicators:price_oscillators_ppo
 */
class PPO : public Indicator
{
public:
    std::string label = "Percentage Price Oscillator";
    std::string id = "ppo";
    std::string id_pattern = R"(ppo-(\d+)-(\d+)-(\d+))";

    /**
     * @brief Construct a new PPO object with a specified short period, long period, and offset.
     *
     * @param short_period The short period for calculating PPO (default is 12).
     * @param long_period The long period for calculating PPO (default is 26).
     * @param offset The offset value (default is 0).
     */
    PPO(int short_period = 12, int long_period = 26, int offset = 0);

    /**
     * @brief Calculate the Price Oscillator (PPO) for a given set of candles.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated PPO values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;

private:
    int short_period; // The short period for calculating PPO.
    int long_period;  // The long period for calculating PPO.
};

// *********************************************************************************************

/**
 * The Percentage Volume Oscillator (PVO) is a momentum oscillator for volume. The PVO measures the difference between two volume-based moving averages as a percentage of the larger moving average.
 *
 * https://school.stockcharts.com/doku.php?id=technical_indicators:percentage_volume_oscillator_pvo
 */
class PVO : public Indicator
{
public:
    std::string label = "Percentage Volume Oscillator";
    std::string id = "pvo";
    std::string id_pattern = R"(pvo-(\d+)-(\d+)-(\d+)-(\d+))";

    /**
     * @brief Construct a new Percentage Volume Oscillator (PVO) object.
     *
     * @param fast_period The fast period used for calculating the PVO. Default is 12.
     * @param slow_period The slow period used for calculating the PVO. Default is 26.
     * @param signal_period The signal period used for calculating the PVO signal line. Default is 9.
     * @param offset Offset value. Default is 0.
     */
    PVO(int fast_period = 12, int slow_period = 26, int signal_period = 9, int offset = 0);

    /**
     * @brief Calculate the Percentage Volume Oscillator (PVO) values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated PVO values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;

private:
    int fast_period;   // Fast period for calculating the PVO
    int slow_period;   // Slow period for calculating the PVO
    int signal_period; // Signal period for calculating the PVO signal line
};

// *********************************************************************************************

/**
 * Rate of Change (ROC)
 *
 * The Rate-of-Change (ROC) indicator, which is also referred to as simply Momentum, is a pure momentum oscillator that measures the percent change in price from one period to the next. The ROC calculation compares the current price with the price �n� periods ago. The plot forms an oscillator that fluctuates above and below the zero line as the Rate-of-Change moves from positive to negative. As a momentum oscillator, ROC signals include centerline crossovers, divergences and overbought-oversold readings. Divergences fail to foreshadow reversals more often than not, so this article will forgo a detailed discussion on them. Even though centerline crossovers are prone to whipsaw, especially short-term, these crossovers can be used to identify the overall trend. Identifying overbought or oversold extremes comes naturally to the Rate-of-Change oscillator.
 *
 * https://school.stockcharts.com/doku.php?id=technical_indicators:rate_of_change_roc_and_momentum
 */
class ROC : public Indicator
{
public:
    std::string label = "Rate of Change";
    std::string id = "roc";
    std::string id_pattern = R"(roc-(\d+)(\d+))";

    /**
     * @brief Construct a new ROC object.
     *
     * @param period Period value. Default is 9.
     * @param offset Offset value. Default is 0.
     */
    ROC(int period = 9, int offset = 0);

    /**
     * @brief Calculate the Rate of Change (ROC) values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;

private:
    int period; // The period for calculating the ROC.
};

// *********************************************************************************************

/**
 * Relative Strength Index (RSI)
 *
 * Compares the magnitude of recent gains and losses over a specified time period to measure speed and change of price movements of a security. It is primarily used to attempt to identify overbought or oversold conditions in the trading of an asset.
 *
 * https://www.investopedia.com/terms/r/rsi.asp
 */
class RSI : public Indicator
{
public:
    std::string label = "Relative Strength Index";
    std::string id = "rsi";
    std::string id_pattern = R"(rsi-(\d+)-(\d+))";

    /**
     * @brief Construct a new RSI object.
     *
     * @param period Period value. Default is 14.
     * @param offset Offset value. Default is 0.
     */
    RSI(int period = 14, int offset = 0);

    /**
     * @brief Calculate the Relative Strength Index (RSI) values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;

private:
    int period; // RSI period
};

// *********************************************************************************************

/**
 * Stochastic RSI
 *
 * The StochRSI oscillator was developed to take advantage of both momentum indicators in order to create a more sensitive indicator that is attuned to a specific security�s historical performance rather than a generalized analysis of price change.
 *
 * https://school.stockcharts.com/doku.php?id=technical_indicators:stochrsi https://www.investopedia.com/terms/s/stochrsi.asp
 */
class StochasticRSI : public Indicator
{
public:
    std::string label = "Stochastic RSI";
    std::string id = "stochastic-rsi";
    std::string id_pattern = R"(stochastic-rsi-(\d+)-(\d+)-(\d+))";

    /**
     * @brief Construct a new StochasticRSI object.
     *
     * @param period Period value. Default is 14.
     * @param sma_period SMA Period value. Default is 3.
     * @param offset Offset value. Default is 0.
     */
    StochasticRSI(int period = 14, int sma_period = 3, int offset = 0);

    /**
     * @brief Calculate the Stochastic Relative Strength Index (Stoch RSI) values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;

private:
    int period;     // Period for calculating the RSI values
    int sma_period; // Period for calculating the SMA of the RSI values
};

// *********************************************************************************************

/**
 * Stochastic Oscillator
 *
 * Developed in the late 1950s by George Lane. The stochastic oscillator presents the location of the closing price of a stock in relation to the high and low range of the price of a stock over a period of time, typically a 14-day period.
 *
 * https://school.stockcharts.com/doku.php?id=technical_indicators:stochastic_oscillator_fast_slow_and_full
 */
class StochasticOscillator : public Indicator
{
public:
    std::string label = "Stochastic Oscillator";
    std::string id = "stochastic-oscillator";
    std::string id_pattern = R"(stochastic-oscillator-(\d+)-(\d+))";

    /**
     * @brief Construct a new StochasticOscillator object.
     *
     * @param k_period K Period value. Default is 14.
     * @param d_period D Period value. Default is 3.
     * @param offset Offset value. Default is 0.
     */
    StochasticOscillator(int k_period = 14, int d_period = 3, int offset = 0);

    /**
     * @brief Calculate the Stochastic Oscillator values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;

private:
    int k_period; // K Period value
    int d_period; // D Period value
};

// *********************************************************************************************

/**
 * True strength index (TSI)
 *
 * Shows both trend direction and overbought/oversold conditions.
 *
 * https://school.stockcharts.com/doku.php?id=technical_indicators:true_strength_index
 */
class TSI : public Indicator
{
public:
    std::string label = "True Strength Index";
    std::string id = "tsi";
    std::string id_pattern = R"(tsi-(\d+)-(\d+)-(\d+))";

    /**
     * @brief Construct a new TSI object.
     *
     * @param short_period Short period value. Default is 13.
     * @param long_period Long period value. Default is 25.
     * @param offset Offset value. Default is 0.
     */
    TSI(int short_period = 13, int long_period = 25, int offset = 0);

    /**
     * @brief Calculate the TSI values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;

private:
    int short_period;
    int long_period;
};

// *********************************************************************************************

/**
 * Ultimate Oscillator
 *
 * Developed by Larry Williams in 1976 and described in Stocks & Commodities Magazine in 1985. It is a momentum oscillator that reflects the overall tendency of three timeframes.
 *
 * https://school.stockcharts.com/doku.php?id=technical_indicators:ultimate_oscillator
 */
class UO : public Indicator
{
public:
    std::string label = "Ultimate Oscillator";
    std::string id = "uo";
    std::string id_pattern = R"(uo-(\d+)-(\d+)-(\d+)-(\d+))";

    /**
     * @brief Construct a new Ultimate Oscillator object.
     *
     * @param period1 Period value for the first time frame. Default is 7.
     * @param period2 Period value for the second time frame. Default is 14.
     * @param period3 Period value for the third time frame. Default is 28.
     * @param offset Offset value. Default is 0.
     */
    UO(int period1 = 7, int period2 = 14, int period3 = 28, int offset = 0);

    /**
     * @brief Calculate the Ultimate Oscillator values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;

private:
    int period1;
    int period2;
    int period3;
};

// *********************************************************************************************

/**
 * Williams %R
 *
 * Developed by Larry Williams, Williams %R is a momentum indicator that is the inverse of the Fast Stochastic Oscillator. Also referred to as %R, Williams %R reflects the level of the close relative to the highest high for the look-back period. In contrast, the Stochastic Oscillator reflects the level of the close relative to the lowest low. %R corrects for the inversion by multiplying the raw value by -100. As a result, the Fast Stochastic Oscillator and Williams %R produce the exact same lines, only the scaling is different. Williams %R oscillates from 0 to -100.
 *
 * Readings from 0 to -20 are considered overbought. Readings from -80 to -100 are considered oversold.
 *
 * Unsurprisingly, signals derived from the Stochastic Oscillator are also applicable to Williams %R.
 *
 * %R = (Highest High - Close)/(Highest High - Lowest Low) * -100
 *
 * Lowest Low = lowest low for the look-back period Highest High = highest high for the look-back period %R is multiplied by -100 correct the inversion and move the decimal.
 *
 * https://school.stockcharts.com/doku.php?id=technical_indicators:williams_r
 *
 * The Williams %R oscillates from 0 to -100. When the indicator produces readings from 0 to -20, this indicates overbought market conditions. When readings are -80 to -100, it indicates oversold market conditions.
 */
class WPR : public Indicator
{
public:
    std::string label = "Williams Percent R";
    std::string id = "wpr";
    std::string id_pattern = R"(wpr-(\d+)-(\d+))";

    /**
     * @brief Construct a new Williams Percent R object.
     *
     * @param period The period for calculating Williams %R. Default to 14.
     * @param offset Offset value. Default is 0.
     */
    WPR(int period = 14, int offset = 0);

    /**
     * @brief Calculate the Williams Percent R values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;

private:
    int period; // The period for calculating Williams %R.
};

#endif /* MOMENTUM_INDICATORS_HPP */
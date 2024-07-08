#ifndef MOMENTUM_SIGNALS_INDICATORS_H
#define MOMENTUM_SIGNALS_INDICATORS_H

#include <vector>
#include "indicator.hpp"

class AwesomeOscillatorSignal : public Indicator
{
public:
    /**
     * @brief Construct a new AwesomeOscillatorSignal object.
     *
     * @param offset Offset value. Default is 0.
     */
    AwesomeOscillatorSignal(int offset = 0);

    /**
     * @brief Calculate the AwesomeOscillatorSignal values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;
};

// *********************************************************************************************

class KAMASignal : public Indicator
{
public:
    /**
     * @brief Construct a new KAMASignal object.
     *
     * @param er_period ER period value. Default is 10.
     * @param fastest_sc_period Fastest SC period value. Default is 2.
     * @param slowest_sc_period Slowest SC period value. Default is 30.
     * @param offset Offset value. Default is 0.
     */
    KAMASignal(int er_period = 10, int fastest_sc_period = 2, int slowest_sc_period = 30, int offset = 0);

    /**
     * @brief Calculate the KAMASignal values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;
};

// *********************************************************************************************

class MFISignal : public Indicator
{
public:
    /**
     * @brief Construct a new MFISignal object.
     *
     * @param period Period value. Default is 14.
     * @param overbought Overbought value. Default is 80.
     * @param oversold Oversold value. Default is 20.
     * @param offset Offset value. Default is 0.
     */
    MFISignal(int period = 14, int overbought = 80, int oversold = 20, int offset = 0);

    /**
     * @brief Calculate the MFISignal values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;
};

// *********************************************************************************************

class PPOSignal : public Indicator
{
public:
    /**
     * @brief Construct a new PPOSignal object.
     *
     * @param short_period Short period value. Default is 12.
     * @param long_period Long period value. Default is 26.
     * @param offset Offset value. Default is 0.
     */
    PPOSignal(int short_period = 12, int long_period = 26, int offset = 0);

    /**
     * @brief Calculate the PPOSignal values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;
};

// *********************************************************************************************

class ROCSignal : public Indicator
{
public:
    /**
     * @brief Construct a new ROCSignal object.
     *
     * @param period Period value. Default is 9.
     * @param offset Offset value. Default is 0.
     */
    ROCSignal(int period = 9, int offset = 0);

    /**
     * @brief Calculate the ROCSignal values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;
};

// *********************************************************************************************

class RSISignal : public Indicator
{
public:
    /**
     * @brief Construct a new RSISignal object.
     *
     * @param period Period value. Default is 14.
     * @param overbought Overbought value. Default is 70.
     * @param oversold Oversold value. Default is 30.
     * @param offset Offset value. Default is 0.
     */
    RSISignal(int period = 14, int overbought = 70, int oversold = 30, int offset = 0);

    /**
     * @brief Calculate the RSISignal values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;
};

// *********************************************************************************************

class StochasticRSISignal : public Indicator
{
public:
    /**
     * @brief Construct a new StochasticRSISignal object.
     *
     * @param period Period value. Default is 14.
     * @param overbought Overbought value. Default is 80.
     * @param oversold Oversold value. Default is 20.
     * @param offset Offset value. Default is 0.
     */
    StochasticRSISignal(int period = 14, int overbought = 80, int oversold = 20, int offset = 0);

    /**
     * @brief Calculate the StochasticRSISignal values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;
};

// *********************************************************************************************

class StochasticOscillatorSignal : public Indicator
{
public:
    /**
     * @brief Construct a new StochasticOscillatorSignal object.
     *
     * @param k_period K period value. Default is 14.
     * @param d_period D period value. Default is 3.
     * @param overbought Overbought value. Default is 80.
     * @param oversold Oversold value. Default is 20.
     * @param offset Offset value. Default is 0.
     */
    StochasticOscillatorSignal(int k_period = 14, int d_period = 3, int overbought = 80, int oversold = 20, int offset = 0);

    /**
     * @brief Calculate the StochasticOscillatorSignal values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;
};

// *********************************************************************************************

class TSISignal : public Indicator
{
public:
    /**
     * @brief Construct a new TSISignal object.
     *
     * @param short_period Short period value. Default is 13.
     * @param long_period Long period value. Default is 25.
     * @param overbought Overbought value. Default is 25.
     * @param oversold Oversold value. Default is -25.
     * @param offset Offset value. Default is 0.
     */
    TSISignal(int short_period = 13, int long_period = 25, int overbought = 25, int oversold = -25, int offset = 0);

    /**
     * @brief Calculate the TSISignal values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;
};

// *********************************************************************************************

class UOSignal : public Indicator
{
public:
    /**
     * @brief Construct a new UOSignal object.
     *
     * @param short_period Short period value. Default is 7.
     * @param medium_period Medium period value. Default is 14.
     * @param long_period Long period value. Default is 28.
     * @param overbought Overbought value. Default is 70.
     * @param oversold Oversold value. Default is 30.
     * @param offset Offset value. Default is 0.
     */
    UOSignal(int short_period = 7, int medium_period = 14, int long_period = 28, int overbought = 70, int oversold = 30, int offset = 0);

    /**
     * @brief Calculate the UOSignal values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;
};

// *********************************************************************************************

class WPRSignal : public Indicator
{
public:
    /**
     * @brief Construct a new WPRSignal object.
     *
     * @param period Period value. Default is 14.
     * @param overbought Overbought value. Default is -20.
     * @param oversold Oversold value. Default is -80.
     * @param offset Offset value. Default is 0.
     */
    WPRSignal(int period = 14, int overbought = -20, int oversold = -80, int offset = 0);

    /**
     * @brief Calculate the WPRSignal values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;
};

const std::unordered_map<std::string, std::function<Indicator *(std::vector<IndicatorParam>)>> momentum_signals_indicators_map = {
    {"awesome-oscillator-signal", [](std::vector<IndicatorParam> params) -> Indicator *
     {
         int offset = std::get<int>(params[0]);
         return new AwesomeOscillatorSignal(offset);
     }},
    {"kama-signal", [](std::vector<IndicatorParam> params) -> Indicator *
     {
         int er_period = std::get<int>(params[0]);
         int fastest_sc_period = std::get<int>(params[1]);
         int slowest_sc_period = std::get<int>(params[2]);
         int offset = std::get<int>(params[3]);
         return new KAMASignal(er_period, fastest_sc_period, slowest_sc_period, offset);
     }},
    {"mfi-signal", [](std::vector<IndicatorParam> params) -> Indicator *
     {
         int period = std::get<int>(params[0]);
         int overbought = std::get<int>(params[1]);
         int oversold = std::get<int>(params[2]);
         int offset = std::get<int>(params[3]);
         return new MFISignal(period, overbought, oversold, offset);
     }},
    {"ppo-signal", [](std::vector<IndicatorParam> params) -> Indicator *
     {
         int short_period = std::get<int>(params[0]);
         int long_period = std::get<int>(params[1]);
         int offset = std::get<int>(params[2]);
         return new PPOSignal(short_period, long_period, offset);
     }},
    {"roc-signal", [](std::vector<IndicatorParam> params) -> Indicator *
     {
         int period = std::get<int>(params[0]);
         int offset = std::get<int>(params[1]);
         return new ROCSignal(period, offset);
     }},
    {"rsi-signal", [](std::vector<IndicatorParam> params) -> Indicator *
     {
         int period = std::get<int>(params[0]);
         int overbought = std::get<int>(params[1]);
         int oversold = std::get<int>(params[2]);
         int offset = std::get<int>(params[3]);
         return new RSISignal(period, overbought, oversold, offset);
     }},
    {"stochastic-rsi-signal", [](std::vector<IndicatorParam> params) -> Indicator *
     {
         int period = std::get<int>(params[0]);
         int overbought = std::get<int>(params[1]);
         int oversold = std::get<int>(params[2]);
         int offset = std::get<int>(params[3]);
         return new StochasticRSISignal(period, overbought, oversold, offset);
     }},
    {"stochastic-oscillator-signal", [](std::vector<IndicatorParam> params) -> Indicator *
     {
         int k_period = std::get<int>(params[0]);
         int d_period = std::get<int>(params[1]);
         int overbought = std::get<int>(params[2]);
         int oversold = std::get<int>(params[3]);
         int offset = std::get<int>(params[4]);
         return new StochasticOscillatorSignal(k_period, d_period, overbought, oversold, offset);
     }},
    {"tsi-signal", [](std::vector<IndicatorParam> params) -> Indicator *
     {
         int short_period = std::get<int>(params[0]);
         int long_period = std::get<int>(params[1]);
         int overbought = std::get<int>(params[2]);
         int oversold = std::get<int>(params[3]);
         int offset = std::get<int>(params[4]);
         return new TSISignal(short_period, long_period, overbought, oversold, offset);
     }},
    {"uo-signal", [](std::vector<IndicatorParam> params) -> Indicator *
     {
         int short_period = std::get<int>(params[0]);
         int medium_period = std::get<int>(params[1]);
         int long_period = std::get<int>(params[2]);
         int overbought = std::get<int>(params[3]);
         int oversold = std::get<int>(params[4]);
         int offset = std::get<int>(params[5]);
         return new UOSignal(short_period, medium_period, long_period, overbought, oversold, offset);
     }},
    {"wpr-signal", [](std::vector<IndicatorParam> params) -> Indicator *
     {
         int period = std::get<int>(params[0]);
         int overbought = std::get<int>(params[1]);
         int oversold = std::get<int>(params[2]);
         int offset = std::get<int>(params[3]);
         return new WPRSignal(period, overbought, oversold, offset);
     }},
};

#endif // MOMENTUM_SIGNALS_INDICATORS_H
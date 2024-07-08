#ifndef VOLATILITY_SIGNALS_HPP
#define VOLATILITY_SIGNALS_HPP

#include <vector>
#include "indicator.hpp"

class ATRSignal : public Indicator
{
public:
    /**
     * @brief Construct a new ATRSignal object.
     *
     * @param period Period for ATR calculation.
     * @param threshold Threshold for ATR signal.
     * @param offset Offset for ATR signal.
     */
    ATRSignal(int period = 14, double threshold = 0.01, int offset = 0);

    /**
     * @brief Calculate the ATRSignal values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;
};

// *********************************************************************************************

class BollingerChannelSignal : public Indicator
{
public:
    /**
     * @brief Construct a new BollingerChannelSignal object.
     *
     * @param period Period for Bollinger Channel calculation.
     * @param multiplier Multiplier for Bollinger Channel.
     * @param offset Offset for Bollinger Channel.
     */
    BollingerChannelSignal(int period = 20, double multiplier = 2.0, int offset = 0);

    /**
     * @brief Calculate the BollingerChannelSignal values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;
};

// *********************************************************************************************

class DonchianChannelSignal : public Indicator
{
public:
    /**
     * @brief Construct a new DonchianChannelSignal object.
     *
     * @param period Period for Donchian Channel calculation.
     * @param offset Offset for Donchian Channel.
     */
    DonchianChannelSignal(int period = 20, int offset = 0);

    /**
     * @brief Calculate the DonchianChannel values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;
};

// *********************************************************************************************

class KeltnerChannelSignal : public Indicator
{
public:
    /**
     * @brief Construct a new KeltnerChannelSignal object.
     *
     * @param period Period for Keltner Channel calculation.
     * @param atr_period ATR period for Keltner Channel calculation.
     * @param multiplier Multiplier for Keltner Channel.
     * @param offset Offset for Keltner Channel.
     */
    KeltnerChannelSignal(int period = 20, int atr_period = 10, double multiplier = 2.0, int offset = 0);

    /**
     * @brief Calculate the KeltnerChannelSignal values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;
};

// *********************************************************************************************

const std::unordered_map<std::string, std::function<Indicator *(std::vector<IndicatorParam>)>> volatility_signals_indicators_map = {
    {"atr-signal", [](std::vector<IndicatorParam> params) -> Indicator *
     {
         int period = std::get<int>(params[0]);
         double threshold = std::get<double>(params[1]);
         int offset = std::get<int>(params[2]);
         return new ATRSignal(period, threshold, offset);
     }},
    {"bollinger-channel-signal", [](std::vector<IndicatorParam> params) -> Indicator *
     {
         int period = std::get<int>(params[0]);
         double multiplier = std::get<double>(params[1]);
         int offset = std::get<int>(params[2]);
         return new BollingerChannelSignal(period, multiplier, offset);
     }},
    {"donchian-channel-signal", [](std::vector<IndicatorParam> params) -> Indicator *
     {
         int period = std::get<int>(params[0]);
         int offset = std::get<int>(params[1]);
         return new DonchianChannelSignal(period, offset);
     }},
    {"keltner-channel-signal", [](std::vector<IndicatorParam> params) -> Indicator *
     {
         int period = std::get<int>(params[0]);
         int atr_period = std::get<int>(params[1]);
         double multiplier = std::get<double>(params[2]);
         int offset = std::get<int>(params[3]);
         return new KeltnerChannelSignal(period, atr_period, multiplier, offset);
     }},
};

#endif // VOLATILITY_SIGNALS_HPP
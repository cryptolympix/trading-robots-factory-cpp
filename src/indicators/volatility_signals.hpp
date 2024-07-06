#ifndef VOLATILITY_SIGNALS_HPP
#define VOLATILITY_SIGNALS_HPP

#include <vector>
#include "indicator.hpp"

class ATRSignal : public Indicator
{
public:
    std::string label = "Average True Range (Signal)";
    std::string id = "atr-signal";
    std::string id_pattern = R"(atr-signal-(\d+)-(\d+.\d+)-(\d+))";

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

private:
    int period;
    double threshold;
};

// *********************************************************************************************

class BollingerChannelSignal : public Indicator
{
public:
    std::string label = "Bollinger Channel (Signal)";
    std::string id = "bollinger-channel-signal";
    std::string id_pattern = R"(bollinger-channel-signal-(\d+)-(\d+.\d+)-(\d+))";

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

private:
    int period;
    double multiplier;
};

// *********************************************************************************************

class DonchianChannelSignal : public Indicator
{
public:
    std::string label = "Donchian Channel (Signal)";
    std::string id = "donchian-channel-signal";
    std::string id_pattern = R"(donchian-channel-signal-(\d+)-(\d+))";

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

private:
    int period;
};

// *********************************************************************************************

class KeltnerChannelSignal : public Indicator
{
public:
    std::string label = "Keltner Channel (Signal)";
    std::string id = "keltner-channel-signal";
    std::string id_pattern = R"(keltner-channel-signal-(\d+)-(\d+)-(\d+.\d+)-(\d+))";

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

private:
    int period;
    int atr_period;
    double multiplier;
};

// *********************************************************************************************

#endif // VOLATILITY_SIGNALS_HPP
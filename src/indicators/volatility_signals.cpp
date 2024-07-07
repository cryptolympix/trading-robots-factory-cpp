#include <vector>
#include "indicator.hpp"
#include "volatility.hpp"
#include "volatility_signals.hpp"

/**
 * @brief Construct a new ATRSignal object.
 *
 * @param period Period for ATR calculation.
 * @param threshold Threshold for ATR signal.
 * @param offset Offset for ATR signal.
 */
ATRSignal::ATRSignal(int period, double threshold, int offset) : Indicator("Average True Range (Signal)", "atr-signal", {{"period", period}, {"threshold", threshold}, {"offset", offset}}, {0, 1}) {}

/**
 * @brief Calculate the ATRSignal values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> ATRSignal::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles)
        {
            int period = std::get<int>(this->params.at("period"));
            double threshold = std::get<double>(this->params.at("threshold"));
            int offset = std::get<int>(this->params.at("offset"));

            std::vector<double> result(candles.size(), 0.0);
            std::vector<double> adx_values = ATR(period, offset).calculate(candles, false);

            for (size_t i = 1; i < adx_values.size(); i++)
            {
                if (adx_values[i - 1] < threshold && adx_values[i] > threshold)
                {
                    result[i] = 1.0;
                }
            }

            return result; },
        normalize_data);
}

// *********************************************************************************************

/**
 * @brief Construct a new BollingerChannelSignal object.
 *
 * @param period Period for Bollinger Channel calculation.
 * @param multiplier Multiplier for Bollinger Channel.
 * @param offset Offset for Bollinger Channel.
 */
BollingerChannelSignal::BollingerChannelSignal(int period, double multiplier, int offset) : Indicator("Bollinger Channel (Signal)", "bollinger-channel-signal", {{"period", period}, {"multiplier", multiplier}, {"offset", offset}}, {-1, 1}) {}

/**
 * @brief Calculate the BollingerBandsSignal values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> BollingerChannelSignal::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles)
        {
            int period = std::get<int>(this->params.at("period"));
            double multiplier = std::get<double>(this->params.at("multiplier"));
            int offset = std::get<int>(this->params.at("offset"));

            std::vector<double> result(candles.size(), 0.0);
            std::vector<double> bb_high_band_values = BollingerChannelHighBand(period, multiplier, offset).calculate(candles, false);
            std::vector<double> bb_middle_band_values = BollingerChannelMiddleBand(period, offset).calculate(candles, false);
            std::vector<double> bb_low_band_values = BollingerChannelLowBand(period, multiplier, offset).calculate(candles, false);

            for (size_t i = 1; i < bb_high_band_values.size(); i++)
            {
                if (candles[i - 1].close < bb_high_band_values[i - 1] && candles[i].close > bb_high_band_values[i])
                {
                    result[i] = 1.0;
                }
                else if (candles[i - 1].close > bb_low_band_values[i - 1] && candles[i].close < bb_low_band_values[i])
                {
                    result[i] = -1.0;
                }
                else if (candles[i - 1].close < bb_middle_band_values[i - 1] && candles[i].close > bb_middle_band_values[i])
                {
                    result[i] = 0.5;
                } 
                else if (candles[i - 1].close > bb_middle_band_values[i - 1] && candles[i].close < bb_middle_band_values[i])
                {
                    result[i] = -0.5;
                }
            }

            return result; },
        normalize_data);
}

// *********************************************************************************************

/**
 * @brief Construct a new DonchianChannelSignal object.
 *
 * @param period Period for Donchian Channel calculation.
 * @param offset Offset for Donchian Channel.
 */
DonchianChannelSignal::DonchianChannelSignal(int period, int offset) : Indicator("Donchian Channel (Signal)", "donchian-channel-signal", {{"period", period}, {"offset", offset}}, {-1, 1}) {}

/**
 * @brief Calculate the DonchianChannelSignal values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> DonchianChannelSignal::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles)
        {
            int period = std::get<int>(this->params.at("period"));
            int offset = std::get<int>(this->params.at("offset"));

            std::vector<double> result(candles.size(), 0.0);
            std::vector<double> dc_high_band_values = DonchianChannelHighBand(period, offset).calculate(candles, false);
            std::vector<double> dc_middle_band_values = DonchianChannelMiddleBand(period, offset).calculate(candles, false);
            std::vector<double> dc_low_band_values = DonchianChannelLowBand(period, offset).calculate(candles, false);

            for (size_t i = 1; i < dc_high_band_values.size(); i++)
            {
                if (candles[i - 1].close < dc_high_band_values[i - 1] && candles[i].close > dc_high_band_values[i])
                {
                    result[i] = 1.0;
                }
                else if (candles[i - 1].close > dc_low_band_values[i - 1] && candles[i].close < dc_low_band_values[i])
                {
                    result[i] = -1.0;
                }
                else if (candles[i - 1].close < dc_middle_band_values[i - 1] && candles[i].close > dc_middle_band_values[i])
                {
                    result[i] = 0.5;
                } 
                else if (candles[i - 1].close > dc_middle_band_values[i - 1] && candles[i].close < dc_middle_band_values[i])
                {
                    result[i] = -0.5;
                }
            }

            return result; },
        normalize_data);
}

// *********************************************************************************************

/**
 * @brief Construct a new KeltnerChannelSignal object.
 *
 * @param period Period for Keltner Channel calculation.
 * @param atr_period ATR period for Keltner Channel calculation.
 * @param multiplier Multiplier for Keltner Channel.
 * @param offset Offset for Keltner Channel.
 */
KeltnerChannelSignal::KeltnerChannelSignal(int period, int atr_period, double multiplier, int offset) : Indicator("Keltner Channel (Signal)", "keltner-channel-signal", {{"period", period}, {"atr_period", atr_period}, {"multiplier", multiplier}, {"offset", offset}}, {-1, 1}) {}

/**
 * @brief Calculate the KeltnerChannelSignal values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> KeltnerChannelSignal::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles)
        {
            int period = std::get<int>(this->params.at("period"));
            int atr_period = std::get<int>(this->params.at("atr_period"));
            double multiplier = std::get<double>(this->params.at("multiplier"));
            int offset = std::get<int>(this->params.at("offset"));
            
            std::vector<double> result(candles.size(), 0.0);
            std::vector<double> kc_high_band_values = KeltnerChannelHighBand(period, atr_period, multiplier, offset).calculate(candles, false);
            std::vector<double> kc_middle_band_values = KeltnerChannelMiddleBand(period, offset).calculate(candles, false);
            std::vector<double> kc_low_band_values = KeltnerChannelLowBand(period, atr_period, multiplier, offset).calculate(candles, false);

            for (size_t i = 1; i < kc_high_band_values.size(); i++)
            {
                if (candles[i - 1].close < kc_high_band_values[i - 1] && candles[i].close > kc_high_band_values[i])
                {
                    result[i] = 1.0;
                }
                else if (candles[i - 1].close > kc_low_band_values[i - 1] && candles[i].close < kc_low_band_values[i])
                {
                    result[i] = -1.0;
                }
                else if (candles[i - 1].close < kc_middle_band_values[i - 1] && candles[i].close > kc_middle_band_values[i])
                {
                    result[i] = 0.5;
                } 
                else if (candles[i - 1].close > kc_middle_band_values[i - 1] && candles[i].close < kc_middle_band_values[i])
                {
                    result[i] = -0.5;
                }
            }

            return result; },
        normalize_data);
}

// *********************************************************************************************
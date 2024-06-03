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
ATRSignal::ATRSignal(int period, int threshold, int offset) : Indicator("Average True Range Signal", "atr-signal-" + std::to_string(period) + "-" + std::to_string(threshold) + "-" + std::to_string(offset), offset, {0, 1}), period(period), threshold(threshold) {}

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
BollingerChannelSignal::BollingerChannelSignal(int period, double multiplier, int offset) : Indicator("Bollinger Channel Signal", "bollinger-channel-signal-" + std::to_string(period) + "-" + std::to_string(multiplier) + "-" + std::to_string(offset), offset, {-1, 1}), period(period), multiplier(multiplier) {}

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
 * @brief Construct a new DonchianChannel object.
 *
 * @param period Period for Donchian Channel calculation.
 * @param offset Offset for Donchian Channel.
 */
DonchianChannel::DonchianChannel(int period, int offset) : Indicator("Donchian Channel", "donchian-channel-" + std::to_string(period) + "-" + std::to_string(offset), offset, {-1, 1}), period(period) {}

/**
 * @brief Calculate the DonchianChannel values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> DonchianChannel::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles)
        {
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
KeltnerChannelSignal::KeltnerChannelSignal(int period, int atr_period, double multiplier, int offset) : Indicator("Keltner Channel Signal", "keltner-channel-signal-" + std::to_string(period) + "-" + std::to_string(atr_period) + "-" + std::to_string(multiplier) + "-" + std::to_string(offset), offset, {-1, 1}), period(period), atr_period(atr_period), multiplier(multiplier) {}

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
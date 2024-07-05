#include <numeric>
#include <algorithm>
#include "../types.hpp"
#include "../utils/candles_source.hpp"
#include "indicator.hpp"
#include "utils.hpp"
#include "volatility.hpp"

/**
 * @brief Construct a new ATRIndicator object.
 *
 * @param period Period value. Default is 14.
 * @param offset Offset value. Default is 0.
 */
ATR::ATR(int period, int offset) : Indicator("Average True Range", "atr-" + std::to_string(period) + "-" + std::to_string(offset), R"(atr-(\d+)-(\d+))", offset), period(period) {}

/**
 * @brief Calculate the Average True Range values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> ATR::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles) -> std::vector<double>
        {
            std::vector<double> atr_values(candles.size(), 0.0);

            if (candles.size() < period) {
                return atr_values;
            }

            std::vector<double> tr_values = calculateTrueRange(candles);

            // Calculate the initial SMA as the average of the first period values
            double sum = 0.0;
            for (int i = 0; i < period; ++i)
            {
                sum += tr_values[i];
            }
            atr_values[period - 1] = sum / period;

            // Calculate SMA for subsequent periods
            for (size_t i = period; i < tr_values.size(); ++i)
            {
                atr_values[i] = (atr_values[i - 1] * (period - 1) + tr_values[i]) / period;
            }

            return atr_values; },

        normalize_data);
}

/**
 * @brief Calculate the True Range for a specific candle.
 *
 * @param candles Vector of Candle data.
 * @return std::vector<double> The calculated True Range values.
 */
std::vector<double> ATR::calculateTrueRange(const std::vector<Candle> &candles) const
{
    std::vector<double> tr_values(candles.size(), 0.0);

    for (size_t i = 0; i < candles.size(); ++i)
    {
        if (i == 0)
        {
            tr_values[i] = candles[i].high - candles[i].low;
        }
        else
        {
            double range1 = candles[i].high - candles[i].low;
            double range2 = std::abs(candles[i].high - candles[i - 1].close);
            double range3 = std::abs(candles[i].low - candles[i - 1].close);
            tr_values[i] = std::max(range1, std::max(range2, range3));
        }
    }

    return tr_values;
}

// *****************************************************************************

/**
 * @brief Construct a new BollingerChannelHighBand object.
 *
 * @param period Period value. Default is 20.
 * @param multiplier Multiplier value. Default is 2.0.
 * @param offset Offset value. Default is 0.
 */
BollingerChannelHighBand::BollingerChannelHighBand(int period, double multiplier, int offset) : Indicator("Bollinger Channel High Band", "bollinger-channel-high-band" + std::to_string(period) + "-" + std::to_string(multiplier) + "-" + std::to_string(offset), R"(bollinger-channel-high-band-(\d+)-(\d+.\d+)-(\d+))", offset), period(period), multiplier(multiplier) {}

/**
 * @brief Calculate the Bollinger Channel High Band values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> BollingerChannelHighBand::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles) -> std::vector<double>
        {
            std::vector<double> bollinger_channel_high_band_values(candles.size(), 0.0);

            if (candles.size() < period) {
                return bollinger_channel_high_band_values;
            }

            std::vector<double> closes = get_candles_with_source(candles, "close");
            std::vector<double> sma_values = calculate_simple_moving_average(closes, period);
            std::vector<double> std_dev_values = calculate_standard_deviation(closes, period);

            for (size_t i = period - 1; i < candles.size(); ++i)
            {
                bollinger_channel_high_band_values[i] = sma_values[i] + (multiplier * std_dev_values[i]);
            }

            return bollinger_channel_high_band_values; },

        normalize_data);
}

// *****************************************************************************

/**
 * @brief Construct a new BollingerChannelLowBand object.
 *
 * @param period Period value. Default is 20.
 * @param multiplier Multiplier value. Default is 2.0.
 * @param offset Offset value. Default is 0.
 */
BollingerChannelLowBand::BollingerChannelLowBand(int period, double multiplier, int offset) : Indicator("Bollinger Channel Low Band", "bollinger-channel-low-band" + std::to_string(period) + "-" + std::to_string(multiplier) + "-" + std::to_string(offset), R"(bollinger-channel-low-band-(\d+)-(\d+.\d+)-(\d+))", offset), period(period), multiplier(multiplier) {}

/**
 * @brief Calculate the Bollinger Channel Low Band values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> BollingerChannelLowBand::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles) -> std::vector<double>
        {
            std::vector<double> bollinger_channel_low_band_values(candles.size(), 0.0);

            if (candles.size() < period) {
                return bollinger_channel_low_band_values;
            }

            std::vector<double> closes = get_candles_with_source(candles, "close");
            std::vector<double> sma_values = calculate_simple_moving_average(closes, period);
            std::vector<double> std_dev_values = calculate_standard_deviation(closes, period);

            for (size_t i = period - 1; i < candles.size(); ++i)
            {
                bollinger_channel_low_band_values[i] = sma_values[i] - (multiplier * std_dev_values[i]);
            }

            return bollinger_channel_low_band_values; },

        normalize_data);
}

// *****************************************************************************

/**
 * @brief Construct a new BollingerChannelMiddleBand object.
 *
 * @param period Period value. Default is 20.
 * @param offset Offset value. Default is 0.
 */
BollingerChannelMiddleBand::BollingerChannelMiddleBand(int period, int offset) : Indicator("Bollinger Channel Middle Band", "bollinger-channel-middle-band" + std::to_string(period) + "-" + std::to_string(offset), R"(bollinger-channel-middle-band-(\d+)-(\d+))", offset), period(period){};

/**
 * @brief Calculate the Bollinger Channel Middle Band values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> BollingerChannelMiddleBand::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles) -> std::vector<double>
        {
            std::vector<double> bollinger_channel_middle_band_values(candles.size(), 0.0);

            if (candles.size() < period) {
                return bollinger_channel_middle_band_values;
            }

            std::vector<double> closes = get_candles_with_source(candles, "close");
            std::vector<double> sma_values = calculate_simple_moving_average(closes, period);

            for (size_t i = 0; i < candles.size(); ++i)
            {
                bollinger_channel_middle_band_values[i] = sma_values[i];
            }

            return bollinger_channel_middle_band_values; },

        normalize_data);
}

// *****************************************************************************

/**
 * @brief Construct a new BollingerChannelPercentageBand object.
 *
 * @param period Period value. Default is 20.
 * @param multiplier Multiplier value. Default is 2.0.
 * @param offset Offset value. Default is 0.
 */
BollingerChannelPercentageBand::BollingerChannelPercentageBand(int period, double multiplier, int offset) : Indicator("Bollinger Channel Percentage Band", "bollinger-channel-percentage-band-" + std::to_string(period) + "-" + std::to_string(multiplier) + "-" + std::to_string(offset), R"(bollinger-channel-percentage-band-(\d+)-(\d+.\d+)-(\d+))", offset), period(period), multiplier(multiplier) {}

/**
 * @brief Calculate the Bollinger Channel Percentage Band values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> BollingerChannelPercentageBand::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles) -> std::vector<double>
        {
            std::vector<double> bollinger_channel_percentage_band_values(candles.size(), 0.0);

            if (candles.size() < period) {
                return bollinger_channel_percentage_band_values;
            }

            std::vector<double> closes = get_candles_with_source(candles, "close");
            std::vector<double> sma_values = calculate_simple_moving_average(closes, period);
            std::vector<double> std_dev_values = calculate_standard_deviation(closes, period);

            for (size_t i = period - 1; i < candles.size(); ++i)
            {
                double middle_band = sma_values[i];
                double upper_band = (sma_values[i] + (multiplier * std_dev_values[i]));
                double lower_band = (sma_values[i] - (multiplier * std_dev_values[i]));

                bollinger_channel_percentage_band_values[i] = (upper_band - lower_band) > 0 ? (closes[i] - lower_band) / (upper_band - lower_band) : 0.0;
            }

            return bollinger_channel_percentage_band_values; },

        normalize_data);
}

// *****************************************************************************

/**
 * @brief Construct a new BollingerChannelWidthBand object.
 *
 * @param period Period value. Default is 20.
 * @param multiplier Multiplier value. Default is 2.0.
 * @param offset Offset value. Default is 0.
 */
BollingerChannelWidthBand::BollingerChannelWidthBand(int period, double multiplier, int offset) : Indicator("Bollinger Channel Width Band", "bollinger-channel-width-band" + std::to_string(period) + "-" + std::to_string(multiplier) + "-" + std::to_string(offset), R"(bollinger-channel-width-band-(\d+)-(\d+.\d+)-(\d+))", offset), period(period), multiplier(multiplier) {}

/**
 * @brief Calculate the Bollinger Channel Width Band values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> BollingerChannelWidthBand::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles) -> std::vector<double>
        {
            std::vector<double> bollinger_channel_width_band_values(candles.size(), 0.0);

            if (candles.size() < period) {
                return bollinger_channel_width_band_values;
            }

            std::vector<double> closes = get_candles_with_source(candles, "close");
            std::vector<double> sma_values = calculate_simple_moving_average(closes, period);
            std::vector<double> std_dev_values = calculate_standard_deviation(closes, period);

            for (size_t i = period - 1; i < candles.size(); ++i)
            {
                double middle_band = sma_values[i];
                double upper_band = (sma_values[i] + (multiplier * std_dev_values[i]));
                double lower_band = (sma_values[i] - (multiplier * std_dev_values[i]));

                bollinger_channel_width_band_values[i] = middle_band > 0 ? (upper_band - lower_band) / middle_band : 0.0;
            }

            return bollinger_channel_width_band_values; },

        normalize_data);
}

// *****************************************************************************

/**
 * @brief Construct a new DonchianChannelHighBand object.
 *
 * @param period Period value. Default is 20.
 * @param offset Offset value. Default is 0.
 */
DonchianChannelHighBand::DonchianChannelHighBand(int period, int offset) : Indicator("Donchian Channel High Band", "donchian-channel-high-band" + std::to_string(period) + "-" + std::to_string(offset), R"(donchian-channel-high-band-(\d+)-(\d+))", offset), period(period) {}

/**
 * @brief Calculate the Donchian Channel High Band values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> DonchianChannelHighBand::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles) -> std::vector<double>
        {
            std::vector<double> donchian_channel_high_band_values(candles.size(), 0.0);

            if (candles.size() < period) {
                return donchian_channel_high_band_values;
            }

            for (size_t i = period - 1; i < candles.size(); ++i)
            {
                double high = candles[i - period + 1].high;

                // Find the highest high value in the period
                for (size_t j = i - period + 2; j <= i; ++j)
                {
                    high = std::max(high, candles[j].high);
                }

                // Set the high band value
                donchian_channel_high_band_values[i] = high;
            }

            return donchian_channel_high_band_values; },

        normalize_data);
}

// *****************************************************************************

/**
 * @brief Construct a new DonchianChannelLowBand object.
 *
 * @param period Period value. Default is 20.
 * @param offset Offset value. Default is 0.
 */
DonchianChannelLowBand::DonchianChannelLowBand(int period, int offset) : Indicator("Donchian Channel Low Band", "donchian-channel-low-band" + std::to_string(period) + "-" + std::to_string(offset), R"(donchian-channel-low-band-(\d+)-(\d+))", offset), period(period) {}

/**
 * @brief Calculate the Donchian Channel Low Band values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> DonchianChannelLowBand::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles) -> std::vector<double>
        {
            std::vector<double> donchian_channel_low_band_values(candles.size(), 0.0);

            if (candles.size() < period) {
                return donchian_channel_low_band_values;
            }

            for (size_t i = period - 1; i < candles.size(); ++i)
            {
                double low = candles[i - period + 1].low;

                // Find the lowest low value in the period
                for (size_t j = i - period + 2; j <= i; ++j)
                {
                    low = std::min(low, candles[j].low);
                }

                // Set the low band value
                donchian_channel_low_band_values[i] = low;
            }

            return donchian_channel_low_band_values; },

        normalize_data);
}

// *****************************************************************************

/**
 * @brief Construct a new DonchianChannelMiddleBand object.
 *
 * @param period Period value. Default is 20.
 * @param offset Offset value. Default is 0.
 */
DonchianChannelMiddleBand::DonchianChannelMiddleBand(int period, int offset) : Indicator("Donchian Channel Middle Band", "donchian-channel-middle-band" + std::to_string(period) + "-" + std::to_string(offset), R"(donchian-channel-middle-band-(\d+)-(\d+))", offset), period(period) {}

/**
 * @brief Calculate the Donchian Channel Middle Band values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> DonchianChannelMiddleBand::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles) -> std::vector<double>
        {
            std::vector<double> donchian_channel_middle_band_values(candles.size(), 0.0);

            if (candles.size() < period) {
                return donchian_channel_middle_band_values;
            }

            for (size_t i = period - 1; i < candles.size(); ++i)
            {
                double high = candles[i - period + 1].high;
                double low = candles[i - period + 1].low;

                // Find the highest high and lowest low values in the period
                for (size_t j = i - period + 2; j <= i; ++j)
                {
                    high = std::max(high, candles[j].high);
                    low = std::min(low, candles[j].low);
                }

                // Set the middle band value
                donchian_channel_middle_band_values[i] = (high + low) / 2;
            }

            return donchian_channel_middle_band_values; },

        normalize_data);
}

// *****************************************************************************

/**
 * @brief Construct a new KeltnerChannelHighBand object.
 *
 * @param period Period value. Default is 20.
 * @param atr_period ATR period value. Default is 10.
 * @param multiplier Multiplier value. Default is 2.0.
 * @param offset Offset value. Default is 0.
 */
KeltnerChannelHighBand::KeltnerChannelHighBand(int period, int atr_period, double multiplier, int offset) : Indicator("Keltner Channel High Band", "keltner-channel-high-band-" + std::to_string(period) + "-" + std::to_string(atr_period) + "-" + std::to_string(multiplier) + "-" + std::to_string(offset), R"(keltner-channel-high-band-(\d+)-(\d+)-(\d+.\d+)-(\d+))", offset), period(period), atr_period(atr_period), multiplier(multiplier) {}

/**
 * @brief Calculate the Keltner Channel High Band values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> KeltnerChannelHighBand::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles) -> std::vector<double>
        {
            std::vector<double> keltner_channel_high_band_values(candles.size(), 0.0);

            if (candles.size() < period) {
                return keltner_channel_high_band_values;
            }

            std::vector<double> closes = get_candles_with_source(candles, "close");
            std::vector<double> ema_values = calculate_exponential_moving_average(closes, period);
            std::vector<double> atr_values = ATR(atr_period).calculate(candles);

            for (size_t i = period - 1; i < candles.size(); ++i)
            {
                keltner_channel_high_band_values[i] = ema_values[i] + (multiplier * atr_values[i]);
            }

            return keltner_channel_high_band_values; },

        normalize_data);
}

// *****************************************************************************

/**
 * @brief Construct a new KeltnerChannelLowBand object.
 *
 * @param period Period value. Default is 20.
 * @param atr_period ATR period value. Default is 10.
 * @param multiplier Multiplier value. Default is 2.0.
 * @param offset Offset value. Default is 0.
 */
KeltnerChannelLowBand::KeltnerChannelLowBand(int period, int atr_period, double multiplier, int offset) : Indicator("Keltner Channel Low Band", "keltner-channel-low-band" + std::to_string(period) + "-" + std::to_string(atr_period) + "-" + std::to_string(multiplier) + "-" + std::to_string(offset), R"(keltner-channel-low-band-(\d+)-(\d+)-(\d+.\d+)-(\d+))", offset), period(period), atr_period(atr_period), multiplier(multiplier) {}

/**
 * @brief Calculate the Keltner Channel Low Band values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> KeltnerChannelLowBand::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles) -> std::vector<double>
        {
            std::vector<double> keltner_channel_low_band_values(candles.size(), 0.0);

            if (candles.size() < period) {
                return keltner_channel_low_band_values;
            }

            std::vector<double> closes = get_candles_with_source(candles, "close");
            std::vector<double> ema_values = calculate_exponential_moving_average(closes, period);
            std::vector<double> atr_values = ATR(atr_period).calculate(candles);

            for (size_t i = period - 1; i < candles.size(); ++i)
            {
                keltner_channel_low_band_values[i] = ema_values[i] - (multiplier * atr_values[i]);
            }

            return keltner_channel_low_band_values; },

        normalize_data);
}

// *****************************************************************************

/**
 * @brief Construct a new KeltnerChannelMiddleBand object.
 *
 * @param period Period value. Default is 20.
 * @param offset Offset value. Default is 0.
 */
KeltnerChannelMiddleBand::KeltnerChannelMiddleBand(int period, int offset) : Indicator("Keltner Channel Middle Band", "keltner-channel-middle-band" + std::to_string(period) + "-" + std::to_string(offset), R"(keltner-channel-middle-band-(\d+)-(\d+))", offset), period(period) {}

/**
 * @brief Calculate the Keltner Channel Middle Band values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> KeltnerChannelMiddleBand::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles) -> std::vector<double>
        {
            std::vector<double> keltner_channel_middle_band_values(candles.size(), 0.0);

            if (candles.size() < period) {
                return keltner_channel_middle_band_values;
            }

            std::vector<double> closes = get_candles_with_source(candles, "close");
            std::vector<double> ema_values = calculate_exponential_moving_average(closes, period);

            for (size_t i = period - 1; i < candles.size(); ++i)
            {
                keltner_channel_middle_band_values[i] = ema_values[i];
            }

            return keltner_channel_middle_band_values; },

        normalize_data);
}

// *****************************************************************************

/**
 * @brief Construct a new Ulcer Index object.
 *
 * @param period Period value. Default is 14.
 * @param offset Offset value. Default is 0.
 */
UI::UI(int period, int offset) : Indicator("Ulcer Index", "ui-" + std::to_string(period) + "-" + std::to_string(offset), R"(ui-(\d+)-(\d+))", offset), period(period) {}

/**
 * @brief Calculate the Ulcer Index values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> UI::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles) -> std::vector<double>
        {
            std::vector<double> ui_values(candles.size(), 0.0);

            if (candles.size() < period) {
                return ui_values;
            }

            std::vector<double> closes = get_candles_with_source(candles, "close");
            std::vector<double> max_values(candles.size(), 0.0);

            for (size_t i = period - 1; i < candles.size(); ++i)
            {
                double max_close = closes[i - period + 1];

                for (size_t j = i - period + 2; j <= i; ++j)
                {
                    max_close = std::max(max_close, closes[j]);
                }

                max_values[i] = max_close;
            }

            std::vector<double> drawdown(candles.size(), 0.0);
            for (size_t i = period - 1; i < candles.size(); ++i)
            {
                drawdown[i] = (closes[i] - max_values[i]) / max_values[i];
            }

            for (size_t i = period - 1; i < candles.size(); ++i)
            {
                double sum = 0.0;
                for (size_t j = i - period + 1; j <= i; ++j)
                {
                    sum += drawdown[j] * drawdown[j];
                }
                ui_values[i] = std::sqrt(sum / period);
            }

            return ui_values; },

        normalize_data);
}

// *****************************************************************************

/**
 * @brief Construct a new Standard Deviation object.
 *
 * @param period Period value. Default is 14.
 * @param offset Offset value. Default is 0.
 */
StandardDeviation::StandardDeviation(int period, int offset) : Indicator("Standard Deviation", "stddev-" + std::to_string(period) + "-" + std::to_string(offset), R"(stddev-(\d+)-(\d+))", offset), period(period) {}

/**
 * @brief Calculate the Standard Deviation values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> StandardDeviation::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles) -> std::vector<double>
        {
            std::vector<double> stddev_values(candles.size(), 0.0);

            if (candles.size() < period) {
                return stddev_values;
            }

            std::vector<double> closes = get_candles_with_source(candles, "close");
            stddev_values = calculate_standard_deviation(closes, period);

            return stddev_values; },

        normalize_data);
}

// *****************************************************************************

/**
 * @brief Construct a new Average Price Change object.
 *
 * @param period Period value. Default is 14.
 * @param offset Offset value. Default is 0.
 */
AveragePriceChange::AveragePriceChange(int period, int offset) : Indicator("Average Price Change", "average-price-change-" + std::to_string(period) + "-" + std::to_string(offset), R"(average-price-change-(\d+)-(\d+))", offset), period(period) {}

/**
 * @brief Calculate the Average Price Change values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> AveragePriceChange::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles) -> std::vector<double>
        {
            std::vector<double> average_price_change_values(candles.size(), 0.0);

            if (candles.size() < period) {
                return average_price_change_values;
            }

            for (int i = period; i < candles.size(); ++i)
            {
                double sum = 0.0;
                for (int j = std::max(i - period + 1, 0); j <= i; ++j)
                {
                    sum += candles[j].close - candles[j - 1].close;
                }
                average_price_change_values[i] = sum / period;
            }

            return average_price_change_values; },

        normalize_data);
}
#include <vector>
#include <stdexcept>
#include <numeric>
#include "../types.hpp"
#include "../utils/math.hpp"
#include "../utils/candles_source.hpp"
#include "../utils/vectors.hpp"
#include "utils.hpp"
#include "indicator.hpp"
#include "trend.hpp"

/**
 * @brief Construct a new Average Directional Movement Index object.
 *
 * @param period Period value. Default is 14.
 * @param offset Offset value. Default is 0.
 */
ADX::ADX(int period, int offset) : Indicator("Average Directional Movement Index", "adx-" + std::to_string(period) + "-" + std::to_string(offset), offset), period(period) {}

/**
 * @brief Calculate the Average Directional Movement Index values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> ADX::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles)
        {
            if (candles.size() < static_cast<size_t>(period))
            {
                throw std::invalid_argument("Insufficient data to calculate Average Directional Movement Index");
            }

            std::vector<double> high_values = get_candles_with_source(candles, CandleSource::High);
            std::vector<double> low_values = get_candles_with_source(candles, CandleSource::Low);
            std::vector<double> close_values = get_candles_with_source(candles, CandleSource::Close);

            std::vector<double> true_range_values;
            true_range_values.reserve(candles.size() - 1);

            for (size_t i = 1; i < candles.size(); ++i)
            {
                double true_range = std::max(high_values[i] - low_values[i], std::max(std::abs(high_values[i] - close_values[i - 1]), std::abs(low_values[i] - close_values[i - 1])));
                true_range_values.push_back(true_range);
            }

            std::vector<double> directional_movement_plus;
            std::vector<double> directional_movement_minus;
            directional_movement_plus.reserve(true_range_values.size());
            directional_movement_minus.reserve(true_range_values.size());

            for (size_t i = 1; i < true_range_values.size(); ++i)
            {
                double high_diff = high_values[i] - high_values[i - 1];
                double low_diff = low_values[i - 1] - low_values[i];
                if (high_diff > low_diff && high_diff > 0)
                {
                    directional_movement_plus.push_back(high_diff);
                }
                else
                {
                    directional_movement_plus.push_back(0);
                }

                if (low_diff > high_diff && low_diff > 0)
                {
                    directional_movement_minus.push_back(low_diff);
                }
                else
                {
                    directional_movement_minus.push_back(0);
                }
            }

            std::vector<double> smooth_directional_movement_plus = smooth_directional_movement(directional_movement_plus);
            std::vector<double> smooth_directional_movement_minus = smooth_directional_movement(directional_movement_minus);
            std::vector<double> true_range_smoothed = smooth_true_range(true_range_values);

            std::vector<double> positive_directional_index = calculate_positive_directional_index(smooth_directional_movement_plus, true_range_smoothed);
            std::vector<double> negative_directional_index = calculate_negative_directional_index(smooth_directional_movement_minus, true_range_smoothed);

            std::vector<double> directional_movement_index = calculate_directional_movement_index(positive_directional_index, negative_directional_index);
            std::vector<double> adx_values = calculate_adx(directional_movement_index);

            return adx_values; },

        normalize_data);
}

std::vector<double> ADX::smooth_directional_movement(const std::vector<double> &directional_movement) const
{
    std::vector<double> smoothed_values;
    smoothed_values.reserve(directional_movement.size() - period + 1);
    double sum = std::accumulate(directional_movement.begin(), directional_movement.begin() + period, 0.0);
    smoothed_values.push_back(sum / period);

    for (size_t i = period; i < directional_movement.size(); ++i)
    {
        sum -= directional_movement[i - period];
        sum += directional_movement[i];
        smoothed_values.push_back(sum / period);
    }

    return smoothed_values;
}

std::vector<double> ADX::smooth_true_range(const std::vector<double> &true_range_values) const
{
    std::vector<double> smoothed_values;
    smoothed_values.reserve(true_range_values.size() - period + 1);
    double sum = std::accumulate(true_range_values.begin(), true_range_values.begin() + period, 0.0);
    smoothed_values.push_back(sum / period);

    for (size_t i = period; i < true_range_values.size(); ++i)
    {
        sum -= true_range_values[i - period];
        sum += true_range_values[i];
        smoothed_values.push_back(sum / period);
    }

    return smoothed_values;
}

std::vector<double> ADX::calculate_positive_directional_index(const std::vector<double> &smoothed_directional_movement_plus, const std::vector<double> &smoothed_true_range) const
{
    std::vector<double> positive_directional_index;
    positive_directional_index.reserve(smoothed_directional_movement_plus.size());
    for (size_t i = 0; i < smoothed_directional_movement_plus.size(); ++i)
    {
        double positive_directional_index_value = smoothed_directional_movement_plus[i] / smoothed_true_range[i] * 100.0;
        positive_directional_index.push_back(positive_directional_index_value);
    }
    return positive_directional_index;
}

std::vector<double> ADX::calculate_negative_directional_index(const std::vector<double> &smoothed_directional_movement_minus, const std::vector<double> &smoothed_true_range) const
{
    std::vector<double> negative_directional_index;
    negative_directional_index.reserve(smoothed_directional_movement_minus.size());
    for (size_t i = 0; i < smoothed_directional_movement_minus.size(); ++i)
    {
        double negative_directional_index_value = smoothed_directional_movement_minus[i] / smoothed_true_range[i] * 100.0;
        negative_directional_index.push_back(negative_directional_index_value);
    }
    return negative_directional_index;
}

std::vector<double> ADX::calculate_directional_movement_index(const std::vector<double> &positive_directional_index, const std::vector<double> &negative_directional_index) const
{
    std::vector<double> directional_movement_index;
    directional_movement_index.reserve(positive_directional_index.size());
    for (size_t i = 0; i < positive_directional_index.size(); ++i)
    {
        double directional_movement_index_value = std::abs((positive_directional_index[i] - negative_directional_index[i]) / (positive_directional_index[i] + negative_directional_index[i])) * 100.0;
        directional_movement_index.push_back(directional_movement_index_value);
    }
    return directional_movement_index;
}

std::vector<double> ADX::calculate_adx(const std::vector<double> &directional_movement_index) const
{
    std::vector<double> adx_values;
    adx_values.reserve(directional_movement_index.size());
    double sum = std::accumulate(directional_movement_index.begin(), directional_movement_index.begin() + period, 0.0);
    adx_values.push_back(sum / period);

    for (size_t i = period; i < directional_movement_index.size(); ++i)
    {
        sum -= adx_values[i - period];
        sum += directional_movement_index[i];
        adx_values.push_back(sum / period);
    }

    return adx_values;
}

// *********************************************************************************************

/**
 * @brief Construct a new AroonTrend object.
 *
 * @param period Period value. Default is 14.
 * @param offset Offset value. Default is 0.
 */
AroonTrend::AroonTrend(int period, int offset) : Indicator("Aroon Trend", "aroon-trend-" + std::to_string(period) + "-" + std::to_string(offset), offset), period(period) {}

/**
 * @brief Calculate the Aroon Up values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> AroonTrend::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles)
        {
            if (candles.size() < static_cast<size_t>(period))
            {
                throw std::invalid_argument("Insufficient data to calculate Aroon Trend");
            }

            std::vector<double> high_values = get_candles_with_source(candles, CandleSource::High);
            std::vector<double> low_values = get_candles_with_source(candles, CandleSource::Low);

            std::vector<double> values;
            values.reserve(candles.size() - period + 1);

            for (size_t i = period; i <= candles.size(); ++i)
            {
                double highest_high = *std::max_element(high_values.begin() + i - period, high_values.begin() + i);
                int distance_from_highest = period - 1 - (std::find(high_values.begin() + i - period, high_values.begin() + i, highest_high) - (high_values.begin() + i - period));
                double aroon_up = (static_cast<double>(distance_from_highest) / period) * 100.0;

                double lowest_low = *std::min_element(low_values.begin() + i - period, low_values.begin() + i);
                int distance_from_lowest = period - 1 - (std::find(low_values.begin() + i - period, low_values.begin() + i, lowest_low) - (low_values.begin() + i - period));
                double aroon_down = (static_cast<double>(distance_from_lowest) / period) * 100.0;

                if (aroon_up > aroon_down)
                {
                    values.push_back(1.0);
                }
                else if (aroon_up < aroon_down)
                {
                    values.push_back(-1.0);
                }
                else
                {
                    values.push_back(0.0);
                }
            }

            return values; },

        normalize_data);
}

// *********************************************************************************************

/**
 * @brief Construct a new CCI object.
 *
 * @param period Period value. Default is 20.
 * @param offset Offset value. Default is 0.
 */
CCI::CCI(int period, int offset) : Indicator("Commodity Channel Index", "cci-" + std::to_string(period) + "-" + std::to_string(offset), offset), period(period) {}

/**
 * @brief Calculate the CCI values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> CCI::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles)
        {
            std::vector<double> typical_prices = get_candles_with_source(candles, CandleSource::HLC3);
            std::vector<double> sma_values = calculate_simple_moving_average(typical_prices, period);
            std::vector<double> mean_deviation_values = calculate_mean_deviation(typical_prices, sma_values);

            std::vector<double> cci_values;
            cci_values.reserve(sma_values.size());

            for (size_t i = period - 1; i < sma_values.size(); ++i)
            {
                double typical_price = typical_prices[i];
                double sma = sma_values[i];
                double mean_deviation = mean_deviation_values[i];
                double cci = (typical_price - sma) / (0.015 * mean_deviation);
                cci_values.push_back(cci);
            } 
            
            return cci_values; },

        normalize_data);
}

/**
 * @brief Calculate the mean deviation of typical prices.
 *
 * @param typical_prices Vector containing typical prices.
 * @param sma_values Vector containing simple moving average values.
 * @param period Period or number of values to consider for the mean deviation calculation.
 * @return std::vector<double> Vector containing mean deviation values.
 */
std::vector<double> CCI::calculate_mean_deviation(const std::vector<double> &typical_prices, const std::vector<double> &sma_values) const
{
    if (typical_prices.size() != sma_values.size())
    {
        throw std::invalid_argument("Typical prices vector size and SMA vector size mismatch");
    }

    std::vector<double> mean_deviation_values(typical_prices.size(), 0.0); // Initialize mean_deviation_values with the same size as input vectors

    for (size_t i = period - 1; i < typical_prices.size(); ++i)
    {
        double mean_deviation = 0.0;
        for (size_t j = i - (period - 1); j <= i; ++j)
        {
            mean_deviation += std::abs(typical_prices[j] - sma_values[j]);
        }
        mean_deviation /= period;
        mean_deviation_values[i] = mean_deviation;
    }

    return mean_deviation_values;
}

// *********************************************************************************************

/**
 * @brief Construct a new DPO object.
 *
 * @param period Period value. Default is 20.
 * @param offset Offset value. Default is 0.
 */
DPO::DPO(int period, int offset) : Indicator("Detrended Price Oscillator", "dpo-" + std::to_string(period) + "-" + std::to_string(offset), offset), period(period) {}

/**
 * @brief Calculate the DPO values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> DPO::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles)
        {
            std::vector<double> dpo_values;
            dpo_values.reserve(candles.size());

            for (size_t i = period; i < candles.size(); ++i)
            {
                double sum = 0.0;
                for (int j = 0; j < period; ++j)
                {
                    sum += candles[i - j - period].close;
                }
                double moving_average = sum / period;
                dpo_values.push_back(candles[i].close - moving_average);
            }

            return dpo_values; },

        normalize_data);
}

// *********************************************************************************************

/**
 * @brief Construct a new EMA object.
 *
 * @param source Source value.
 * @param period Period value.
 * @param offset Offset value. Default is 0.
 */
EMA::EMA(CandleSource source, int period, int offset) : Indicator("Exponential Moving Average", "ema-" + CandleSourceMap[source] + "-" + std::to_string(period) + "-" + std::to_string(offset), offset), source(source), period(period) {}

/**
 * @brief Calculate the EMA values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> EMA::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles)
        {
            std::vector<double> ema_values;
            ema_values.reserve(candles.size());

            double multiplier = 2.0 / (period + 1);
            double ema = 0.0;
            std::vector<double> source_values = get_candles_with_source(candles, source);

            // Calculate initial SMA as first value
            double sum = 0.0;
            for (int i = 0; i < period; ++i)
            {
                ema_values.push_back(std::numeric_limits<double>::quiet_NaN());
            }

            // Calculate EMA for subsequent values
            for (size_t i = period; i < candles.size(); ++i)
            {
                ema = (source_values[i] - ema) * multiplier + ema;
                ema_values.push_back(ema);
            }

            return ema_values; },

        normalize_data);
}

// *********************************************************************************************

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
KSTOscillator::KSTOscillator(int roc_periods1, int roc_periods2, int roc_periods3, int roc_periods4,
                             int sma_periods1, int sma_periods2, int sma_periods3, int sma_periods4,
                             int signal_periods, int offset)
    : Indicator("KST Oscillator", "kst-" + std::to_string(roc_periods1) + "-" + std::to_string(roc_periods2) + "-" + std::to_string(roc_periods3) + "-" + std::to_string(roc_periods4) + "-" + std::to_string(sma_periods1) + "-" + std::to_string(sma_periods2) + "-" + std::to_string(sma_periods3) + "-" + std::to_string(sma_periods4) + "-" + std::to_string(signal_periods) + "-" + std::to_string(offset),
                offset),
      roc_periods1(roc_periods1), roc_periods2(roc_periods2), roc_periods3(roc_periods3), roc_periods4(roc_periods4),
      sma_periods1(sma_periods1), sma_periods2(sma_periods2), sma_periods3(sma_periods3), sma_periods4(sma_periods4),
      signal_periods(signal_periods) {}

/**
 * @brief Calculate the KST Oscillator values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> KSTOscillator::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles)
        {
            ROC roc1(roc_periods1);
            ROC roc2(roc_periods2);
            ROC roc3(roc_periods3);
            ROC roc4(roc_periods4);

            std::vector<double> roc1_values = roc1.calculate(candles, false);
            std::vector<double> roc2_values = roc2.calculate(candles, false);
            std::vector<double> roc3_values = roc3.calculate(candles, false);
            std::vector<double> roc4_values = roc4.calculate(candles, false);

            std::vector<double> kst_values;
            kst_values.reserve(candles.size());

            for (size_t i = 0; i < candles.size(); ++i)
            {
                double value = roc1_values[i] * 1.0 + roc2_values[i] * 2.0 + roc3_values[i] * 3.0 + roc4_values[i] * 4.0;
                kst_values.push_back(value);
            }

            std::vector<double> sma_values = calculate_simple_moving_average(kst_values, sma_periods1);
            sma_values = add_vectors(sma_values, calculate_simple_moving_average(kst_values, sma_periods2));
            sma_values = add_vectors(sma_values, calculate_simple_moving_average(kst_values, sma_periods3));
            sma_values = add_vectors(sma_values, calculate_simple_moving_average(kst_values, sma_periods4));

            // divide vector by scalar
            for (size_t i = 0; i < sma_values.size(); ++i)
            {
                sma_values[i] /= 4.0;
            }

            return kst_values; },

        normalize_data);
}

// *********************************************************************************************

/**
 * @brief Construct a new MassIndex object.
 *
 * @param offset Offset value. Default is 0.
 */
MI::MI(int offset) : Indicator("Mass Index", "mass-index-" + std::to_string(offset), offset) {}

/**
 * @brief Calculate the Mass Index values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> MI::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles)
        {
            std::vector<double> high_low_diff;
            high_low_diff.reserve(candles.size());

            for (size_t i = 0; i < candles.size(); ++i) {
                high_low_diff.push_back(candles[i].high - candles[i].low);
            }

            std::vector<double> exponential_ma = calculate_exponential_moving_average(high_low_diff, 9);

            std::vector<double> ratio;
            ratio.reserve(candles.size());

            for (size_t i = 0; i < candles.size(); ++i) {
                ratio.push_back(high_low_diff[i] / exponential_ma[i]);
            }

            std::vector<double> sum;
            sum.reserve(candles.size());

            double sum_value = 0.0;
            for (size_t i = 0; i < candles.size(); ++i) {
                sum_value += ratio[i];
                sum.push_back(sum_value);
            }

            std::vector<double> mass_index;
            mass_index.reserve(candles.size());

            for (size_t i = 0; i < candles.size(); ++i) {
                if (i >= 25) {
                    mass_index.push_back(sum[i] - sum[i - 25]);
                } else {
                    mass_index.push_back(0.0);
                }
            }

            return mass_index; },

        normalize_data);
}

// *********************************************************************************************

/**
 * @brief Construct a new ParabolicSAR object.
 *
 * @param acceleration_factor Initial acceleration factor value. Default is 0.02.
 * @param acceleration_factor_max Maximum acceleration factor value. Default is 0.2.
 * @param offset Offset value. Default is 0.
 */
PSAR::PSAR(double acceleration_factor, double acceleration_factor_max, int offset)
    : Indicator("Parabolic SAR", "parabolic-sar-" + std::to_string(acceleration_factor) + "-" + std::to_string(acceleration_factor_max) + "-" + std::to_string(offset), offset),
      acceleration_factor(acceleration_factor), acceleration_factor_max(acceleration_factor_max) {}

/**
 * @brief Calculate the Parabolic SAR values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing the Parabolic SAR values.
 */
std::vector<double> PSAR::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles)
        {
            std::vector<double> sar_values;
            sar_values.reserve(candles.size());

            if (candles.empty())
            {
                return sar_values; // Return an empty vector if input candles are empty
            }

            double af = acceleration_factor;        // Initialize acceleration factor
            double sar = candles[0].low;            // Initialize SAR value with the low of the first candle
            double extreme_point = candles[0].high; // Initialize extreme point with the high of the first candle
            bool uptrend = true;                    // Initialize uptrend flag

            // Calculate SAR values for each candle
            for (size_t i = 1; i < candles.size(); ++i)
            {
                // Update the SAR value
                sar_values.push_back(sar);

                // Update the extreme point if necessary
                if (uptrend && candles[i].high > extreme_point)
                {
                    extreme_point = candles[i].high;
                }
                else if (!uptrend && candles[i].low < extreme_point)
                {
                    extreme_point = candles[i].low;
                }

                // Calculate the SAR for the next period
                if (uptrend)
                {
                    if (candles[i].low < sar)
                    {
                        sar = extreme_point;
                        extreme_point = candles[i].low;
                        af = std::min(af + acceleration_factor, acceleration_factor_max);
                    }
                    else
                    {
                        sar += af * (extreme_point - sar);
                        sar = std::min(sar, candles[i].low);
                    }
                    if (candles[i].high > candles[i - 1].high)
                    {
                        uptrend = true;
                    }
                    else
                    {
                        uptrend = false;
                        sar = extreme_point;
                        extreme_point = candles[i].low;
                        af = acceleration_factor;
                    }
                }
                else
                {
                    if (candles[i].high > sar)
                    {
                        sar = extreme_point;
                        extreme_point = candles[i].high;
                        af = std::min(af + acceleration_factor, acceleration_factor_max);
                    }
                    else
                    {
                        sar += af * (extreme_point - sar);
                        sar = std::max(sar, candles[i].high);
                    }
                    if (candles[i].low < candles[i - 1].low)
                    {
                        uptrend = false;
                    }
                    else
                    {
                        uptrend = true;
                        sar = extreme_point;
                        extreme_point = candles[i].high;
                        af = acceleration_factor;
                    }
                }
            }

            // Add the SAR value for the last candle
            sar_values.push_back(sar);

            return sar_values; },

        normalize_data);
}

// *********************************************************************************************

/**
 * @brief Construct a new SMA object.
 *
 * @param source Source value.
 * @param period Period for the SMA calculation.
 * @param offset Offset value. Default is 0.
 */
SMA::SMA(CandleSource source, int period, int offset) : Indicator("Simple Moving Average", "sma-" + CandleSourceMap[source] + "-" + std::to_string(period) + "-" + std::to_string(offset), offset), period(period) {}

/**
 * @brief Calculate the Simple Moving Average (SMA).
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing the calculated SMA values.
 */
std::vector<double> SMA::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles)
        {
            std::vector<double> sma_values;
            sma_values.reserve(candles.size());

            if (candles.size() < static_cast<size_t>(period))
            {
                return sma_values; // Return an empty vector if there are not enough candles
            }

            std::vector<double> source_values = get_candles_with_source(candles, CandleSource::Close); // Extract close prices from candles

            // Calculate the initial SMA as the average of the previous candles
            for (int i = 0; i < period; ++i)
            {
                double sum = 0.0;
                for (int j = i; j < period; ++j)
                {
                    sum += source_values[j];
                }
                sma_values.push_back(sum / period);
            }

            // Calculate SMA for subsequent periods
            for (size_t i = period; i < source_values.size(); ++i)
            {
                double sum = 0.0;
                for (size_t j = i - period; j < i; ++j)
                {
                    sum += source_values[j];
                }
                sma_values.push_back(sum / period); // Calculate and store the SMA
            }

            return sma_values; },

        normalize_data);
}

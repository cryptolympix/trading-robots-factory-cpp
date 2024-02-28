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
 * @param adx_period Period value. Default is 14.
 * @param offset Offset value. Default is 0.
 */
ADX::ADX(int adx_period, int offset) : Indicator("Average Directional Movement Index", "adx-" + std::to_string(adx_period) + "-" + std::to_string(offset), offset), adx_period(adx_period) {}

/**
 * @brief Calculate the Average Directional Index (ADX).
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing ADX values.
 */
std::vector<double> ADX::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles) -> std::vector<double>
        {
             std::vector<double> adx_values(candles.size(), 0.0); // Initialize adx_values with the same size as input vectors

            if (candles.size() < static_cast<size_t>(adx_period))
            {
                return adx_values; // Return an empty vector if there are not enough candles
            }

            std::vector<double> dm_plus;
            std::vector<double> dm_minus;
            std::vector<double> tr_values;

            for (size_t i = 1; i < candles.size(); ++i)
            {
                double dm_plus_value = (candles[i].high - candles[i - 1].high) > (candles[i - 1].low - candles[i].low) ? std::max(candles[i].high - candles[i - 1].high, 0.0) : 0.0;
                double dm_minus_value = (candles[i - 1].low - candles[i].low) > (candles[i].high - candles[i - 1].high) ? std::max(candles[i - 1].low - candles[i].low, 0.0) : 0.0;
                dm_plus.push_back(dm_plus_value);
                dm_minus.push_back(dm_minus_value);

                double tr = std::max({candles[i].high - candles[i].low, std::abs(candles[i].high - candles[i - 1].close), std::abs(candles[i].low - candles[i - 1].close)});
                tr_values.push_back(tr);
            }

            std::vector<double> smoothed_dm_plus = calculate_smoothed(dm_plus);
            std::vector<double> smoothed_dm_minus = calculate_smoothed(dm_minus);
            std::vector<double> smoothed_tr = calculate_smoothed(tr_values);

            std::vector<double> di_plus = calculate_directional_index(smoothed_dm_plus, smoothed_tr);
            std::vector<double> di_minus = calculate_directional_index(smoothed_dm_minus, smoothed_tr);

            std::vector<double> dx_values = calculate_dx(di_plus, di_minus);
            std::vector<double> adx_values_partial = calculate_adx(dx_values);

            for (size_t i = candles.size() - adx_period; i < candles.size(); ++i)
            {
                adx_values[i] = adx_values_partial[i - (candles.size() - adx_period)];
            }

            return adx_values; },

        normalize_data);
}

/**
 * @brief Calculate the smoothed values.
 *
 * @param values Vector containing the values to be smoothed.
 * @return std::vector<double> Vector containing the smoothed values.
 */
std::vector<double> ADX::calculate_smoothed(const std::vector<double> &values) const
{
    std::vector<double> smoothed_values;
    double sum = 0.0;
    for (size_t i = 0; i < adx_period; ++i)
    {
        sum += values[i];
    }
    double first_smoothed = sum / adx_period;
    smoothed_values.push_back(first_smoothed);

    for (size_t i = adx_period; i < values.size(); ++i)
    {
        double smoothed = smoothed_values.back() - (smoothed_values.back() / adx_period) + values[i];
        smoothed_values.push_back(smoothed);
    }

    return smoothed_values;
}

/**
 * @brief Calculate the directional index values.
 *
 * @param smoothed_dm Vector containing smoothed directional movement values.
 * @param smoothed_tr Vector containing smoothed true range values.
 * @return std::vector<double> Vector containing the calculated directional index values.
 */
std::vector<double> ADX::calculate_directional_index(const std::vector<double> &smoothed_dm, const std::vector<double> &smoothed_tr) const
{
    std::vector<double> di_values;
    for (size_t i = 0; i < smoothed_dm.size(); ++i)
    {
        double di = (smoothed_dm[i] / smoothed_tr[i]) * 100.0;
        di_values.push_back(di);
    }
    return di_values;
}

/**
 * @brief Calculate the DX values.
 *
 * @param di_plus Vector containing Plus Directional Indicator values.
 * @param di_minus Vector containing Minus Directional Indicator values.
 * @return std::vector<double> Vector containing the calculated DX values.
 */
std::vector<double> ADX::calculate_dx(const std::vector<double> &di_plus, const std::vector<double> &di_minus) const
{
    std::vector<double> dx_values;
    for (size_t i = 0; i < di_plus.size(); ++i)
    {
        double dx = (std::abs(di_plus[i] - di_minus[i]) / (di_plus[i] + di_minus[i])) * 100.0;
        dx_values.push_back(dx);
    }
    return dx_values;
}

/**
 * @brief Calculate the ADX values.
 *
 * @param dx_values Vector containing DX values.
 * @return std::vector<double> Vector containing the calculated ADX values.
 */
std::vector<double> ADX::calculate_adx(const std::vector<double> &dx_values) const
{
    std::vector<double> adx_values;
    double sum_dx = 0.0;
    for (size_t i = 0; i < adx_period; ++i)
    {
        sum_dx += dx_values[i];
    }
    double first_adx = sum_dx / adx_period;
    adx_values.push_back(first_adx);

    for (size_t i = adx_period; i < dx_values.size(); ++i)
    {
        double adx = ((adx_values.back() * (adx_period - 1)) + dx_values[i]) / adx_period;
        adx_values.push_back(adx);
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

            std::vector<double> partial_values;
            partial_values.reserve(candles.size() - period + 1);

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
                    partial_values.push_back(1.0);
                }
                else if (aroon_up < aroon_down)
                {
                    partial_values.push_back(-1.0);
                }
                else
                {
                    partial_values.push_back(0.0);
                }
            }

            std::vector<double> values(candles.size(), 0.0); // Initialize values with the same size as input vectors
            for (size_t i = 0; i < partial_values.size(); ++i)
            {
                values[i + period - 1] = partial_values[i];
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

            std::vector<double> cci_values(candles.size(), 0.0); // Initialize cci_values with the same size as input vectors

            for (size_t i = period - 1; i < sma_values.size(); ++i)
            {
                double typical_price = typical_prices[i];
                double sma = sma_values[i];
                double mean_deviation = mean_deviation_values[i];
                double cci = (typical_price - sma) / (0.015 * mean_deviation);
                cci_values[i] = cci;
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
 * @brief Construct a new MACD object.
 *
 * @param short_period Short EMA period. Default is 12.
 * @param long_period Long EMA period. Default is 26.
 * @param signal_period Signal EMA period. Default is 9.
 * @param offset Offset value. Default is 0.
 */
MACD::MACD(int short_period, int long_period, int signal_period, int offset) : Indicator("Moving Average Convergence Divergence", "macd-" + std::to_string(short_period) + "-" + std::to_string(long_period) + "-" + std::to_string(signal_period) + "-" + std::to_string(offset), offset),
                                                                               short_period(short_period), long_period(long_period), signal_period(signal_period) {}

/**
 * @brief Calculate the MACD line and signal line.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing MACD line.
 * @details The MACD line is calculated as the difference between the short EMA and the long EMA.
 */
std::vector<double> MACD::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles) -> std::vector<double>
        {
            std::vector<double> closes = get_candles_with_source(candles, CandleSource::Close);

            std::vector<double> macd_line = calculate_macd_line(closes);
            std::vector<double> signal_line = calculate_signal_line(macd_line);

            std::vector<double> result(candles.size(), 0.0);
            for (size_t i = 0; i < candles.size(); ++i) {
                result[i] = macd_line[i];
            }

            return result; },

        normalize_data);
}

/**
 * @brief Calculate the MACD line.
 *
 * @param closes Vector containing close values.
 * @return std::vector<double> Vector containing MACD line.
 */
std::vector<double> MACD::calculate_macd_line(const std::vector<double> &closes) const
{
    // Calculate short EMA
    std::vector<double> short_ema = calculate_exponential_moving_average(closes, short_period);
    // Calculate long EMA
    std::vector<double> long_ema = calculate_exponential_moving_average(closes, long_period);

    // Calculate MACD line (difference between short EMA and long EMA)
    std::vector<double> macd_line;
    for (size_t i = 0; i < closes.size(); ++i)
    {
        macd_line.push_back(short_ema[i] - long_ema[i]);
    }
    return macd_line;
}

/**
 * @brief Calculate the signal line.
 *
 * @param macd_line Vector containing MACD line.
 * @return std::vector<double> Vector containing signal line.
 */
std::vector<double> MACD::calculate_signal_line(const std::vector<double> &macd_line) const
{
    // Calculate signal line (EMA of MACD line)
    return calculate_exponential_moving_average(macd_line, signal_period);
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

// *********************************************************************************************

/**
 * @brief Construct a new STC object.
 *
 * @param short_period Short period for MACD calculation.
 * @param long_period Long period for MACD calculation.
 * @param signal_period Signal period for MACD calculation.
 * @param smooth_period Period for smoothing.
 * @param offset Offset value. Default is 0.
 */
STC::STC(int short_period, int long_period, int signal_period, int smooth_period, int offset) : Indicator("Schaff Trend Cycle", "stc-" + std::to_string(short_period) + "-" + std::to_string(long_period) + "-" + std::to_string(signal_period) + "-" + std::to_string(smooth_period) + "-" + std::to_string(offset), offset),
                                                                                                short_period(short_period), long_period(long_period), signal_period(signal_period), smooth_period(smooth_period) {}

/**
 * @brief Calculate the Schaff Trend Cycle (STC) values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> STC::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](const std::vector<Candle> &candles) -> std::vector<double>
        {
                std::vector<double> result(candles.size(), 0.0);

                // Calculate MACD using short and long periods
                MACD macd_indicator(short_period, long_period, signal_period);
                std::vector<double> macd = macd_indicator.calculate(candles, false);

                // Calculate Signal line using signal period
                std::vector<double> signal_line = calculate_simple_moving_average(macd, signal_period);

                // Smooth the Signal line using smooth period
                std::vector<double> smooth_signal = calculate_simple_moving_average(signal_line, smooth_period);

                // Apply the smoothing algorithm for the final STC value
                for (size_t i = smooth_period; i < candles.size(); ++i) {
                    result[i] = 0.5 * smooth_signal[i] + 0.5 * smooth_signal[i - 1];
                }

                return result; },

        normalize_data);
}

// *********************************************************************************************

/**
 * @brief Construct a new TRIX object.
 *
 * @param period Period for TRIX calculation.
 * @param offset Offset value. Default is 0.
 */
TRIX::TRIX(int period, int offset) : Indicator("TRIX", "trix-" + std::to_string(period) + "-" + std::to_string(offset), offset), period(period) {}

/**
 * @brief Calculate the TRIX values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<Candle> Vector containing calculated values.
 */
std::vector<double> TRIX::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](const std::vector<Candle> &candles) -> std::vector<double>
        {
                std::vector<double> closes = get_candles_with_source(candles, CandleSource::Close);
                std::vector<double> ema1 = calculate_exponential_moving_average(closes, period);
                std::vector<double> ema2 = calculate_exponential_moving_average(ema1, period);
                std::vector<double> ema3 = calculate_exponential_moving_average(ema2, period);

                std::vector<double> trix(closes.size(), 0.0);

                // Calculate TRIX values
                for (size_t i = period * 3 - 1; i < closes.size(); ++i) {
                    trix[i] = (ema3[i] - ema3[i - 1]) / ema3[i - 1] * 100.0;
                }

                return trix; },

        normalize_data);
}

// *********************************************************************************************

/**
 * @brief Construct a new InstitutionalBias object.
 *
 * @param short_period Period for short EMA calculation.
 * @param long_period Period for long EMA calculation.
 * @param offset Offset value. Default is 0.
 */
InstitutionalBias::InstitutionalBias(int short_period, int long_period, int offset) : Indicator("Institutional Bias", "institutional-bias-" + std::to_string(short_period) + "-" + std::to_string(long_period) + "-" + std::to_string(offset), offset),
                                                                                      short_period(short_period), long_period(long_period) {}

/**
 * @brief Calculate the Institutional Bias values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<int> Vector containing calculated values. 1 for short EMA > long EMA, -1 otherwise.
 */
std::vector<double> InstitutionalBias::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](const std::vector<Candle> &candles) -> std::vector<double>
        {
            std::vector<double> closes = get_candles_with_source(candles, CandleSource::Close);
            std::vector<double> short_ema = calculate_exponential_moving_average(closes, short_period);
            std::vector<double> long_ema = calculate_exponential_moving_average(closes, long_period);

            std::vector<double> institutional_bias(candles.size(), 0.0);

            // Calculate Institutional Bias values
            for (size_t i = 0; i < candles.size(); ++i) {
                if (short_ema[i] > long_ema[i]) {
                    institutional_bias[i] = 1.0;
                } else if (short_ema[i] < long_ema[i]){
                    institutional_bias[i] = -1.0;
                } else {
                    institutional_bias[i] = 0.0;
                }
            }

            return institutional_bias; },

        normalize_data);
}

// *********************************************************************************************

/**
 * @brief Construct a new EMADifference object.
 *
 * @param short_period Period for short EMA calculation. Default is 9.
 * @param long_period Period for long EMA calculation. Default is 18.
 * @param offset Offset value. Default is 0.
 */
EMADifference::EMADifference(int short_period, int long_period, int offset) : Indicator("EMA Difference", "ema-difference-" + std::to_string(short_period) + "-" + std::to_string(long_period) + "-" + std::to_string(offset), offset),
                                                                              short_period(short_period), long_period(long_period) {}

/**
 * @brief Calculate the difference between two exponential moving averages.
 *
 * @param closes Vector of closing prices.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing the calculated difference values.
 */
std::vector<double> EMADifference::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](const std::vector<Candle> &candles) -> std::vector<double>
        {
            std::vector<double> closes = get_candles_with_source(candles, CandleSource::Close);
            std::vector<double> short_ema = calculate_exponential_moving_average(closes, short_period);
            std::vector<double> long_ema = calculate_exponential_moving_average(closes, long_period);

            std::vector<double> ema_difference(candles.size(), 0.0);

            // Calculate EMA Difference values
            for (size_t i = 0; i < candles.size(); ++i) {
                ema_difference[i] = short_ema[i] - long_ema[i];
            }

            return ema_difference; },

        normalize_data);
}

// *********************************************************************************************

/**
 * @brief Construct a new IchimokuCloudTrend object.
 *
 * @param offset Offset value. Default is 0.
 */
IchimokuCloudTrend::IchimokuCloudTrend(int offset) : Indicator("Ichimoku Cloud Trend", "ichimoku-trend-" + std::to_string(offset), offset) {}

/**
 * @brief Determine the trend based on Ichimoku Cloud.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing the trend values:
 *         1 if close price is above the cloud,
 *        -1 if close price is below the cloud,
 *         0 if close price is in the cloud.
 */
std::vector<double> IchimokuCloudTrend::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles) -> std::vector<double>
        {
            std::vector<double> trend(candles.size());

            std::vector<double> high_prices = get_candles_with_source(candles, CandleSource::High);
            std::vector<double> low_prices = get_candles_with_source(candles, CandleSource::Low);

            std::vector<double> tenkan_sen = calculate_tenkan_sen(high_prices);
            std::vector<double> kijun_sen = calculate_kijun_sen(high_prices);
            std::vector<double> senkou_span_a = calculate_senkou_span_a(tenkan_sen, kijun_sen);
            std::vector<double> senkou_span_b = calculate_senkou_span_b(candles);

            for (size_t i = 0; i < candles.size(); ++i) {
                if (candles[i].close > senkou_span_a[i] && candles[i].close > senkou_span_b[i]) {
                    trend[i] = 1.0; // Close price is above the cloud
                } else if (candles[i].close < senkou_span_a[i] && candles[i].close < senkou_span_b[i]) {
                    trend[i] = -1.0; // Close price is below the cloud
                } else {
                    trend[i] = 0.0; // Close price is in the cloud
                }
            }

            return trend; },

        normalize_data);
}

/**
 * @brief Calculate the Tenkan-sen values.
 *
 * @param high_prices Vector containing high prices.
 * @return std::vector<double> Vector containing the calculated Tenkan-sen values.
 */
std::vector<double> IchimokuCloudTrend::calculate_tenkan_sen(const std::vector<double> &high_prices) const
{
    // Calculate Tenkan-sen (Conversion Line)
    return calculate_simple_moving_average(high_prices, 9);
}

/**
 * @brief Calculate the Kijun-sen values.
 *
 * @param high_prices Vector containing high prices.
 * @return std::vector<double> Vector containing the calculated Kijun-sen values.
 */
std::vector<double> IchimokuCloudTrend::calculate_kijun_sen(const std::vector<double> &high_prices) const
{
    // Calculate Kijun-sen (Base Line)
    return calculate_simple_moving_average(high_prices, 26);
}

/**
 * @brief Calculate the Senkou Span A values.
 *
 * @param tenkan_sen Vector containing Tenkan-sen values.
 * @param kijun_sen Vector containing Kijun-sen values.
 * @return std::vector<double> Vector containing the calculated Senkou Span A values.
 */
std::vector<double> IchimokuCloudTrend::calculate_senkou_span_a(const std::vector<double> &tenkan_sen,
                                                                const std::vector<double> &kijun_sen) const
{
    // Calculate Senkou Span A (Leading Span A)
    std::vector<double> senkou_span_a(tenkan_sen.size());
    for (size_t i = 0; i < tenkan_sen.size(); ++i)
    {
        senkou_span_a[i] = (tenkan_sen[i] + kijun_sen[i]) / 2.0;
    }
    return senkou_span_a;
}

/**
 * @brief Calculate the Senkou Span B values.
 *
 * @param candles Vector of Candle data.
 * @return std::vector<double> Vector containing the calculated Senkou Span B values.
 */
std::vector<double> IchimokuCloudTrend::calculate_senkou_span_b(const std::vector<Candle> &candles) const
{
    // Calculate Senkou Span B (Leading Span B)
    std::vector<double> close_prices(candles.size());
    for (size_t i = 0; i < candles.size(); ++i)
    {
        close_prices[i] = candles[i].close;
    }
    std::vector<double> senkou_span_b = calculate_simple_moving_average(close_prices, 52);

    // Shift Senkou Span B 26 periods ahead
    std::rotate(senkou_span_b.begin(), senkou_span_b.begin() + 26, senkou_span_b.end());

    return senkou_span_b;
}

// *********************************************************************************************

/**
 * @brief Construct a new IchimokuTenkanKijunTrend object.
 *
 * @param offset Offset value. Default is 0.
 */
IchimokuTenkanKijunTrend::IchimokuTenkanKijunTrend(int offset) : Indicator("Ichimoku Tenkan-Kijun Trend", "tenkan-kijun-trend-" + std::to_string(offset), offset) {}

/**
 * @brief Determine the trend based on the relationship between Tenkan-sen (Conversion Line) and Kijun-sen (Base Line).
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<int> Vector containing the trend values:
 *         1 if close price is above both Tenkan-sen and Kijun-sen,
 *        -1 if close price is below both Tenkan-sen and Kijun-sen,
 *         0 if close price is between Tenkan-sen and Kijun-sen.
 */
std::vector<double> IchimokuTenkanKijunTrend::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles) -> std::vector<double>
        {
            std::vector<double> trend(candles.size());

            std::vector<double> high_prices = get_candles_with_source(candles, CandleSource::High);
            std::vector<double> low_prices = get_candles_with_source(candles, CandleSource::Low);

            std::vector<double> tenkan_sen = calculate_tenkan_sen(high_prices);
            std::vector<double> kijun_sen = calculate_kijun_sen(high_prices);

            for (size_t i = 0; i < candles.size(); ++i) {
                if (candles[i].close > tenkan_sen[i] && candles[i].close > kijun_sen[i]) {
                    trend[i] = 1.0; // Close price is above both Tenkan-sen and Kijun-sen
                } else if (candles[i].close < tenkan_sen[i] && candles[i].close < kijun_sen[i]) {
                    trend[i] = -1.0; // Close price is below both Tenkan-sen and Kijun-sen
                } else {
                    trend[i] = 0.0; // Close price is between Tenkan-sen and Kijun-sen
                }
            }

            return trend; },

        normalize_data);
}

/**
 * @brief Calculate the Tenkan-sen values.
 *
 * @param high_prices Vector containing high prices.
 * @return std::vector<double> Vector containing the calculated Tenkan-sen values.
 */
std::vector<double> IchimokuTenkanKijunTrend::calculate_tenkan_sen(const std::vector<double> &high_prices) const
{
    // Calculate Tenkan-sen (Conversion Line)
    return calculate_simple_moving_average(high_prices, 9);
}

/**
 * @brief Calculate the Kijun-sen values.
 *
 * @param high_prices Vector containing high prices.
 * @return std::vector<double> Vector containing the calculated Kijun-sen values.
 */
std::vector<double> IchimokuTenkanKijunTrend::calculate_kijun_sen(const std::vector<double> &high_prices) const
{
    // Calculate Kijun-sen (Base Line)
    return calculate_simple_moving_average(high_prices, 26);
}

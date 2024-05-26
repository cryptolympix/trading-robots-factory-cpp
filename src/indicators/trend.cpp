#include <vector>
#include <stdexcept>
#include <numeric>
#include <cmath>
#include "../types.hpp"
#include "../utils/math.hpp"
#include "../utils/candles_source.hpp"
#include "../utils/vectors.hpp"
#include "utils.hpp"
#include "indicator.hpp"
#include "candle.hpp"
#include "trend.hpp"

/**
 * @brief Construct a new Average Directional Movement Index object.
 *
 * @param adx_period Period value. Default is 14.
 * @param offset Offset value. Default is 0.
 */
ADX::ADX(int adx_period, int offset) : Indicator("Average Directional Movement Index", "adx-" + std::to_string(adx_period) + "-" + std::to_string(offset), offset, {0, 100}), adx_period(adx_period) {}

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
                std::cerr << "Insufficient data to calculate ADX." << std::endl;
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

                double tr = std::max({ candles[i].high - candles[i].low, std::abs(candles[i].high - candles[i - 1].close), std::abs(candles[i].low - candles[i - 1].close) });
                tr_values.push_back(tr);
            }

            std::vector<double> smoothed_dm_plus = calculate_smoothed(dm_plus);
            std::vector<double> smoothed_dm_minus = calculate_smoothed(dm_minus);
            std::vector<double> smoothed_tr = calculate_smoothed(tr_values);

            std::vector<double> di_plus = calculate_directional_index(smoothed_dm_plus, smoothed_tr);
            std::vector<double> di_minus = calculate_directional_index(smoothed_dm_minus, smoothed_tr);

            std::vector<double> dx_values = calculate_dx(di_plus, di_minus);
            std::vector<double> adx_values_partial = calculate_adx(dx_values);

            // Fill the last candles with the calculated ADX values    
            for (size_t i = candles.size() - adx_values_partial.size(); i < candles.size(); ++i)
            {
                adx_values[i] = adx_values_partial[i];
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
        double di = smoothed_tr[i] != 0 ? (smoothed_dm[i] / smoothed_tr[i]) * 100.0 : 0.0;
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
        double dx = di_plus[i] + di_minus[i] != 0 ? (std::abs(di_plus[i] - di_minus[i]) / (di_plus[i] + di_minus[i])) * 100.0 : 0.0;
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
 * @brief Construct a new Aroon Up object.
 *
 * @param period Period value. Default is 14.
 * @param offset Offset value. Default is 0.
 */
AroonUp::AroonUp(int period, int offset) : Indicator("Aroon Up", "aroon-up-" + std::to_string(period) + "-" + std::to_string(offset), offset, {0, 100}), period(period) {}

/**
 * @brief Calculate the Aroon Up values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> AroonUp::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles) -> std::vector<double>
        {
            // Initialize aroon_up_values with the same size as input vectors
            std::vector<double> aroon_up_values(candles.size(), 0.0);

            if (candles.size() < static_cast<size_t>(period))
            {
                std::cerr << "Insufficient data to calculate Aroon Up." << std::endl;
                return aroon_up_values; // Not enough data
            }

            HighestHigh highest_source(period, 0);
            std::vector<double> highest_highs = highest_source.calculate(candles, false);

            for (size_t i = period - 1; i < candles.size(); ++i)
            {
                double highest_high = highest_highs[i];

                // Calculate days since highest high
                double days_since_high = 0;
                for (size_t j = i; j > 0; --j)
                {
                    if (candles[j].high == highest_high)
                    {
                        days_since_high = i - j;
                        break;
                    }
                }

                aroon_up_values[i] = ((period - days_since_high) / period) * 100.0;
            }

            return aroon_up_values; },

        normalize_data);
}

// *********************************************************************************************

/**
 * @brief Construct a new AroonDown object.
 *
 * @param period Period value. Default is 14.
 * @param offset Offset value. Default is 0.
 */
AroonDown::AroonDown(int period, int offset) : Indicator("Aroon Down", "aroon-down-" + std::to_string(period) + "-" + std::to_string(offset), offset, {0, 100}), period(period) {}

/**
 * @brief Calculate the Aroon Down values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> AroonDown::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles) -> std::vector<double>
        {
            std::vector<double> aroon_down_values(candles.size(), 0.0); // Initialize aroon_trend_values with the same size as input vectors

            if (candles.size() < static_cast<size_t>(period))
            {
                std::cerr << "Insufficient data to calculate Aroon Down." << std::endl;
                return aroon_down_values; // Not enough data
            }

            LowestLow lowest_source(period, 0);
            std::vector<double> lowest_lows = lowest_source.calculate(candles, false);

            for (size_t i = period - 1; i < candles.size(); ++i)
            {
                double lowest_low = lowest_lows[i];

                // Calculate days since lowest low
                double days_since_low = 0;
                for (size_t j = i; j > 0; --j)
                {
                    if (candles[j].low == lowest_low)
                    {
                        days_since_low = i - j;
                        break;
                    }
                }

                aroon_down_values[i] = ((period - days_since_low) / period) * 100.0;
            }

            return aroon_down_values; },

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
            // Initialize cci_values with the same size as input vectors
            std::vector<double> cci_values(candles.size(), 0.0);

            if (candles.size() < static_cast<size_t>(period))
            {
                std::cerr << "Insufficient data to calculate CCI." << std::endl;
                return cci_values; // Not enough data
            }

            std::vector<double> typical_prices = get_candles_with_source(candles, "hlc3");
            std::vector<double> sma_values = calculate_simple_moving_average(typical_prices, period);
            std::vector<double> mean_deviation_values = calculate_mean_deviation(typical_prices, sma_values);

            for (size_t i = period - 1; i < sma_values.size(); ++i)
            {
                double typical_price = typical_prices[i];
                double sma = sma_values[i];
                double mean_deviation = mean_deviation_values[i];
                double cci = mean_deviation != 0.0 ? (typical_price - sma) / (0.015 * mean_deviation) : 0.0;
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
            // Initialize dpo_values with the same size as input vectors
            std::vector<double> dpo_values(candles.size(), 0.0);

            if (candles.size() < static_cast<size_t>(period + offset))
            {
                std::cerr << "Insufficient data to calculate DPO." << std::endl;
                return dpo_values; // Not enough data
            }

            // Calculate the X-period simple moving average
            SMA sma("close", period);
            std::vector<double> sma_values = sma.calculate(candles, false);

            // Calculate the Detrended Price Oscillator (DPO)
            for (size_t i = period / 2 + offset; i < candles.size(); ++i)
            {
                if (i < period + offset)
                {
                    continue;
                } 
                double dpo = candles[i - period / 2 - 1].close - sma_values[i - period + offset];
                dpo_values[i] = dpo;
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
EMA::EMA(std::string source, int period, int offset) : Indicator("Exponential Moving Average", "ema-" + source + "-" + std::to_string(period) + "-" + std::to_string(offset), offset), source(source), period(period) {}

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
            std::vector<double> ema_values(candles.size(), 0.0); // Initialize ema_values with the same size as input vectors

            if (candles.size() < static_cast<size_t>(period))
            {
                return ema_values; // Return an empty vector if there are not enough candles
            }

            ema_values = calculate_exponential_moving_average(get_candles_with_source(candles, source), period);
            return ema_values; },

        normalize_data);
}

// *********************************************************************************************

/**
 * @brief Construct a new KST Oscillator object.
 *
 * @param roc_periods1 Periods for Rate of Change (ROC) 1. Default is 10.
 * @param roc_periods2 Periods for Rate of Change (ROC) 2. Default is 15.
 * @param roc_periods3 Periods for Rate of Change (ROC) 3. Default is 20.
 * @param roc_periods4 Periods for Rate of Change (ROC) 4. Default is 30.
 * @param sma_periods1 Periods for Simple Moving Average (SMA) 1. Default is 10.
 * @param sma_periods2 Periods for Simple Moving Average (SMA) 2. Default is 10.
 * @param sma_periods3 Periods for Simple Moving Average (SMA) 3. Default is 10.
 * @param sma_periods4 Periods for Simple Moving Average (SMA) 4. Default is 15.
 * @param signal_periods Periods for the signal line. Default is 9.
 * @param offset Offset value. Default is 0.
 */
KST::KST(int roc_periods1, int roc_periods2, int roc_periods3, int roc_periods4,
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
std::vector<double> KST::calculate(const std::vector<Candle> &candles, bool normalize_data) const
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

            std::vector<double> sma1_values = calculate_simple_moving_average(roc1_values, sma_periods1);
            std::vector<double> sma2_values = calculate_simple_moving_average(roc2_values, sma_periods2);
            std::vector<double> sma3_values = calculate_simple_moving_average(roc3_values, sma_periods3);
            std::vector<double> sma4_values = calculate_simple_moving_average(roc4_values, sma_periods4);

            std::vector<double> kst_values(candles.size(), 0.0);

            for (size_t i = 0; i < candles.size(); ++i)
            {
                kst_values[i] = roc1_values[i] * 1.0 + roc2_values[i] * 2.0 + roc3_values[i] * 3.0 + roc4_values[i] * 4.0;
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
            std::vector<double> closes = get_candles_with_source(candles, "close");

            std::vector<double> macd_line = calculate_macd_line(closes);
            std::vector<double> signal_line = calculate_signal_line(macd_line);

            std::vector<double> values(candles.size(), 0.0);

            for (size_t i = 0; i < candles.size(); ++i) {
                values[i] = macd_line[i];
            }

            return values; },

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

            std::vector<double> single_ema = calculate_exponential_moving_average(high_low_diff, 9);
            std::vector<double> double_ema = calculate_exponential_moving_average(single_ema, 9);

            std::vector<double> ratio;
            ratio.reserve(candles.size());

            for (size_t i = 0; i < candles.size(); ++i) {
                ratio.push_back(double_ema[i] > 0 ? single_ema[i] / double_ema[i] : 0.0);
            }

            std::vector<double> mass_index;
            mass_index.reserve(candles.size());

            for (size_t i = 0; i < candles.size(); ++i) {
                double sum = 0.0;
                for (size_t j = i - 25; j <= i; ++j) {
                    if (j >= 0) {
                        sum += ratio[j];
                    }
                }
                mass_index.push_back(sum);
            }

            return mass_index; },

        normalize_data);
}

// *********************************************************************************************

/**
 * @brief Construct a new ParabolicSAR object.
 *
 * @param acceleration_factor_initial Initial acceleration factor value. Default is 0.02.
 * @param acceleration_factor_maximum Maximum acceleration factor value. Default is 0.2.
 * @param offset Offset value. Default is 0.
 */
ParabolicSAR::ParabolicSAR(double acceleration_factor_initial, double acceleration_factor_maximum, int offset)
    : Indicator("Parabolic SAR", "sar", offset), acceleration_factor_initial(acceleration_factor_initial), acceleration_factor_maximum(acceleration_factor_maximum), offset(offset) {}

/**
 * @brief Calculate the Parabolic SAR values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing the Parabolic SAR values.
 */
std::vector<double> ParabolicSAR::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles) -> std::vector<double>
        {
            std::vector<double> sar_values(candles.size(), 0.0);

            if (candles.size() < 2)
            {
                std::cerr << "Insufficient data for Parabolic SAR calculation." << std::endl;
                return sar_values;
            }

            double af = acceleration_factor_initial;
            double ep = 0.0;
            double sar = 0.0;

            // Determine initial SAR direction
            bool uptrend = candles[1].close > candles[0].close;

            // Set initial SAR value
            if (uptrend)
            {
                sar = std::min(candles[0].low, candles[1].low);
            }
            else
            {
                sar = std::max(candles[0].high, candles[1].high);
            }

            for (size_t i = 2; i < candles.size(); ++i)
            {
                // Update SAR
                if (uptrend)
                {
                    if (candles[i].high > ep)
                    {
                        ep = candles[i].high;
                        af = std::min(af + acceleration_factor_initial, acceleration_factor_maximum);
                    }
                    sar = sar + af * (ep - sar);
                    sar = std::min(sar, std::min(candles[i - 1].low, candles[i - 2].low));
                    if (candles[i].low <= sar)
                    {
                        uptrend = false;
                        sar = ep;
                        ep = candles[i].low;
                        af = acceleration_factor_initial;
                    }
                }
                else
                {
                    if (candles[i].low < ep)
                    {
                        ep = candles[i].low;
                        af = std::min(af + acceleration_factor_initial, acceleration_factor_maximum);
                    }
                    sar = sar - af * (sar - ep);
                    sar = std::max(sar, std::max(candles[i - 1].high, candles[i - 2].high));
                    if (candles[i].high >= sar)
                    {
                        uptrend = true;
                        sar = ep;
                        ep = candles[i].high;
                        af = acceleration_factor_initial;
                    }
                }
                sar_values[i] = sar;
            }

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
SMA::SMA(std::string source, int period, int offset) : Indicator("Simple Moving Average", "sma-" + source + "-" + std::to_string(period) + "-" + std::to_string(offset), offset), source(source), period(period) {}

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
            std::vector<double> values = get_candles_with_source(candles, source);
            return calculate_simple_moving_average(values, period); },

        normalize_data);
}

// *********************************************************************************************

STC::STC(int short_length, int long_length, int cycle_length, int offset)
    : Indicator("Schaff Trend Cycle", "stc-" + std::to_string(short_length) + "-" + std::to_string(long_length) + "-" + std::to_string(cycle_length) + "-" + std::to_string(offset), offset),
      short_length(short_length),
      long_length(long_length),
      cycle_length(cycle_length) {}

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
            // Calculate MACD using MACD class
            MACD macd_calculator(short_length, long_length);
            std::vector<double> macd_values = macd_calculator.calculate(candles, false);

            // Calculate %K and %D for the MACD values
            std::vector<double> k_values = calculate_stochastic_oscillator(macd_values, cycle_length);
            std::vector<double> d_values = calculate_exponential_moving_average(k_values, cycle_length);

            // Calculate Schaff Trend Cycle (STC)
            std::vector<double> stc_values(candles.size(), 0.0);
            for (size_t i = 0; i < candles.size(); ++i) {
                double stc = d_values[i] - k_values[i] != 0 ? 100.0 * (macd_values[i] - k_values[i]) / (d_values[i] - k_values[i]) : 0.0;
                stc_values[i] = stc;
            }

            return stc_values; },

        normalize_data);
}

/**
 * @brief Calculate the Stochastic Oscillator values.
 *
 * @param values Vector containing the values to calculate the Stochastic Oscillator.
 * @param period Period for the Stochastic Oscillator calculation.
 * @return std::vector<double> Vector containing the calculated Stochastic Oscillator values.
 */
std::vector<double> STC::calculate_stochastic_oscillator(const std::vector<double> &values, int period) const
{
    std::vector<double> k_values(values.size(), 0.0);

    for (size_t i = period - 1; i < values.size(); ++i)
    {
        double max_value = *std::max_element(values.begin() + i - period + 1, values.begin() + i + 1);
        double min_value = *std::min_element(values.begin() + i - period + 1, values.begin() + i + 1);

        if (max_value != min_value)
        {
            double k = 100.0 * (values[i] - min_value) / (max_value - min_value);
            k_values[i] = k;
        }
        else
        {
            k_values[i] = 0.0; // Handle division by zero case
        }
    }

    return k_values;
}

// *********************************************************************************************

/**
 * @brief Construct a new TRIX object.
 *
 * @param period Period for TRIX calculation. Default is 15.
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
            std::vector<double> closes = get_candles_with_source(candles, "close");
            std::vector<double> ema1 = calculate_exponential_moving_average(closes, period);
            std::vector<double> ema2 = calculate_exponential_moving_average(ema1, period);
            std::vector<double> ema3 = calculate_exponential_moving_average(ema2, period);

            std::vector<double> trix(closes.size(), 0.0);

            // Calculate TRIX values
            for (size_t i = period * 3 - 1; i < closes.size(); ++i) {
                trix[i] = ema3[i - 1] > 0 ? (ema3[i] - ema3[i - 1]) / ema3[i - 1] * 100.0 : 0.0;
            }

            return trix; },

        normalize_data);
}

// *********************************************************************************************

/**
 * @brief Construct a new Vortex Indicator object.
 *
 * @param period The period for calculating Vortex Indicator (default: 14).
 * @param offset Offset value. Default is 0.
 */
Vortex::Vortex(int period, int offset)
    : Indicator("Vortex", "vortex-" + std::to_string(period), offset),
      period(period) {}

/**
 * @brief Calculate the Vortex Indicator values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated Vortex Indicator values.
 */
std::vector<double> Vortex::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    std::vector<double> positive_trend_movement(candles.size(), 0);
    std::vector<double> negative_trend_movement(candles.size(), 0);
    std::vector<double> true_range(candles.size(), 0);

    // Calculate positive and negative trend movements
    for (size_t i = 1; i < candles.size(); ++i)
    {
        double high_low_diff = candles[i].high - candles[i - 1].low;
        double low_high_diff = candles[i].low - candles[i - 1].high;
        positive_trend_movement[i] = std::abs(high_low_diff);
        negative_trend_movement[i] = std::abs(low_high_diff);
    }

    // Calculate True Range
    for (size_t i = 1; i < candles.size(); ++i)
    {
        double high_low_range = candles[i].high - candles[i].low;
        double high_close_diff = std::abs(candles[i].high - candles[i - 1].close);
        double low_close_diff = std::abs(candles[i].low - candles[i - 1].close);
        true_range[i] = std::max({high_low_range, high_close_diff, low_close_diff});
    }

    // Calculate 14-period sums of +VM, -VM, and TR
    std::vector<double> positive_vm_14 = calculate_sum_subvector(positive_trend_movement, period);
    std::vector<double> negative_vm_14 = calculate_sum_subvector(negative_trend_movement, period);
    std::vector<double> tr_14 = calculate_sum_subvector(true_range, period);

    // // Calculate normalized positive and negative trend movements
    std::vector<double> positive_vi_14(candles.size(), 0);
    std::vector<double> negative_vi_14(candles.size(), 0);
    for (size_t i = period; i < candles.size(); ++i)
    {
        positive_vi_14[i] = positive_vm_14[i] / tr_14[i];
        negative_vi_14[i] = negative_vm_14[i] / tr_14[i];
    }

    // // Normalize data if required
    positive_vi_14 = normalize_vector(positive_vi_14);
    negative_vi_14 = normalize_vector(negative_vi_14);

    // // Combine positive and negative VI into one vector for output
    std::vector<double> vortex_indicator(candles.size(), 0);
    for (size_t i = period; i < candles.size(); ++i)
    {
        vortex_indicator[i] = positive_vi_14[i] - negative_vi_14[i];
    }

    return vortex_indicator;
}

// *********************************************************************************************

/**
 * @brief Construct a new InstitutionalBias object.
 *
 * @param short_period Period for short EMA calculation.
 * @param long_period Period for long EMA calculation.
 * @param offset Offset value. Default is 0.
 */
InstitutionalBias::InstitutionalBias(int short_period, int long_period, int offset) : Indicator("Institutional Bias", "institutional-bias-" + std::to_string(short_period) + "-" + std::to_string(long_period) + "-" + std::to_string(offset), offset, {-1, 1}),
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
            std::vector<double> closes = get_candles_with_source(candles, "close");
            std::vector<double> short_ema = calculate_exponential_moving_average(closes, short_period);
            std::vector<double> long_ema = calculate_exponential_moving_average(closes, long_period);

            std::vector<double> institutional_bias(candles.size(), 0.0);

            // Calculate Institutional Bias values
            for (size_t i = 0; i < candles.size(); ++i) {
                if (short_ema[i] > long_ema[i]) {
                    institutional_bias[i] = 1.0;
                }
                else if (short_ema[i] < long_ema[i]) {
                    institutional_bias[i] = -1.0;
                }
                else {
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
            std::vector<double> closes = get_candles_with_source(candles, "close");
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
 * @brief Construct a new AroonTrend object.
 *
 * @param period Period value. Default is 14.
 * @param offset Offset value. Default is 0.
 */
AroonTrend::AroonTrend(int period, int offset) : Indicator("Aroon Trend", "aroon-trend-" + std::to_string(period) + "-" + std::to_string(offset), offset, {-1, 1}), period(period) {}

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
        candles, [this](std::vector<Candle> candles) -> std::vector<double>
        {
            std::vector<double> aroon_trend_values(candles.size(), 0.0); // Initialize aroon_trend_values with the same size as input vectors

            if (candles.size() < static_cast<size_t>(period))
            {
                std::cerr << "Insufficient data to calculate Aroon Trend." << std::endl;
                return aroon_trend_values; // Not enough data
            }

            std::vector<double> highest_highs = HighestHigh(period, 0).calculate(candles, false);
            std::vector<double> lowest_lows = LowestLow(period, 0).calculate(candles, false);

            for (size_t i = period - 1; i < candles.size(); ++i)
            {
                double highest_high = highest_highs[i];
                double lowest_low = lowest_lows[i];

                // Calculate days since highest high and lowest low
                double days_since_high = 0;
                double days_since_low = 0;
                for (size_t j = i; j > 0; --j)
                {
                    if (candles[j].high == highest_high)
                    {
                        days_since_high = i - j;
                        break;
                    }
                }
                for (size_t j = i; j > 0; --j)
                {
                    if (candles[j].low == lowest_low)
                    {
                        days_since_low = i - j;
                        break;
                    }
                }

                double aroon_up = ((period - days_since_high) / period) * 100.0;
                double aroon_down = ((period - days_since_low) / period) * 100.0;

                if (aroon_up > aroon_down)
                {
                    aroon_trend_values[i] = 1;
                }
                else if (aroon_up < aroon_down)
                {
                    aroon_trend_values[i] = -1;
                }
                else
                {
                    aroon_trend_values[i] = 0;
                }
            }

            return aroon_trend_values; },

        normalize_data);
}

// *********************************************************************************************

/**
 * @brief Construct a new Ichimoku Cloud Trend object.
 *
 * @param conversion_period The period for the Conversion Line (Tenkan-sen).
 * @param base_period The period for the Base Line (Kijun-sen).
 * @param lagging_period The period for the Lagging Span (Chikou Span).
 * @param leading_period The period for the Leading Span (Senkou Span).
 * @param offset Offset value. Default is 0.
 */
IchimokuCloudTrend::IchimokuCloudTrend(int conversion_period, int base_period, int lagging_period, int leading_period, int offset)
    : Indicator("Ichimoku Cloud Trend", "ichimoku-cloud-trend-" + std::to_string(conversion_period) + "-" + std::to_string(base_period) + "-" + std::to_string(lagging_period) + "-" + std::to_string(leading_period) + "-" + std::to_string(offset), offset, {-1, 1}),
      conversion_period(conversion_period),
      base_period(base_period),
      lagging_period(lagging_period),
      leading_period(leading_period) {}

/**
 * @brief Calculate the Ichimoku Cloud Trend values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated Ichimoku Cloud values.
 */
std::vector<double> IchimokuCloudTrend::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](const std::vector<Candle> &candles) -> std::vector<double>
        {
            std::vector<double> conversion_line(candles.size(), 0.0);
            std::vector<double> base_line(candles.size(), 0.0);
            std::vector<double> leading_span_a(candles.size(), 0.0);
            std::vector<double> leading_span_b(candles.size(), 0.0);
            std::vector<double> lagging_span(candles.size(), 0.0);

            // Calculate Conversion Line (Tenkan-sen)
            for (size_t i = conversion_period - 1; i < candles.size(); ++i)
            {
                double high_sum = 0.0;
                double low_sum = 0.0;
                for (size_t j = i - (conversion_period - 1); j <= i; ++j)
                {
                    high_sum += candles[j].high;
                    low_sum += candles[j].low;
                }
                double avg_high = high_sum / conversion_period;
                double avg_low = low_sum / conversion_period;
                conversion_line[i] = (avg_high + avg_low) / 2.0;
            }

            // Calculate Base Line (Kijun-sen)
            for (size_t i = base_period - 1; i < candles.size(); ++i)
            {
                double high_sum = 0.0;
                double low_sum = 0.0;
                for (size_t j = i - (base_period - 1); j <= i; ++j)
                {
                    high_sum += candles[j].high;
                    low_sum += candles[j].low;
                }
                double avg_high = high_sum / base_period;
                double avg_low = low_sum / base_period;
                base_line[i] = (avg_high + avg_low) / 2.0;
            }

            // Calculate Leading Span A (Senkou Span A)
            for (size_t i = 0; i < candles.size(); ++i)
            {
                leading_span_a[i] = (conversion_line[i] + base_line[i]) / 2.0;
            }

            // Calculate Leading Span B (Senkou Span B)
            for (size_t i = leading_period - 1; i < candles.size(); ++i)
            {
                double high_sum = 0.0;
                double low_sum = 0.0;
                for (size_t j = i - (leading_period - 1); j <= i; ++j)
                {
                    high_sum += candles[j].high;
                    low_sum += candles[j].low;
                }
                double avg_high = high_sum / leading_period;
                double avg_low = low_sum / leading_period;
                leading_span_b[i] = (avg_high + avg_low) / 2.0;
            }

            // Calculate Lagging Span (Chikou Span)
            for (size_t i = lagging_period; i < candles.size(); ++i)
            {
                lagging_span[i] = candles[i - lagging_period].close;
            }

            std::vector<double> trend(candles.size(), 0.0);

            for (size_t i = 0; i < candles.size(); ++i)
            {
                if (candles[i].close > leading_span_a[i] && leading_span_a[i] > leading_span_b[i])
                {
                    trend[i] = 1.0;
                }
                else if (candles[i].close < leading_span_a[i] && leading_span_a[i] < leading_span_b[i])
                {
                    trend[i] = -1.0;
                }
                else
                {
                    trend[i] = 0.0;
                }
            }

            return trend; },

        normalize_data);
}

// *********************************************************************************************

/**
 * @brief Construct a new Ichimoku Kijun Tenkan Trend object.
 *
 * @param conversion_period The period for the Conversion Line (Tenkan-sen).
 * @param base_period The period for the Base Line (Kijun-sen).
 * @param lagging_period The period for the Lagging Span (Chikou Span).
 * @param leading_period The period for the Leading Span (Senkou Span).
 * @param offset Offset value. Default is 0.
 */
IchimokuKijunTenkanTrend::IchimokuKijunTenkanTrend(int conversion_period, int base_period, int lagging_period, int leading_period, int offset)
    : Indicator("Ichimoku Kijun Tenkan Trend", "ichimoku-tenkan-kijun-trend-" + std::to_string(conversion_period) + "-" + std::to_string(base_period) + "-" + std::to_string(lagging_period) + "-" + std::to_string(leading_period) + "-" + std::to_string(offset), offset, {-1, 1}),
      conversion_period(conversion_period),
      base_period(base_period),
      lagging_period(lagging_period),
      leading_period(leading_period) {}

/**
 * @brief Calculate the Ichimoku Kijun Tenkan Trend values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated Ichimoku Cloud values.
 */
std::vector<double> IchimokuKijunTenkanTrend::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](const std::vector<Candle> &candles) -> std::vector<double>
        {
            std::vector<double> conversion_line(candles.size(), 0.0);
            std::vector<double> base_line(candles.size(), 0.0);

            // Calculate Conversion Line (Tenkan-sen)
            for (size_t i = conversion_period - 1; i < candles.size(); ++i)
            {
                double high_sum = 0.0;
                double low_sum = 0.0;
                for (size_t j = i - (conversion_period - 1); j <= i; ++j)
                {
                    high_sum += candles[j].high;
                    low_sum += candles[j].low;
                }
                double avg_high = high_sum / conversion_period;
                double avg_low = low_sum / conversion_period;
                conversion_line[i] = (avg_high + avg_low) / 2.0;
            }

            // Calculate Base Line (Kijun-sen)
            for (size_t i = base_period - 1; i < candles.size(); ++i)
            {
                double high_sum = 0.0;
                double low_sum = 0.0;
                for (size_t j = i - (base_period - 1); j <= i; ++j)
                {
                    high_sum += candles[j].high;
                    low_sum += candles[j].low;
                }
                double avg_high = high_sum / base_period;
                double avg_low = low_sum / base_period;
                base_line[i] = (avg_high + avg_low) / 2.0;
            }

            std::vector<double> trend(candles.size(), 0.0);

            for (size_t i = 0; i < candles.size(); ++i)
            {
                if (candles[i].close > conversion_line[i] && conversion_line[i] > base_line[i])
                {
                    trend[i] = 1.0;
                }
                else if (candles[i].close < conversion_line[i] && conversion_line[i] < base_line[i])
                {
                    trend[i] = -1.0;
                }
                else
                {
                    trend[i] = 0.0;
                }
            }

            return trend; },

        normalize_data);
}

// *********************************************************************************************

/**
 * @brief Construct a new Simple Moving Average Slope object.
 *
 * @param period The period for the moving average.
 * @param source The source of data (e.g., close, high, low).
 * @param offset Offset value. Default is 0.
 */
SMASlope::SMASlope(int period, std::string source, int offset)
    : Indicator("Simple Moving Average Slope", "sma-slope-" + std::to_string(period) + "-" + source + "-" + std::to_string(offset), offset),
      period(period),
      source(source) {}

/**
 * @brief Calculate the slope of the simple moving average.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated slope values.
 */
std::vector<double> SMASlope::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](const std::vector<Candle> &candles) -> std::vector<double>
        {
            // Calculate the moving average
            std::vector<double> sma_values = calculate_simple_moving_average(get_candles_with_source(candles, source), period);

            // Calculate the slope of the moving average
            std::vector<double> slope_values(sma_values.size(), 0.0);

            for (size_t i = period; i < sma_values.size(); ++i)
            {
                double slope = (sma_values[i] - sma_values[i - period]) / period;
                slope_values[i] = slope;
            }

            return slope_values; },

        normalize_data);
}

// *********************************************************************************************

/**
 * @brief Construct a new Exponential Moving Average Slope object.
 *
 * @param period The period for the exponential moving average.
 * @param source The source of data (e.g., close, high, low).
 * @param offset Offset value. Default is 0.
 */
EMASlope::EMASlope(int period, std::string source, int offset)
    : Indicator("Exponential Moving Average Slope", "ema-slope-" + std::to_string(period) + "-" + source + "-" + std::to_string(offset), offset),
      period(period),
      source(source) {}

/**
 * @brief Calculate the slope of the exponential moving average.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated slope values.
 */
std::vector<double> EMASlope::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](const std::vector<Candle> &candles) -> std::vector<double>
        {
            // Calculate the moving average
            std::vector<double> sma_values = calculate_exponential_moving_average(get_candles_with_source(candles, source), period);

            // Calculate the slope of the moving average
            std::vector<double> slope_values(sma_values.size(), 0.0);

            for (size_t i = period; i < sma_values.size(); ++i)
            {
                double slope = (sma_values[i] - sma_values[i - period]) / period;
                slope_values[i] = slope;
            }

            return slope_values; },

        normalize_data);
}

// *********************************************************************************************

/**
 * @brief Construct a new Zigzag object.
 *
 * @param deviation The deviation value for the Zigzag calculation.
 * @param offset Offset value. Default is 0.
 */
Zigzag::Zigzag(double deviation, int offset) : Indicator("Zigzag", "zigzag-" + std::to_string(deviation) + "-" + std::to_string(offset), offset), deviation(deviation) {}

/**
 * @brief Calculate the Zigzag values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated Zigzag values.
 */
std::vector<double> Zigzag::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](const std::vector<Candle> &candles) -> std::vector<double>
        {
            std::vector<double> zigzag_values(candles.size(), 0.0);

            bool uptrend = true;                 // Flag to track the current trend (true for uptrend, false for downtrend)
            double last_peak = candles[0].high;  // Variable to store the last peak value
            double last_valley = candles[0].low; // Variable to store the last valley value

            // Loop through the candles
            for (size_t i = 0; i < candles.size(); ++i)
            {
                double current_high = candles[i].high;
                double current_low = candles[i].low;

                // Calculate percentage change from last peak or valley
                double high_change = (current_high - last_peak) / last_peak * 100;
                double low_change = (current_low - last_valley) / last_valley * 100;

                // Determine if the change exceeds the box size
                bool high_change_exceeds = (high_change >= this->deviation);
                bool low_change_exceeds = (low_change <= -this->deviation);

                // If in uptrend and current high exceeds last peak by box size
                if (uptrend && high_change_exceeds)
                {
                    // Set zigzag value to the last peak
                    zigzag_values[i] = last_peak;

                    // Change to downtrend
                    uptrend = false;

                    // Update last valley
                    last_valley = current_low;
                }
                // If in downtrend and current low exceeds last valley by -box size
                else if (!uptrend && low_change_exceeds)
                {
                    // Set zigzag value to the last valley
                    zigzag_values[i] = last_valley;

                    // Change to uptrend
                    uptrend = true;

                    // Update last peak
                    last_peak = current_high;
                }
                // Otherwise, no significant change, set zigzag value to current high/low
                else
                {
                    zigzag_values[i] = uptrend ? current_high : current_low;
                }
            }

            return zigzag_values; },

        normalize_data);
}

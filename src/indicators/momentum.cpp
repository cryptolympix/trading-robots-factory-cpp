#include <vector>
#include "../types.hpp"
#include "../utils/math.hpp"
#include "../utils/candles_source.hpp"
#include "../utils/vectors.hpp"
#include "utils.hpp"
#include "candle.hpp"
#include "momentum.hpp"

/**
 * @brief Construct a new AwesomeOscillator object.
 *
 * @param offset Offset value. Default is 0.
 */
AwesomeOscillator::AwesomeOscillator(int offset) : Indicator("Awesome Oscillator", "awesome-indicator-" + std::to_string(offset), offset) {}

/**
 * @brief Calculate the CandleOpen values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> AwesomeOscillator::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles)
        {
            std::vector<double> median_prices = get_candles_with_source(candles, CandleSource::HL2);
            std::vector<double> average_5 = calculate_exponential_moving_average(median_prices, 5);
            std::vector<double> average_34 = calculate_exponential_moving_average(median_prices, 34);
            std::vector<double> result = subtract_vectors(average_5, average_34);
            return result; },
        normalize_data);
}

// *********************************************************************************************

/**
 * @brief Construct a new KAMA object.
 *
 * @param er_period Period for the Efficiency Ratio (ER). Default is 10.
 * @param fastest_sc_period Period for the fastest Smoothing Constant (SC). Default is 2.
 * @param slowest_sc_period Period for the slowest Smoothing Constant (SC). Default is 30.
 * @param offset Offset value. Default is 0.
 */
KAMA::KAMA(int er_period, int fastest_sc_period, int slowest_sc_period, int offset) : Indicator("Kaufman's Adaptive Moving Average", "kama-" + std::to_string(er_period) + "-" + std::to_string(fastest_sc_period) + "-" + std::to_string(slowest_sc_period) + "-" + std::to_string(offset), offset),
                                                                                      er_period(er_period), fastest_sc_period(fastest_sc_period), slowest_sc_period(slowest_sc_period) {}

/**
 * @brief Calculate Kaufman's Adaptive Moving Average (KAMA).
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing the calculated KAMA values.
 */
std::vector<double> KAMA::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles)
        {
            std::vector<double> kama_values(candles.size(), 0.0);

            if (candles.size() < static_cast<size_t>(slowest_sc_period))
            {
                return kama_values; // Return an empty vector if there are not enough candles
            }

            std::vector<double> closes = get_candles_with_source(candles, CandleSource::Close); // Extract close prices from candles
            std::vector<double> er_values = calculate_er(closes);                               // Calculate Efficiency Ratio (ER)
            std::vector<double> sc_values = calculate_sc(er_values);                            // Calculate Smoothing Constant (SC)

            double prior_kama = calculate_initial_kama(closes); // Calculate the initial KAMA

            // Calculate KAMA for subsequent periods
            for (size_t i = slowest_sc_period; i < closes.size(); ++i)
            {
                double current_kama = prior_kama + sc_values[i] * (closes[i] - prior_kama); // Calculate KAMA
                kama_values[i] = current_kama;                                              // Store the KAMA
                prior_kama = current_kama;                                                  // Update prior KAMA for the next iteration
            }

            return kama_values; },

        normalize_data);
}

/**
 * @brief Calculate the Efficiency Ratio (ER).
 *
 * @param closes Vector of closing prices.
 * @return std::vector<double> Vector containing the calculated ER values.
 */
std::vector<double> KAMA::calculate_er(const std::vector<double> &closes) const
{
    std::vector<double> er_values(closes.size(), 0.0);

    for (size_t i = er_period; i < closes.size(); ++i)
    {
        double change = std::abs(closes[i] - closes[i - er_period]);
        double volatility = 0.0;
        size_t start_index = i - er_period + 1;

        for (size_t j = start_index; j <= i; ++j)
        {
            volatility += std::abs(closes[j] - closes[j - 1]);
        }
        er_values[i] = volatility != 0.0 ? change / volatility : 0.0;
    }

    return er_values;
}

/**
 * @brief Calculate the Smoothing Constant (SC).
 *
 * @param er_values Vector of Efficiency Ratio (ER) values.
 * @return std::vector<double> Vector containing the calculated SC values.
 */
std::vector<double> KAMA::calculate_sc(const std::vector<double> &er_values) const
{
    std::vector<double> sc_values(er_values.size(), 0.0);

    double fastest_sc = 2.0 / (fastest_sc_period + 1);
    double slowest_sc = 2.0 / (slowest_sc_period + 1);

    for (size_t i = slowest_sc_period; i < er_values.size(); ++i)
    {
        sc_values[i] = std::pow(er_values[i] * (fastest_sc - slowest_sc) + slowest_sc, 2);
    }

    return sc_values;
}

/**
 * @brief Calculate the initial KAMA (Simple Moving Average).
 *
 * @param closes Vector of closing prices.
 * @return double Initial KAMA value.
 */
double KAMA::calculate_initial_kama(const std::vector<double> &closes) const
{
    double sum = 0.0;
    for (int i = 0; i < slowest_sc_period; ++i)
    {
        sum += closes[i];
    }
    return sum / slowest_sc_period;
}

// *********************************************************************************************

/**
 * @brief Construct a new PPO object with a specified short period, long period, and offset.
 *
 * @param short_period The short period for calculating PPO (default is 12).
 * @param long_period The long period for calculating PPO (default is 26).
 * @param offset The offset value (default is 0).
 */
PPO::PPO(int short_period, int long_period, int offset) : Indicator("Percentage Price Oscillator", "ppo-" + std::to_string(short_period) + "-" + std::to_string(long_period) + "-" + std::to_string(offset), offset), short_period(short_period), long_period(long_period) {}

/**
 * @brief Calculate the Price Oscillator (PPO) for a given set of candles.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated PPO values.
 */
std::vector<double> PPO::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](const std::vector<Candle> &candles) -> std::vector<double>
        {
            std::vector<double> closes = get_candles_with_source(candles, CandleSource::Close);
            std::vector<double> short_ema = calculate_exponential_moving_average(closes, short_period);
            std::vector<double> long_ema = calculate_exponential_moving_average(closes, long_period);

            std::vector<double> ppo_values(candles.size(), 0.0); // Initialize result vector with the same size as input

            std::vector<double> ema_diff = subtract_vectors(short_ema, long_ema);

            for (size_t i = 0; i < candles.size(); ++i)
            {
                double ppo = (ema_diff[i] / long_ema[i]) * 100.0;
                ppo_values[i] = ppo; // Assign values to the appropriate index in the result vector
            }

            return ppo_values; },
        normalize_data);
}

// *********************************************************************************************

/**
 * @brief Construct a new Percentage Volume Oscillator (PVO) object.
 *
 * @param fast_period The fast period used for calculating the PVO.
 * @param slow_period The slow period used for calculating the PVO.
 * @param signal_period The signal period used for calculating the PVO signal line.
 * @param offset Offset value. Default is 0.
 */
PVO::PVO(int fast_period, int slow_period, int signal_period, int offset)
    : Indicator("Percentage Volume Oscillator", "pvo-" + std::to_string(fast_period) + "-" + std::to_string(slow_period) + "-" + std::to_string(signal_period) + "-" + std::to_string(offset), offset),
      fast_period(fast_period), slow_period(slow_period), signal_period(signal_period)
{
}

/**
 * @brief Calculate the Percentage Volume Oscillator (PVO) values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated PVO values.
 */
std::vector<double> PVO::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles) -> std::vector<double>
        {
            std::vector<double> volume_values = get_candles_with_source(candles, CandleSource::Volume);
            std::vector<double> pvo_values;
            pvo_values.reserve(candles.size());

            if (volume_values.size() < static_cast<size_t>(slow_period)) {
                return pvo_values;
            }

            // Calculate fast EMA of volume
            std::vector<double> fast_ema_volume = calculate_exponential_moving_average(volume_values, fast_period);

            // Calculate slow EMA of volume
            std::vector<double> slow_ema_volume = calculate_exponential_moving_average(volume_values, slow_period);

            // Difference between fast and slow EMA of volume
            std::vector<double> ema_diff = subtract_vectors(fast_ema_volume, slow_ema_volume);

            // Divide the difference by the slow EMA of volume
            std::vector<double> ema_div = divide_vectors(ema_diff, slow_ema_volume);

            // Multiply by 100 to get the percentage
            for (size_t i = 0; i < slow_ema_volume.size(); ++i)
            {
                double pvo_value = ema_div[i] * 100.0;
                pvo_values.push_back(pvo_value);
            }

            // Apply offset if necessary
            if (offset > 0)
            {
                pvo_values.insert(pvo_values.begin(), offset, std::numeric_limits<double>::quiet_NaN());
            }

            return pvo_values; },

        normalize_data);
}

// *********************************************************************************************

/**
 * @brief Construct a new ROC object.
 *
 * @param period Period value.
 * @param offset Offset value. Default is 0.
 */
ROC::ROC(int period, int offset) : Indicator("Rate of Change", "roc-" + std::to_string(offset), offset), period(period) {}

/**
 * @brief Calculate the Rate of Change (ROC) values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> ROC::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles)
        {
            std::vector<double> result;
            result.reserve(candles.size());

            std::vector<double> closes = get_candles_with_source(candles, CandleSource::Close);
            size_t n = closes.size();
            if (n <= 1) return result; // Not enough data

            for (size_t i = 0; i < period; ++i)
            {
                result.push_back(std::numeric_limits<double>::quiet_NaN());
            }

            for (size_t i = period; i < n; ++i) {
                double roc = (closes[i] / closes[i - period]) * 100.0;
                result.push_back(roc);
            }
            return result; },
        normalize_data);
}

// *********************************************************************************************

/**
 * @brief Construct a new RelativeStrengthIndex object.
 *
 * @param period Period value. Default is 14.
 * @param offset Offset value. Default is 0.
 */
RSI::RSI(int period, int offset) : Indicator("Relative Strength Index (RSI)", "rsi-" + std::to_string(period) + "-" + std::to_string(offset), offset), period(period) {}

/**
 * @brief Calculate the Relative Strength Index (RSI) values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> RSI::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](const std::vector<Candle> &candles) -> std::vector<double>
        {
            std::vector<double> closes = get_candles_with_source(candles, CandleSource::Close);
            std::vector<double> result(closes.size(), 0.0); // Initialize result vector with the same size as input
            size_t n = closes.size();

            if (n <= static_cast<size_t>(period)) return result; // Not enough data

            double gain_sum = 0.0;
            double loss_sum = 0.0;

            // Calculate initial gains and losses
            for (int i = 1; i <= period; ++i) {
                double diff = closes[i] - closes[i - 1];
                if (diff > 0) {
                    gain_sum += diff;
                } else {
                    loss_sum += std::abs(diff);
                }
            }

            double avg_gain = gain_sum / period;
            double avg_loss = loss_sum / period;
            double rs = (avg_loss != 0) ? (avg_gain / avg_loss) : 0;
            double rsi = 100.0 - (100.0 / (1.0 + rs));
            result[period] = rsi; // Store the initial RSI value

            // Calculate RSI for subsequent periods
            for (size_t i = period + 1; i < n; ++i) {
                double diff = closes[i] - closes[i - 1];
                double gain = (diff > 0) ? diff : 0;
                double loss = (diff < 0) ? std::abs(diff) : 0;
                avg_gain = (avg_gain * (period - 1) + gain) / period;
                avg_loss = (avg_loss * (period - 1) + loss) / period;
                rs = (avg_loss != 0) ? (avg_gain / avg_loss) : 0;
                rsi = 100.0 - (100.0 / (1.0 + rs));
                result[i] = rsi; // Store the RSI value
            }

            return result; },
        normalize_data);
}

// *********************************************************************************************

/**
 * @brief Construct a new StochasticRSI object.
 *
 * @param period Period value. Default is 14.
 * @param sma_period SMA Period value. Default is 3.
 * @param offset Offset value. Default is 0.
 */
StochasticRSI::StochasticRSI(int period, int sma_period, int offset)
    : Indicator("Stochastic Relative Strength Index (Stoch RSI)", "stoch-rsi-" + std::to_string(period) + "-" + std::to_string(sma_period) + "-" + std::to_string(offset), offset),
      period(period), sma_period(sma_period) {}

/**
 * @brief Calculate the Stochastic Relative Strength Index (Stoch RSI) values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> StochasticRSI::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](const std::vector<Candle> &candles) -> std::vector<double>
        {
            // Calculate RSI values using the RelativeStrengthIndex class
            RSI rsi(period);
            std::vector<double> rsi_values = rsi.calculate(candles, false);

            // Calculate Stochastic RSI values
            std::vector<double> result(candles.size(), 0.0); // Initialize result vector with the same size as input
            size_t n = rsi_values.size();
            if (n <= static_cast<size_t>(sma_period))
                return result; // Not enough data

            for (size_t i = sma_period; i < n; ++i)
            {
                double max_rsi = *std::max_element(rsi_values.begin() + (i - sma_period), rsi_values.begin() + i + 1);
                double min_rsi = *std::min_element(rsi_values.begin() + (i - sma_period), rsi_values.begin() + i + 1);
                double stoch_rsi = (rsi_values[i] - min_rsi) / (max_rsi - min_rsi) * 100.0;
                result[i] = stoch_rsi; // Store the Stochastic RSI value
            }

            return result; },
        normalize_data);
}

// *********************************************************************************************

/**
 * @brief Construct a new StochasticOscillator object.
 *
 * @param k_period K Period value. Default is 14.
 * @param d_period D Period value. Default is 3.
 * @param offset Offset value. Default is 0.
 */
StochasticOscillator::StochasticOscillator(int k_period, int d_period, int offset)
    : Indicator("Stochastic Oscillator", "stoch-oscillator-" + std::to_string(k_period) + "-" + std::to_string(d_period) + "-" + std::to_string(offset), offset),
      k_period(k_period),
      d_period(d_period) {}

/**
 * @brief Calculate the Stochastic Oscillator values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> StochasticOscillator::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](const std::vector<Candle> &candles) -> std::vector<double>
        {
            // Calculate RSI values using the RelativeStrengthIndex class
            RSI rsi(k_period);
            std::vector<double> rsi_values = rsi.calculate(candles, false);

            // Calculate Stochastic Oscillator values
            std::vector<double> result(candles.size(), 0.0); // Initialize result vector with the same size as input
            size_t n = rsi_values.size();

            if (n <= static_cast<size_t>(d_period)) {
                return result; // Not enough data
            }

            std::vector<double> all_highs = get_candles_with_source(candles, CandleSource::High);
            std::vector<double> all_lows = get_candles_with_source(candles, CandleSource::Low);

            for (size_t i = d_period - 1; i < n; ++i)
            {
                std::vector<double> highs = std::vector<double>(all_highs.begin() + (i - d_period + 1), all_highs.begin() + i + 1);
                std::vector<double> lows = std::vector<double>(all_lows.begin() + (i - d_period + 1), all_lows.begin() + i + 1);

                // Calculate %K value
                double highest_high = *std::max_element(highs.begin(), highs.end());
                double lowest_low = *std::min_element(lows.begin(), lows.end());
                double stochastic_k = 100.0 * (candles[i].close - lowest_low) / (highest_high - lowest_low);

                // Add %K value to result
                result[i] = stochastic_k;
            }

            return result; },
        normalize_data);
}

// *********************************************************************************************

/**
 * @brief Construct a new TrueStrengthIndex object.
 *
 * @param r_period R Period value. Default is 25.
 * @param s_period S Period value. Default is 13.
 * @param smooth_period Smoothing Period value. Default is 7.
 * @param offset Offset value. Default is 0.
 */
TSI::TSI(int r_period, int s_period, int smooth_period, int offset)
    : Indicator("True Strength Index", "tsi-" + std::to_string(r_period) + "-" + std::to_string(s_period) + "-" + std::to_string(smooth_period) + "-" + std::to_string(offset), offset),
      r_period(r_period),
      s_period(s_period),
      smooth_period(smooth_period) {}

/**
 * @brief Calculate the True Strength Index values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> TSI::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](const std::vector<Candle> &candles) -> std::vector<double>
        {
            // Extract close prices from candles
            std::vector<double> closes = get_candles_with_source(candles, CandleSource::Close);

            // Calculate first-order momentum using the ExponentialMovingAverage class
            std::vector<double> momentum_1 = calculate_exponential_moving_average(closes, r_period);

            // Calculate second-order momentum using the ExponentialMovingAverage class on first-order momentum
            std::vector<double> momentum_2 = calculate_exponential_moving_average(momentum_1, s_period);

            // Calculate the difference between the first and second-order momentum
            std::vector<double> tsi_values = subtract_vectors(momentum_1, momentum_2);

            // Smooth the TSI values using the ExponentialMovingAverage class
            std::vector<double> smoothed_tsi_values = calculate_exponential_moving_average(tsi_values, smooth_period);

            return smoothed_tsi_values; },

        normalize_data);
}

// *********************************************************************************************

/**
 * @brief Construct a new Ultimate Oscillator object.
 *
 * @param offset Offset value. Default is 0.
 */
UO::UO(int offset) : Indicator("Ultimate Oscillator", "uo-" + std::to_string(offset), offset) {}

/**
 * @brief Calculate the Ultimate Oscillator values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> UO::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](const std::vector<Candle> &candles) -> std::vector<double>
        {
            const size_t period1 = 7;
            const size_t period2 = 14;
            const size_t period3 = 28;

            std::vector<double> true_range = get_true_range(candles);
            std::vector<double> avg_true_range1 = calculate_exponential_moving_average(true_range, period1);
            std::vector<double> avg_true_range2 = calculate_exponential_moving_average(true_range, period2);
            std::vector<double> avg_true_range3 = calculate_exponential_moving_average(true_range, period3);

            std::vector<double> buying_pressure = get_buying_pressure(candles);
            std::vector<double> avg_buying_pressure1 = calculate_exponential_moving_average(buying_pressure, period1);
            std::vector<double> avg_buying_pressure2 = calculate_exponential_moving_average(buying_pressure, period2);
            std::vector<double> avg_buying_pressure3 = calculate_exponential_moving_average(buying_pressure, period3);

            std::vector<double> result(candles.size(), 0.0); // Initialize result vector with the same size as input

            for (size_t i = period3; i < candles.size(); ++i) {
                double raw_value = 100.0 * (4 * avg_buying_pressure1[i] + 2 * avg_buying_pressure2[i] + avg_buying_pressure3[i]) /
                                           (4 * avg_true_range1[i] + 2 * avg_true_range2[i] + avg_true_range3[i]);
                result[i] = raw_value; // Store the raw value
            }

            return result; },
        normalize_data);
}

std::vector<double> UO::get_true_range(const std::vector<Candle> &candles) const
{
    std::vector<double> true_range;
    true_range.reserve(candles.size());

    for (size_t i = 1; i < candles.size(); ++i)
    {
        double high_low = candles[i].high - candles[i].low;
        double high_close = std::abs(candles[i].high - candles[i - 1].close);
        double low_close = std::abs(candles[i].low - candles[i - 1].close);
        double value = std::max({high_low, high_close, low_close});
        true_range.push_back(value);
    }

    return true_range;
}

std::vector<double> UO::get_buying_pressure(const std::vector<Candle> &candles) const
{
    std::vector<double> buying_pressure;
    buying_pressure.reserve(candles.size());

    for (const auto &candle : candles)
    {
        double value = candle.close - std::min(candle.low, candle.open);
        buying_pressure.push_back(value);
    }

    return buying_pressure;
}

// *********************************************************************************************

/**
 * @brief Construct a new Williams Percent R object.
 *
 * @param period The period for calculating Williams %R.
 * @param offset Offset value. Default is 0.
 */
WilliamsPercentR::WilliamsPercentR(int period, int offset) : Indicator("Williams %R", "williams-percent-r-" + std::to_string(period) + "-" + std::to_string(offset), offset), period(period) {}

/**
 * @brief Calculate the Williams Percent R values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> WilliamsPercentR::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](const std::vector<Candle> &candles) -> std::vector<double>
        {
            std::vector<double> result(candles.size(), 0.0); // Initialize result vector with the same size as input
            Highest highest_source(CandleSource::High, period, 0);
            Lowest lowest_source(CandleSource::Low, period, 0);

            std::vector<double> highest_highs = highest_source.calculate(candles, false);
            std::vector<double> lowest_lows = lowest_source.calculate(candles, false);

            for (size_t i = period - 1; i < candles.size(); ++i)
            {
                double highest_high = highest_highs[i];
                double lowest_low = lowest_lows[i];
                double current_close = candles[i].close;
                double value = (highest_high != lowest_low) ? ((highest_high - current_close) / (highest_high - lowest_low)) * -100.0 : 0.0;
                result[i] = value; // Store the Williams %R value
            }

            return result; },

        normalize_data);
}

#include <vector>
#include "../utils/vectors.hpp"
#include "trend.hpp"
#include "trend_signals.hpp"
#include "indicator.hpp"
#include "utils.hpp"

/**
 * @brief Construct a new ADXSignal object.
 *
 * @param period ADX period value. Default is 14.
 * @param threshold Threshold value. Default is 25.
 * @param offset Offset value. Default is 0.
 */
ADXSignal::ADXSignal(int period, int threshold, int offset) : Indicator(this->id + "-" + std::to_string(period) + "-" + std::to_string(threshold) + "-" + std::to_string(offset), offset, {0, 1}), period(period), threshold(threshold) {}

/**
 * @brief Calculate the ADXSignal values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> ADXSignal::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles)
        {
            std::vector<double> result(candles.size(), 0.0);
            std::vector<double> adx_values = ADX(period, offset).calculate(candles, false);

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
 * @brief Construct a new AroonSignal object.
 *
 * @param aroon_period Aroon period value. Default is 14.
 * @param offset Offset value. Default is 0.
 */
AroonSignal::AroonSignal(int period, int offset) : Indicator(this->id + "-" + std::to_string(period) + "-" + std::to_string(offset), offset, {-1, 1}), period(period) {}

/**
 * @brief Calculate the AroonSignal values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> AroonSignal::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles)
        {
            std::vector<double> result(candles.size(), 0.0);
            std::vector<double> aroon_up_values = AroonUp(period, offset).calculate(candles, false);
            std::vector<double> aroon_down_values = AroonDown(period, offset).calculate(candles, false);

            for (size_t i = 1; i < aroon_up_values.size(); i++)
            {
                if (aroon_up_values[i - 1] < aroon_down_values[i - 1] && aroon_up_values[i] > aroon_down_values[i])
                {
                    result[i] = 1.0;
                }
                else if (aroon_up_values[i - 1] > aroon_down_values[i - 1] && aroon_up_values[i] < aroon_down_values[i])
                {
                    result[i] = -1.0;
                }
            }

            return result; },
        normalize_data);
}

// *********************************************************************************************

/**
 * @brief Construct a new CCISignal object.
 *
 * @param period CCI period value. Default is 20.
 * @param overbought Overbought value. Default is 100.
 * @param oversold Oversold value. Default is -100.
 * @param offset Offset value. Default is 0.
 */
CCISignal::CCISignal(int period, int overbought, int oversold, int offset) : Indicator(this->id + "-" + std::to_string(period) + "-" + std::to_string(overbought) + "-" + std::to_string(oversold) + "-" + std::to_string(offset), offset, {-1, 1}), period(period), overbought(overbought), oversold(oversold) {}

/**
 * @brief Calculate the CCISignal values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> CCISignal::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles)
        {
            std::vector<double> result(candles.size(), 0.0);
            std::vector<double> cci_values = CCI(period, offset).calculate(candles, false);

            for (size_t i = 1; i < cci_values.size(); i++)
            {
                if (cci_values[i - 1] < oversold && cci_values[i] > oversold)
                {
                    result[i] = 1.0;
                }
                else if (cci_values[i - 1] > overbought && cci_values[i] < overbought)
                {
                    result[i] = -1.0;
                }
            }

            return result; },
        normalize_data);
}

// *********************************************************************************************

/**
 * @brief Construct a new DPOSignal object.
 *
 * @param period DPO period value. Default is 20.
 * @param offset Offset value. Default is 0.
 */
DPOSignal::DPOSignal(int period, int offset) : Indicator(this->id + "-" + std::to_string(period) + "-" + std::to_string(offset), offset, {-1, 1}), period(period) {}

/**
 * @brief Calculate the DPOSignal values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> DPOSignal::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles)
        {
            std::vector<double> result(candles.size(), 0.0);
            std::vector<double> dpo_values = DPO(period, offset).calculate(candles, false);

            for (size_t i = 1; i < dpo_values.size(); i++)
            {
                if (dpo_values[i - 1] < 0 && dpo_values[i] > 0)
                {
                    result[i] = 1.0;
                }
                else if (dpo_values[i - 1] > 0 && dpo_values[i] < 0)
                {
                    result[i] = -1.0;
                }
            }

            return result; },
        normalize_data);
}

// *********************************************************************************************

/**
 * @brief Construct a new EMASignal object.
 *
 * @param source The source of the data. Default is "close".
 * @param period EMA period value. Default is 20.
 * @param offset Offset value. Default is 0.
 */
EMASignal::EMASignal(std::string source, int period, int offset) : Indicator(this->id + "-" + source + "-" + std::to_string(period) + "-" + std::to_string(offset), offset, {-1, 1}), source(source), period(period) {}

/**
 * @brief Calculate the EMASignal values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> EMASignal::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles)
        {
            std::vector<double> result(candles.size(), 0.0);
            std::vector<double> ema_values = EMA(source, period, offset).calculate(candles, false);

            for (size_t i = 1; i < ema_values.size(); i++)
            {
                if (candles[i - 1].close < ema_values[i - 1] && candles[i].close > ema_values[i])
                {
                    result[i] = 1.0;
                }
                else if (candles[i - 1].close > ema_values[i - 1] && candles[i].close < ema_values[i])
                {
                    result[i] = -1.0;
                }
            }

            return result; },
        normalize_data);
}

// *********************************************************************************************

/**
 * @brief Construct a new KSTSignal object.
 *
 * @param roc_period1 Rate of Change period 1 value. Default is 10.
 * @param roc_period2 Rate of Change period 2 value. Default is 15.
 * @param roc_period3 Rate of Change period 3 value. Default is 20.
 * @param roc_period4 Rate of Change period 4 value. Default is 30.
 * @param sma_period1 Simple Moving Average period 1 value. Default is 10.
 * @param sma_period2 Simple Moving Average period 2 value. Default is 10.
 * @param sma_period3 Simple Moving Average period 3 value. Default is 10.
 * @param sma_period4 Simple Moving Average period 4 value. Default is 15.
 * @param offset Offset value. Default is 0.
 */
KSTSignal::KSTSignal(int roc_period1, int roc_period2, int roc_period3, int roc_period4, int sma_period1, int sma_period2, int sma_period3, int sma_period4, int offset) : Indicator(this->id + "-" + std::to_string(roc_period1) + "-" + std::to_string(roc_period2) + "-" + std::to_string(roc_period3) + "-" + std::to_string(roc_period4) + "-" + std::to_string(sma_period1) + "-" + std::to_string(sma_period2) + "-" + std::to_string(sma_period3) + "-" + std::to_string(sma_period4) + "-" + std::to_string(offset), offset, {-1, 1}), roc_period1(roc_period1), roc_period2(roc_period2), roc_period3(roc_period3), roc_period4(roc_period4), sma_period1(sma_period1), sma_period2(sma_period2), sma_period3(sma_period3), sma_period4(sma_period4) {}

/**
 * @brief Calculate the KSTSignal values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> KSTSignal::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles)
        {
            std::vector<double> result(candles.size(), 0.0);
            std::vector<double> kst_values = KST(roc_period1, roc_period2, roc_period3, roc_period4, sma_period1, sma_period2, sma_period3, sma_period4, offset).calculate(candles, false);
            std::vector<double> kst_signal_line = calculate_simple_moving_average(kst_values, 9);

            for (size_t i = 1; i < kst_values.size(); i++)
            {
                if (kst_values[i - 1] < kst_signal_line[i - 1] && kst_values[i] > kst_signal_line[i])
                {
                    result[i] = 1.0;
                }
                else if (kst_values[i - 1] > kst_signal_line[i - 1] && kst_values[i] < kst_signal_line[i])
                {
                    result[i] = -1.0;
                }
            }

            return result; },
        normalize_data);
}

// *********************************************************************************************

/**
 * @brief Construct a new MACDSignal object.
 *
 * @param short_period Short EMA period value. Default is 12.
 * @param long_period Long EMA period value. Default is 26.
 * @param signal_period Signal EMA period value. Default is 9.
 * @param offset Offset value. Default is 0.
 */
MACDSignal::MACDSignal(int short_period, int long_period, int signal_period, int offset) : Indicator(this->id + "-" + std::to_string(short_period) + "-" + std::to_string(long_period) + "-" + std::to_string(signal_period) + "-" + std::to_string(offset), offset, {-1, 1}), short_period(short_period), long_period(long_period), signal_period(signal_period) {}

/**
 * @brief Calculate the MACDSignal values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> MACDSignal::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles)
        {
            std::vector<double> result(candles.size(), 0.0);
            std::vector<double> closes = get_candles_with_source(candles, "close");

            // Calculate short and long EMA
            std::vector<double> short_ema = calculate_exponential_moving_average(closes, short_period);
            std::vector<double> long_ema = calculate_exponential_moving_average(closes, long_period);

            // Calculate MACD line (difference between short EMA and long EMA)
            std::vector<double> macd_line(candles.size(), 0.0);
            for (size_t i = 0; i < closes.size(); ++i)
            {
                macd_line[i] = short_ema[i] - long_ema[i];
            }

            // Calculate signal line (EMA of MACD line)
            std::vector<double> signal_line = calculate_exponential_moving_average(macd_line, signal_period);

            // If MACD line crosses above signal line, buy signal
            // if MACD line crosses below signal line, sell signal
            for (size_t i = 1; i < candles.size(); ++i) {
                if (macd_line[i - 1] < signal_line[i - 1] && macd_line[i] > signal_line[i])
                {
                    result[i] = 1.0;
                }
                else if (macd_line[i - 1] > signal_line[i - 1] && macd_line[i] < signal_line[i])
                {
                    result[i] = -1.0;
                }
            }

            return result; },
        normalize_data);
}

// *********************************************************************************************

/**
 * @brief Construct a new ParabolicSARSignal object.
 *
 * @param acceleration_factor_initial Initial acceleration factor value. Default is 0.02.
 * @param acceleration_factor_maximum Maximum acceleration factor value. Default is 0.2.
 * @param offset Offset value. Default is 0.
 */
ParabolicSARSignal::ParabolicSARSignal(double acceleration_factor_initial, double acceleration_factor_maximum, int offset) : Indicator(this->id + "-" + std::to_string(acceleration_factor_initial) + "-" + std::to_string(acceleration_factor_maximum) + "-" + std::to_string(offset), offset, {-1, 1}), acceleration_factor_initial(acceleration_factor_initial), acceleration_factor_maximum(acceleration_factor_maximum) {}

/**
 * @brief Calculate the ParabolicSARSignal values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> ParabolicSARSignal::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles)
        {
            std::vector<double> result(candles.size(), 0.0);
            std::vector<double> psar_values = ParabolicSAR(acceleration_factor_initial, acceleration_factor_maximum, offset).calculate(candles, false);

            for (size_t i = 1; i < psar_values.size(); i++)
            {
                if (candles[i - 1].close < psar_values[i - 1] && candles[i].close > psar_values[i])
                {
                    result[i] = 1.0;
                }
                else if (candles[i - 1].close > psar_values[i - 1] && candles[i].close < psar_values[i])
                {
                    result[i] = -1.0;
                }
            }

            return result; },
        normalize_data);
}

// *********************************************************************************************

/**
 * @brief Construct a new SMASignal object.
 *
 * @param source The source of the data. Default is "close".
 * @param period SMA period value. Default is 20.
 * @param offset Offset value. Default is 0.
 */
SMASignal::SMASignal(std::string source, int period, int offset) : Indicator(this->id + "-" + source + "-" + std::to_string(period) + "-" + std::to_string(offset), offset, {-1, 1}), source(source), period(period) {}

/**
 * @brief Calculate the SMASignal values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> SMASignal::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles)
        {
            std::vector<double> result(candles.size(), 0.0);
            std::vector<double> sma_values = SMA(source, period, offset).calculate(candles, false);

            for (size_t i = 1; i < sma_values.size(); i++)
            {
                if (candles[i - 1].close < sma_values[i - 1] && candles[i].close > sma_values[i])
                {
                    result[i] = 1.0;
                }
                else if (candles[i - 1].close > sma_values[i - 1] && candles[i].close < sma_values[i])
                {
                    result[i] = -1.0;
                }
            }

            return result; },
        normalize_data);
}

// *********************************************************************************************

/**
 * @brief Construct a new TRIXSignal object.
 *
 * @param period TRIX period value. Default is 20.
 * @param offset Offset value. Default is 0.
 */
TRIXSignal::TRIXSignal(int period, int offset) : Indicator(this->id + "-" + std::to_string(period) + "-" + std::to_string(offset), offset, {-1, 1}), period(period) {}

/**
 * @brief Calculate the TRIXSignal values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> TRIXSignal::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles)
        {
            std::vector<double> result(candles.size(), 0.0);
            std::vector<double> trix_values = TRIX(period, offset).calculate(candles, false);
            std::vector<double>trix_signal_line = calculate_simple_moving_average(trix_values, 9);

            for (size_t i = 1; i < trix_values.size(); i++)
            {
                if (trix_values[i - 1] < trix_signal_line[i - 1] && trix_values[i] > trix_signal_line[i])
                {
                    result[i] = 1.0;
                }
                else if (trix_values[i - 1] > trix_signal_line[i - 1] && trix_values[i] < trix_signal_line[i])
                {
                    result[i] = -1.0;
                }
            }

            return result; },
        normalize_data);
}

// *********************************************************************************************

/**
 * @brief Construct a new VortexSignal object.
 *
 * @param period Vortex period value. Default is 14.
 * @param offset Offset value. Default is 0.
 */
VortexSignal::VortexSignal(int period, int offset) : Indicator(this->id + "-" + std::to_string(period) + "-" + std::to_string(offset), offset, {-1, 1}), period(period) {}

/**
 * @brief Calculate the VortexSignal values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> VortexSignal::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles)
        {
            std::vector<double> result(candles.size(), 0.0);

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

            for (size_t i = 1; i < candles.size(); i++)
            {
                if (positive_vi_14[i - 1] < negative_vi_14[i - 1] && positive_vi_14[i] > negative_vi_14[i])
                {
                    result[i] = 1.0;
                }
                else if (positive_vi_14[i - 1] > negative_vi_14[i - 1] && positive_vi_14[i] < negative_vi_14[i])
                {
                    result[i] = -1.0;
                }
            }

            return result; },
        normalize_data);
}

// *********************************************************************************************

/**
 * @brief Construct a new InstitutionalBiasSignal object.
 *
 * @param short_period Short period value. Default is 9.
 * @param long_period Long period value. Default is 18.
 * @param offset Offset value. Default is 0.
 */
InstitutionalBiasSignal::InstitutionalBiasSignal(int short_period, int long_period, int offset) : Indicator(this->id + "-" + std::to_string(short_period) + "-" + std::to_string(long_period) + "-" + std::to_string(offset), offset, {-1, 1}), short_period(short_period), long_period(long_period) {}

/**
 * @brief Calculate the InstitutionalBiasSignal values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> InstitutionalBiasSignal::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles)
        {
            std::vector<double> result(candles.size(), 0.0);

            std::vector<double> short_ema_values = EMA("close", short_period, offset).calculate(candles, false);
            std::vector<double> long_ema_values = EMA("close", long_period, offset).calculate(candles, false);

            for (size_t i = 1; i < candles.size(); i++)
            {
                if (short_ema_values[i - 1] < long_ema_values[i - 1] && short_ema_values[i] > long_ema_values[i])
                {
                    result[i] = 1.0;
                }
                else if (short_ema_values[i - 1] > long_ema_values[i - 1] && short_ema_values[i] < long_ema_values[i])
                {
                    result[i] = -1.0;
                }
            }

            return result; },
        normalize_data);
}

// *********************************************************************************************

/**
 * @brief Construct a new IchimokuCloudSignal object.
 *
 * @param conversion_period Conversion line period value. Default is 9.
 * @param base_period Base line period value. Default is 26.
 * @param lagging_period Lagging span period value. Default is 26.
 * @param leading_period Leading span period value. Default is 52.
 * @param offset Offset value. Default is 0.
 */
IchimokuCloudSignal::IchimokuCloudSignal(int conversion_period, int base_period, int lagging_period, int leading_period, int offset) : Indicator(this->id + "-" + std::to_string(conversion_period) + "-" + std::to_string(base_period) + "-" + std::to_string(lagging_period) + "-" + std::to_string(leading_period) + "-" + std::to_string(offset), offset, {-1, 1}), conversion_period(conversion_period), base_period(base_period), lagging_period(lagging_period), leading_period(leading_period) {}

/**
 * @brief Calculate the IchimokuCloudSignal values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> IchimokuCloudSignal::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles)
        {
            std::vector<double> result(candles.size(), 0.0);

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

            for (size_t i = 1; i < candles.size(); ++i)
            {
                if (leading_span_a[i - 1] < leading_span_b[i - 1] && leading_span_a[i] > leading_span_b[i])
                {
                    result[i] = 1.0;
                }
                else if (leading_span_a[i - 1] > leading_span_b[i - 1] && leading_span_a[i] < leading_span_b[i])
                {
                    result[i] = -1.0;
                }
            }

            return result; },
        normalize_data);
}

// *********************************************************************************************

/**
 * @brief Construct a new IchimokuKijunTenkanSignal object.
 *
 * @param conversion_period Conversion line period value. Default is 9.
 * @param base_period Base line period value. Default is 26.
 * @param lagging_period Lagging span period value. Default is 26.
 * @param leading_period Leading span period value. Default is 52.
 * @param offset Offset value. Default is 0.
 */
IchimokuKijunTenkanSignal::IchimokuKijunTenkanSignal(int conversion_period, int base_period, int lagging_period, int leading_period, int offset) : Indicator(this->id + "-" + std::to_string(conversion_period) + "-" + std::to_string(base_period) + "-" + std::to_string(lagging_period) + "-" + std::to_string(leading_period) + "-" + std::to_string(offset), offset, {-1, 1}), conversion_period(conversion_period), base_period(base_period), lagging_period(lagging_period), leading_period(leading_period) {}

/**
 * @brief Calculate the IchimokuKijunTenkanSignal values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> IchimokuKijunTenkanSignal::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles)
        {
        std::vector<double> result(candles.size(), 0.0);

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

        for (size_t i = 1; i < candles.size(); ++i)
        {
            if (conversion_line[i - 1] < base_line[i - 1] && conversion_line[i] > base_line[i])
            {
                result[i] = 1.0;
            }
            else if (conversion_line[i - 1] > base_line[i - 1] && conversion_line[i] < base_line[i])
            {
                result[i] = -1.0;
            }
        }

        return result; },
        normalize_data);
}

// *********************************************************************************************

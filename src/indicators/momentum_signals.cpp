#include <vector>
#include "indicator.hpp"
#include "momentum.hpp"
#include "momentum_signals.hpp"
#include "utils.hpp"

/**
 * @brief Construct a new AwesomeOscillatorSignal object.
 *
 * @param offset Offset value. Default is 0.
 */
AwesomeOscillatorSignal::AwesomeOscillatorSignal(int offset) : Indicator("Awesome Oscillator (Signal)", "awesome-oscillator-signal", {{"offset", offset}}, {-1, 1}) {}

/**
 * @brief Calculate the AwesomeOscillatorSignal values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> AwesomeOscillatorSignal::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles)
        {
            int offset = std::get<int>(this->params.at("offset"));

            std::vector<double> result(candles.size(), 0.0);
            std::vector<double> momentum_values = AwesomeOscillator(offset).calculate(candles, false);

            for (size_t i = 1; i < momentum_values.size(); i++)
            {
                if (momentum_values[i] > momentum_values[i - 1])
                {
                    result[i] = 1.0;
                }
                else if (momentum_values[i] < momentum_values[i - 1])
                {
                    result[i] = -1.0;
                }
            }

            return result; },
        normalize_data);
}

// *********************************************************************************************

/**
 * @brief Construct a new KAMASignal object.
 *
 * @param er_period ER period value. Default is 10.
 * @param fastest_sc_period Fastest SC period value. Default is 2.
 * @param slowest_sc_period Slowest SC period value. Default is 30.
 * @param offset Offset value. Default is 0.
 */
KAMASignal::KAMASignal(int er_period, int fastest_sc_period, int slowest_sc_period, int offset) : Indicator("Kaufman's Adaptive Moving Average (Signal)", "kama-signal", {{"er_period", er_period}, {"fastest_sc_period", fastest_sc_period}, {"slowest_sc_period", slowest_sc_period}, {"offset", offset}}, {-1, 1}) {}

/**
 * @brief Calculate the KAMASignal values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> KAMASignal::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles)
        {
            int er_period = std::get<int>(this->params.at("er_period"));
            int fastest_sc_period = std::get<int>(this->params.at("fastest_sc_period"));
            int slowest_sc_period = std::get<int>(this->params.at("slowest_sc_period"));
            int offset = std::get<int>(this->params.at("offset"));

            std::vector<double> result(candles.size(), 0.0);
            std::vector<double> kama_values = KAMA(er_period, fastest_sc_period, slowest_sc_period, offset).calculate(candles, false);

            for (size_t i = 0; i < kama_values.size(); i++)
            {
                if (candles[i].close > kama_values[i])
                {
                    result[i] = 1.0;
                }
                else if (candles[i].close < kama_values[i])
                {
                    result[i] = -1.0;
                }
            }

            return result; },
        normalize_data);
}

// *********************************************************************************************

/**
 * @brief Construct a new MFISignal object.
 *
 * @param period Period value. Default is 14.
 * @param overbought Overbought value. Default is 80.
 * @param oversold Oversold value. Default is 20.
 * @param offset Offset value. Default is 0.
 */
MFISignal::MFISignal(int period, int overbought, int oversold, int offset) : Indicator("Money Flow Index (Signal)", "mfi-signal", {{"period", period}, {"overbought", overbought}, {"oversold", oversold}, {"offset", offset}}, {-1, 1}) {}

/**
 * @brief Calculate the MFISignal values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> MFISignal::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles)
        {
            int period = std::get<int>(this->params.at("period"));
            int overbought = std::get<int>(this->params.at("overbought"));
            int oversold = std::get<int>(this->params.at("oversold"));
            int offset = std::get<int>(this->params.at("offset"));

            std::vector<double> result(candles.size(), 0.0);
            std::vector<double> mfi_values = MFI(period, offset).calculate(candles, false);

            for (size_t i = 1; i < mfi_values.size(); i++)
            {
                if (mfi_values[i - 1] < oversold && mfi_values[i] > oversold)
                {
                    result[i] = 1.0;
                }
                else if (mfi_values[i - 1] > overbought && mfi_values[i] < overbought)
                {
                    result[i] = -1.0;
                }
            }

            return result; },
        normalize_data);
}

// *********************************************************************************************

/**
 * @brief Construct a new PPOSignal object.
 *
 * @param short_period Short period value. Default is 12.
 * @param long_period Long period value. Default is 26.
 * @param offset Offset value. Default is 0.
 */
PPOSignal::PPOSignal(int short_period, int long_period, int offset) : Indicator("Percentage Price Oscillator (Signal)", "ppo-signal", {{"short_period", short_period}, {"long_period", long_period}, {"offset", offset}}, {-1, 1}) {}

/**
 * @brief Calculate the PPOSignal values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> PPOSignal::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles)
        {
            int short_period = std::get<int>(this->params.at("short_period"));
            int long_period = std::get<int>(this->params.at("long_period"));
            int offset = std::get<int>(this->params.at("offset"));

            std::vector<double> result(candles.size(), 0.0);
            std::vector<double> ppo_values = PPO(short_period, long_period, offset).calculate(candles, false);

            for (size_t i = 1; i < ppo_values.size(); i++)
            {
                if (ppo_values[i - 1] < 0 && ppo_values[i] > 0)
                {
                    result[i] = 1.0;
                }
                else if (ppo_values[i - 1] > 0 && ppo_values[i] < 0)
                {
                    result[i] = -1.0;
                }
            }

            return result; },
        normalize_data);
}

// *********************************************************************************************

/**
 * @brief Construct a new ROCSignal object.
 *
 * @param period Period value. Default is 9.
 * @param offset Offset value. Default is 0.
 */
ROCSignal::ROCSignal(int period, int offset) : Indicator("Rate of Change (Signal)", "roc-signal", {{"period", period}, {"offset", offset}}, {-1, 1}) {}

/**
 * @brief Calculate the ROCSignal values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> ROCSignal::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles)
        {
            int period = std::get<int>(this->params.at("period"));
            int offset = std::get<int>(this->params.at("offset"));

            std::vector<double> result(candles.size(), 0.0);
            std::vector<double> roc_values = ROC(period, offset).calculate(candles, false);

            for (size_t i = 1; i < roc_values.size(); i++)
            {
                if (roc_values[i - 1] < 0 && roc_values[i] > 0)
                {
                    result[i] = 1.0;
                }
                else if (roc_values[i - 1] > 0 && roc_values[i] < 0)
                {
                    result[i] = -1.0;
                }
            }

            return result; },
        normalize_data);
}

// *********************************************************************************************

/**
 * @brief Construct a new RSISignal object.
 *
 * @param period Period value. Default is 14.
 * @param offset Offset value. Default is 0.
 */
RSISignal::RSISignal(int period, int overbought, int oversold, int offset) : Indicator("Relative Strength Index (Signal)", "rsi-signal", {{"period", period}, {"overbought", overbought}, {"oversold", oversold}, {"offset", offset}}, {-1, 1}) {}

/**
 * @brief Calculate the RSISignal values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> RSISignal::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles)
        {
            int period = std::get<int>(this->params.at("period"));
            int overbought = std::get<int>(this->params.at("overbought"));
            int oversold = std::get<int>(this->params.at("oversold"));
            int offset = std::get<int>(this->params.at("offset"));

            std::vector<double> result(candles.size(), 0.0);
            std::vector<double> rsi_values = RSI(period, offset).calculate(candles, false);

            for (size_t i = 1; i < rsi_values.size(); i++)
            {
                if (rsi_values[i - 1] < oversold && rsi_values[i] > oversold)
                {
                    result[i] = 1.0;
                }
                else if (rsi_values[i - 1] > overbought && rsi_values[i] < overbought)
                {
                    result[i] = -1.0;
                }
            }

            return result; },
        normalize_data);
}

// *********************************************************************************************

/**
 * @brief Construct a new StochasticRSISignal object.
 *
 * @param period Period value. Default is 14.
 * @param overbought Overbought value. Default is 80.
 * @param oversold Oversold value. Default is 20.
 * @param offset Offset value. Default is 0.
 */
StochasticRSISignal::StochasticRSISignal(int period, int overbought, int oversold, int offset) : Indicator("Stochastic Relative Strength Index (Signal)", "stochastic-rsi-signal", {{"period", period}, {"overbought", overbought}, {"oversold", oversold}, {"offset", offset}}, {-1, 1}) {}

/**
 * @brief Calculate the StochasticRSISignal values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> StochasticRSISignal::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles)
        {
            int period = std::get<int>(this->params.at("period"));
            int overbought = std::get<int>(this->params.at("overbought"));
            int oversold = std::get<int>(this->params.at("oversold"));
            int offset = std::get<int>(this->params.at("offset"));

            std::vector<double> result(candles.size(), 0.0);
            std::vector<double> stoch_values = StochasticRSI(period, offset).calculate(candles, false);

            for (size_t i = 1; i < stoch_values.size(); i++)
            {
                if (stoch_values[i - 1] < oversold && stoch_values[i] > oversold)
                {
                    result[i] = 1.0;
                }
                else if (stoch_values[i - 1] > overbought && stoch_values[i] < overbought)
                {
                    result[i] = -1.0;
                }
            }

            return result; },
        normalize_data);
}

// *********************************************************************************************

/**
 * @brief Construct a new StochasticOscillatorSignal object.
 *
 * @param k_period K period value. Default is 14.
 * @param d_period D period value. Default is 3.
 * @param overbought Overbought value. Default is 80.
 * @param oversold Oversold value. Default is 20.
 * @param offset Offset value. Default is 0.
 */
StochasticOscillatorSignal::StochasticOscillatorSignal(int k_period, int d_period, int overbought, int oversold, int offset) : Indicator("Stochastic Oscillator (Signal)", "stochastic-oscillator-signal", {{"k_period", k_period}, {"d_period", d_period}, {"overbought", overbought}, {"oversold", oversold}, {"offset", offset}}, {-1, 1}) {}

/**
 * @brief Calculate the StochasticOscillatorSignal values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> StochasticOscillatorSignal::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles)
        {
            int k_period = std::get<int>(this->params.at("k_period"));
            int d_period = std::get<int>(this->params.at("d_period"));
            int overbought = std::get<int>(this->params.at("overbought"));
            int oversold = std::get<int>(this->params.at("oversold"));
            int offset = std::get<int>(this->params.at("offset"));

            std::vector<double> result(candles.size(), 0.0);
            std::vector<double> stoch_values = StochasticOscillator(k_period, d_period, offset).calculate(candles, false);

            for (size_t i = 1; i < stoch_values.size(); i++)
            {
                if (stoch_values[i - 1] < oversold && stoch_values[i] > oversold)
                {
                    result[i] = 1.0;
                }
                else if (stoch_values[i - 1] > overbought && stoch_values[i] < overbought)
                {
                    result[i] = -1.0;
                }
            }

            return result; },
        normalize_data);
}

// *********************************************************************************************

/**
 * @brief Construct a new TSISignal object.
 *
 * @param short_period Short period value. Default is 13.
 * @param long_period Long period value. Default is 25.
 * @param overbought Overbought value. Default is 25.
 * @param oversold Oversold value. Default is -25.
 * @param offset Offset value. Default is 0.
 */
TSISignal::TSISignal(int short_period, int long_period, int overbought, int oversold, int offset) : Indicator("True Strength Index (Signal)", "tsi-signal", {{"short_period", short_period}, {"long_period", long_period}, {"overbought", overbought}, {"oversold", oversold}, {"offset", offset}}, {-1, 1}) {}

/**
 * @brief Calculate the TSISignal values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> TSISignal::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles)
        {
            int short_period = std::get<int>(this->params.at("short_period"));
            int long_period = std::get<int>(this->params.at("long_period"));
            int overbought = std::get<int>(this->params.at("overbought"));
            int oversold = std::get<int>(this->params.at("oversold"));
            int offset = std::get<int>(this->params.at("offset"));

            std::vector<double> result(candles.size(), 0.0);
            std::vector<double> tsi_values = TSI(short_period, long_period, offset).calculate(candles, false);

            for (size_t i = 1; i < tsi_values.size(); i++)
            {
                if (tsi_values[i - 1] < oversold && tsi_values[i] > oversold)
                {
                    result[i] = 1.0;
                }
                else if (tsi_values[i - 1] > overbought && tsi_values[i] < overbought)
                {
                    result[i] = -1.0;
                }
            }

            return result; },
        normalize_data);
}

// *********************************************************************************************

/**
 * @brief Construct a new UOSignal object.
 *
 * @param short_period Short period value. Default is 7.
 * @param medium_period Medium period value. Default is 14.
 * @param long_period Long period value. Default is 28.
 * @param overbought Overbought value. Default is 70.
 * @param oversold Oversold value. Default is 30.
 * @param offset Offset value. Default is 0.
 */
UOSignal::UOSignal(int short_period, int medium_period, int long_period, int overbought, int oversold, int offset) : Indicator("Ultimate Oscillator (Signal)", "uo-signal", {{"short_period", short_period}, {"medium_period", medium_period}, {"long_period", long_period}, {"overbought", overbought}, {"oversold", oversold}, {"offset", offset}}, {-1, 1}) {}

/**
 * @brief Calculate the UOSignal values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> UOSignal::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles)
        {
            int short_period = std::get<int>(this->params.at("short_period"));
            int medium_period = std::get<int>(this->params.at("medium_period"));
            int long_period = std::get<int>(this->params.at("long_period"));
            int overbought = std::get<int>(this->params.at("overbought"));
            int oversold = std::get<int>(this->params.at("oversold"));
            int offset = std::get<int>(this->params.at("offset"));

            std::vector<double> result(candles.size(), 0.0);
            std::vector<double> uo_values = UO(short_period, medium_period, long_period, offset).calculate(candles, false);

            for (size_t i = 1; i < uo_values.size(); i++)
            {
                if (uo_values[i - 1] < oversold && uo_values[i] > oversold)
                {
                    result[i] = 1.0;
                }
                else if (uo_values[i - 1] > overbought && uo_values[i] < overbought)
                {
                    result[i] = -1.0;
                }
            }

            return result; },
        normalize_data);
}

// *********************************************************************************************

/**
 * @brief Construct a new WPRSignal object.
 *
 * @param period Period value. Default is 14.
 * @param overbought Overbought value. Default is -20.
 * @param oversold Oversold value. Default is -80.
 * @param offset Offset value. Default is 0.
 */
WPRSignal::WPRSignal(int period, int overbought, int oversold, int offset) : Indicator("Williams %R (Signal)", "wpr-signal", {{"period", period}, {"overbought", overbought}, {"oversold", oversold}, {"offset", offset}}, {-1, 1}) {}

/**
 * @brief Calculate the WPRSignal values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> WPRSignal::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles)
        {
            int period = std::get<int>(this->params.at("period"));
            int overbought = std::get<int>(this->params.at("overbought"));
            int oversold = std::get<int>(this->params.at("oversold"));
            int offset = std::get<int>(this->params.at("offset"));

            std::vector<double> result(candles.size(), 0.0);
            std::vector<double> williams_r_values = WPR(period, offset).calculate(candles, false);

            for (size_t i = 1; i < williams_r_values.size(); i++)
            {
                if (williams_r_values[i - 1] < oversold && williams_r_values[i] > oversold)
                {
                    result[i] = 1.0;
                }
                else if (williams_r_values[i - 1] > overbought && williams_r_values[i] < overbought)
                {
                    result[i] = -1.0;
                }
            }

            return result; },
        normalize_data);
}
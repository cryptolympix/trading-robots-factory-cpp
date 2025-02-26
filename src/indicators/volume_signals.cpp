#include <vector>
#include "indicator.hpp"
#include "volume.hpp"
#include "volume_signals.hpp"
#include "utils.hpp"

/**
 * @brief Construct a new CMFSignal object.
 *
 * @param period Period for the CMF. Default is 20.
 * @param bullish_threshold Bullish threshold. Default is 0.1.
 * @param bearish_threshold Bearish threshold. Default is -0.1.
 * @param offset Offset value. Default is 0.
 */
CMFSignal::CMFSignal(int period, double bullish_threshold, double bearish_threshold, int offset) : Indicator("Chaikin Money Flow (Signal)", "cmf-signal", {{"period", period}, {"bullish_threshold", bullish_threshold}, {"bearish_threshold", bearish_threshold}, {"offset", offset}}, {-1, 1}) {}

/**
 * @brief Calculate the CMFSignal values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> CMFSignal::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles)
        {
            int period = std::get<int>(this->params.at("period"));
            double bullish_threshold = std::get<double>(this->params.at("bullish_threshold"));
            double bearish_threshold = std::get<double>(this->params.at("bearish_threshold"));
            int offset = std::get<int>(this->params.at("offset"));
            
            std::vector<double> result(candles.size(), 0.0);
            std::vector<double> cmf_values = CMF(period, offset).calculate(candles, false);

            for (size_t i = 1; i < cmf_values.size(); ++i)
            {
                if (cmf_values[i - 1] < bullish_threshold && cmf_values[i] > bullish_threshold)
                {
                    result[i] = 1;
                }
                else if (cmf_values[i - 1] > bearish_threshold && cmf_values[i] < bearish_threshold)
                {
                    result[i] = -1;
                }
            }

            return result; },
        normalize_data);
}

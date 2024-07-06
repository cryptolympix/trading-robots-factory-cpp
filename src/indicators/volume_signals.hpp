#ifndef VOLUME_SIGNALS_HPP
#define VOLUME_SIGNALS_HPP

#include <vector>
#include "indicator.hpp"

class CMFSignal : public Indicator
{
public:
    std::string label = "Chaikin Money Flow (Signal)";
    std::string id = "cmf-signal";
    std::string id_pattern = R"(cmf-signal-(\d+)-(-?\d+.\d+)-(-?\d+.\d+)-(\d+))";

    /**
     * @brief Construct a new CMFSignal object.
     *
     * @param period Period for the CMF. Default is 20.
     * @param bullish_threshold Bullish threshold. Default is 0.1.
     * @param bearish_threshold Bearish threshold. Default is -0.1.
     * @param offset Offset value. Default is 0.
     */
    CMFSignal(int period = 20, double bullish_threshold = 0.1, double bearish_threshold = -0.1, int offset = 0);

    /**
     * @brief Calculate the ATRSignal values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;

private:
    int period;
    double bullish_threshold;
    double bearish_threshold;
};

#endif // VOLUME_SIGNALS_HPP

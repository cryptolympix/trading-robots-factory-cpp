#ifndef VOLUME_SIGNALS_HPP
#define VOLUME_SIGNALS_HPP

#include <vector>
#include <functional>
#include <unordered_map>
#include <stdexcept>
#include <iostream>
#include "builder.hpp"
#include "indicator.hpp"

class CMFSignal : public Indicator
{
public:
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
};

const std::unordered_map<std::string, std::function<Indicator *(std::unordered_map<std::string, IndicatorParam>)>> volume_signals_indicators_map = {
    {"cmf-signal", [](std::unordered_map<std::string, IndicatorParam> params) -> Indicator *
     {
         try
         {
             if (check_params(params, {{"period", typeid(int)}, {"bullish_threshold", typeid(double)}, {"bearish_threshold", typeid(double)}, {"offset", typeid(int)}}))
             {
                 int period = std::get<int>(params["period"]);
                 double bullish_threshold = std::get<double>(params["bullish_threshold"]);
                 double bearish_threshold = std::get<double>(params["bearish_threshold"]);
                 int offset = std::get<int>(params["offset"]);
                 return new CMFSignal(period, bullish_threshold, bearish_threshold, offset);
             }
         }
         catch (const std::exception &e)
         {
             std::cerr << "Error creating CMFSignal: " << e.what() << std::endl;
             std::exit(1);
         }

         return nullptr;
     }},
};

#endif // VOLUME_SIGNALS_HPP

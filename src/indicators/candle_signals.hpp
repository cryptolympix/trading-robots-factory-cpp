#ifndef CANDLE_SIGNALS_INDICATORS_HPP
#define CANDLE_SIGNALS_INDICATORS_HPP

#include <string>
#include <vector>
#include <functional>
#include <unordered_map>
#include <stdexcept>
#include <iostream>
#include "../types.hpp"
#include "builder.hpp"
#include "indicator.hpp"

class NewHighSignal : public Indicator
{
public:
    /**
     * @brief Construct a New High Signal object.
     *
     * @param left_bars Number of left bars.
     * @param right_bars Number of right bars.
     * @param offset Offset value. Default is 0.
     */
    NewHighSignal(int left_bars = 5, int right_bars = 0, int offset = 0);

    /**
     * @brief Calculate the New High Signal values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;
};

// *********************************************************************************************

class NewLowSignal : public Indicator
{
public:
    /**
     * @brief Construct a New Low Signal object.
     *
     * @param left_bars Number of left bars.
     * @param right_bars Number of right bars.
     * @param offset Offset value. Default is 0.
     */
    NewLowSignal(int left_bars = 5, int right_bars = 0, int offset = 0);

    /**
     * @brief Calculate the New Low Signal values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;
};

// *********************************************************************************************

class HighBreakSignal : public Indicator
{
public:
    /**
     * @brief Construct a new High Break Signal object.
     *
     * @param left_bars Number of left bars.
     * @param right_bars Number of right bars.
     * @param offset Offset value. Default is 0.
     */
    HighBreakSignal(int left_bars = 5, int right_bars = 0, int offset = 0);

    /**
     * @brief Calculate the High Break Signal values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;
};

// *********************************************************************************************

class LowBreakSignal : public Indicator
{
public:
    /**
     * @brief Construct a new Low Break Signal object.
     *
     * @param left_bars Number of left bars.
     * @param right_bars Number of right bars.
     * @param offset Offset value. Default is 0.
     */
    LowBreakSignal(int left_bars = 5, int right_bars = 0, int offset = 0);

    /**
     * @brief Calculate the Low Break Signal values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;
};

const std::unordered_map<std::string, std::function<Indicator *(std::unordered_map<std::string, IndicatorParam>)>> candle_signals_indicators_map = {
    {"new-high-signal", [](std::unordered_map<std::string, IndicatorParam> params) -> Indicator *
     {
         try
         {
             if (check_params(params, {{"left_bars", typeid(int)}, {"right_bars", typeid(int)}, {"offset", typeid(int)}}))
             {
                 int left_bars = std::get<int>(params["left_bars"]);
                 int right_bars = std::get<int>(params["right_bars"]);
                 int offset = std::get<int>(params["offset"]);
                 return new NewHighSignal(left_bars, right_bars, offset);
             }
         }
         catch (const std::exception &e)
         {
             std::cerr << "Error creating NewHighSignal: " << e.what() << std::endl;
             std::exit(1);
         }

         return nullptr;
     }},
    {"new-low-signal", [](std::unordered_map<std::string, IndicatorParam> params) -> Indicator *
     {
         try
         {
             if (check_params(params, {{"left_bars", typeid(int)}, {"right_bars", typeid(int)}, {"offset", typeid(int)}}))
             {
                 int left_bars = std::get<int>(params["left_bars"]);
                 int right_bars = std::get<int>(params["right_bars"]);
                 int offset = std::get<int>(params["offset"]);
                 return new NewLowSignal(left_bars, right_bars, offset);
             }
         }
         catch (const std::exception &e)
         {
             std::cerr << "Error creating NewLowSignal: " << e.what() << std::endl;
             std::exit(1);
         }

         return nullptr;
     }},
    {"high-break-signal", [](std::unordered_map<std::string, IndicatorParam> params) -> Indicator *
     {
         try
         {
             if (check_params(params, {{"left_bars", typeid(int)}, {"right_bars", typeid(int)}, {"offset", typeid(int)}}))
             {
                 int left_bars = std::get<int>(params["left_bars"]);
                 int right_bars = std::get<int>(params["right_bars"]);
                 int offset = std::get<int>(params["offset"]);
                 return new HighBreakSignal(left_bars, right_bars, offset);
             }
         }
         catch (const std::exception &e)
         {
             std::cerr << "Error creating HighBreakSignal: " << e.what() << std::endl;
             std::exit(1);
         }

         return nullptr;
     }},
    {"low-break-signal", [](std::unordered_map<std::string, IndicatorParam> params) -> Indicator *
     {
         try
         {
             if (check_params(params, {{"left_bars", typeid(int)}, {"right_bars", typeid(int)}, {"offset", typeid(int)}}))
             {
                 int left_bars = std::get<int>(params["left_bars"]);
                 int right_bars = std::get<int>(params["right_bars"]);
                 int offset = std::get<int>(params["offset"]);
                 return new LowBreakSignal(left_bars, right_bars, offset);
             }
         }
         catch (const std::exception &e)
         {
             std::cerr << "Error creating LowBreakSignal: " << e.what() << std::endl;
             std::exit(1);
         }

         return nullptr;
     }},
};

#endif // CANDLE_SIGNALS_INDICATORS_HPP
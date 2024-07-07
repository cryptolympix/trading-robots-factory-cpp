#ifndef CANDLE_SIGNALS_INDICATORS_HPP
#define CANDLE_SIGNALS_INDICATORS_HPP

#include "../types.hpp"
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

#endif // CANDLE_SIGNALS_INDICATORS_HPP
#ifndef TIME_INDICATORS_HPP
#define TIME_INDICATORS_HPP

#include "../types.hpp"
#include "indicator.hpp"

/**
 * @brief Class for Hour indicator.
 */
class Hour : public Indicator
{
public:
    /**
     * @brief Construct a new Hour object.
     *
     * @param offset Offset value. Default is 0.
     */
    Hour(int offset = 0);

    /**
     * @brief Calculate the Hour values.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing calculated values.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;
};

// *********************************************************************************************

class NFPWeek : public Indicator
{
public:
    /**
     * @brief Construct a new NFPWeek object.
     *
     * @param offset Offset value. Default is 0.
     */
    NFPWeek(int offset = 0);

    /**
     * @brief Check if the candle is on NFP week.
     *
     * @param candles Vector of Candle data.
     * @return std::vector<double> Vector containing 1 if the candle is on NFP week, 0 otherwise.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;
};

// *********************************************************************************************

class MarketSession : public Indicator
{
public:
    /**
     * @brief Construct a new MarketSession object.
     *
     * @param zone Market session zone.
     * @param offset Offset value. Default is 0.
     */
    MarketSession(std::string zone, int offset = 0);

    /**
     * @brief Check if the candle is on a market session.
     *
     * @param candles Vector of Candle data.
     * @param normalize_data Boolean flag indicating whether to normalize data.
     * @return std::vector<double> Vector containing 1 if the candle is on the market session, 0 otherwise.
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;

private:
    std::string zone; // Market session zone
};

// *********************************************************************************************

class WeekDay : public Indicator
{
public:
    /**
     * @brief Construct a new WeekDay object.
     *
     * @param offset Offset value. Default is 0.
     */
    WeekDay(int offset = 0);

    /**
     * @brief Get the days of the candle.
     *
     * @param candles Vector of Candle data.
     * @return std::vector<double> Vector containing the number of each candle day (0-6).
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;
};

#endif // TIME_INDICATORS_HPP
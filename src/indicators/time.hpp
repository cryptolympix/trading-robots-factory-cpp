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

/**
 * @brief Class for Minute indicator.
 */
class Minute : public Indicator
{
public:
    /**
     * @brief Construct a new Minute object.
     *
     * @param offset Offset value. Default is 0.
     */
    Minute(int offset = 0);

    /**
     * @brief Calculate the Minute values.
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
};

// *********************************************************************************************

class WeekDay : public Indicator
{
public:
    /**
     * @brief Construct a new WeekDay object.
     *
     * @param day Day of the week.
     * @param offset Offset value. Default is 0.
     */
    WeekDay(std::string day, int offset = 0);

    /**
     * @brief Get the days of the candle.
     *
     * @param candles Vector of Candle data.
     * @return std::vector<double> Vector containing the number of each candle day (0-6).
     */
    std::vector<double> calculate(const std::vector<Candle> &candles, bool normalize_data = false) const override;
};

const std::unordered_map<std::string, std::function<Indicator *(std::vector<IndicatorParam>)>> time_indicators_map = {
    {"hour", [](std::vector<IndicatorParam> params) -> Indicator *
     {
         int offset = std::get<int>(params[0]);
         return new Hour(offset);
     }},
    {"minute", [](std::vector<IndicatorParam> params) -> Indicator *
     {
         int offset = std::get<int>(params[0]);
         return new Minute(offset);
     }},
    {"nfp-week", [](std::vector<IndicatorParam> params) -> Indicator *
     {
         int offset = std::get<int>(params[0]);
         return new NFPWeek(offset);
     }},
    {"market-session", [](std::vector<IndicatorParam> params) -> Indicator *
     {
         std::string zone = std::get<std::string>(params[0]);
         int offset = std::get<int>(params[1]);
         return new MarketSession(zone, offset);
     }},
    {"weekday", [](std::vector<IndicatorParam> params) -> Indicator *
     {
         std::string day = std::get<std::string>(params[0]);
         int offset = std::get<int>(params[1]);
         return new WeekDay(day, offset);
     }},
};

#endif // TIME_INDICATORS_HPP
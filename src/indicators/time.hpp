#ifndef TIME_INDICATORS_HPP
#define TIME_INDICATORS_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <stdexcept>
#include <iostream>
#include "builder.hpp"
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

const std::unordered_map<std::string, std::function<Indicator *(std::unordered_map<std::string, IndicatorParam>)>> time_indicators_map = {
    {"hour", [](std::unordered_map<std::string, IndicatorParam> params) -> Indicator *
     {
         try
         {
             if (check_params(params, {{"offset", typeid(int)}}))
             {
                 int offset = std::get<int>(params["offset"]);
                 return new Hour(offset);
             }
         }
         catch (const std::exception &e)
         {
             std::cerr << "Error creating Hour: " << e.what() << std::endl;
             std::exit(1);
         }

         return nullptr;
     }},
    {"minute", [](std::unordered_map<std::string, IndicatorParam> params) -> Indicator *
     {
         try
         {
             if (check_params(params, {{"offset", typeid(int)}}))
             {
                 int offset = std::get<int>(params["offset"]);
                 return new Minute(offset);
             }
         }
         catch (const std::exception &e)
         {
             std::cerr << "Error creating Minute: " << e.what() << std::endl;
             std::exit(1);
         }

         return nullptr;
     }},
    {"nfp-week", [](std::unordered_map<std::string, IndicatorParam> params) -> Indicator *
     {
         try
         {
             if (check_params(params, {{"offset", typeid(int)}}))
             {
                 int offset = std::get<int>(params["offset"]);
                 return new NFPWeek(offset);
             }
         }
         catch (const std::exception &e)
         {
             std::cerr << "Error creating NFPWeek: " << e.what() << std::endl;
             std::exit(1);
         }

         return nullptr;
     }},
    {"market-session", [](std::unordered_map<std::string, IndicatorParam> params) -> Indicator *
     {
         try
         {
             if (check_params(params, {{"offset", typeid(int)}, {"zone", typeid(std::string)}}))
             {
                 int offset = std::get<int>(params["offset"]);
                 std::string zone = std::get<std::string>(params["zone"]);
                 return new MarketSession(zone, offset);
             }
         }
         catch (const std::exception &e)
         {
             std::cerr << "Error creating MarketSession: " << e.what() << std::endl;
             std::exit(1);
         }

         return nullptr;
     }},
    {"week-day", [](std::unordered_map<std::string, IndicatorParam> params) -> Indicator *
     {
         try
         {
             if (check_params(params, {{"offset", typeid(int)}, {"day", typeid(std::string)}}))
             {
                 int offset = std::get<int>(params["offset"]);
                 std::string day = std::get<std::string>(params["day"]);
                 return new WeekDay(day, offset);
             }
         }
         catch (const std::exception &e)
         {
             std::cerr << "Error creating WeekDay: " << e.what() << std::endl;
             std::exit(1);
         }

         return nullptr;
     }},
};

#endif // TIME_INDICATORS_HPP
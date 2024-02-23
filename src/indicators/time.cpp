#include <ctime>
#include <string>
#include <cctype> // for std::toupper
#include "../types.hpp"
#include "time.hpp"

/**
 * @brief Construct a new Hour object.
 *
 * @param offset Offset value. Default is 0.
 */
Hour::Hour(int offset) : Indicator("Hour", "hour-" + std::to_string(offset), offset) {}

/**
 * @brief Calculate the Hour values.
 *
 * @param candles Vector of Candle data.
 * @param normalize_data Boolean flag indicating whether to normalize data.
 * @return std::vector<double> Vector containing calculated values.
 */
std::vector<double> Hour::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles) -> std::vector<double>
        {
            std::vector<double> values;
            values.reserve(candles.size()); // Reserve memory for the values vector

            for (const auto &candle : candles)
            {
                struct tm *time = localtime(&candle.date);
                values.push_back(time->tm_hour);
            }

            return values; },

        normalize_data);
}

// *********************************************************************************************

/**
 * @brief Construct a new NFPWeek object.
 *
 * @param offset Offset value. Default is 0.
 */
NFPWeek::NFPWeek(int offset) : Indicator("NFP Week", "nfp-week-" + std::to_string(offset), offset) {}

/**
 * @brief Check if the candle is on NFP week.
 *
 * @param candles Vector of Candle data.
 * @return std::vector<double> Vector containing 1 if the candle is on NFP week, 0 otherwise.
 */
std::vector<double> NFPWeek::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles) -> std::vector<double>
        {
            std::vector<double> result;
            result.reserve(candles.size());

            for (const auto &candle : candles)
            {
                // Check if the candle's date falls within the NFP week (assuming NFP week is the first week of each month)
                struct tm *time = localtime(&candle.date);
                bool is_nfp_week = time->tm_mday >= 1 && time->tm_mday <= 7;
                result.push_back(is_nfp_week ? 1.0 : 0.0);
            }

            return result; },

        normalize_data);
}

// *********************************************************************************************

/**
 * @brief Map for different market session zone.
 */
std::map<MarketSessionZone, std::string> MarketSessionZoneMap = {
    {MarketSessionZone::LONDON, "london"},
    {MarketSessionZone::NEW_YORK, "new-york"},
    {MarketSessionZone::TOKYO, "tokyo"}};

/**
 * @brief Construct a new MarketSession object.
 *
 * @param offset Offset value. Default is 0.
 */
MarketSession::MarketSession(MarketSessionZone zone, int offset) : Indicator("Market Session " + std::string(1, std::toupper(MarketSessionZoneMap[zone][0])), "market-session-" + MarketSessionZoneMap[zone] + "-" + std::to_string(offset), offset), zone(zone) {}

/**
 * @brief Check if the candle is on a market session.
 *
 * @param candles Vector of Candle data.
 * @return std::vector<double> Vector containing 1 if the candle is on the market session, 0 otherwise.
 */
std::vector<double> MarketSession::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles) -> std::vector<double>
        {
            std::vector<double> result;
            result.reserve(candles.size());

            for (const auto &candle : candles)
            {
                // Check if the candle's date falls within the market session
                struct tm *time = localtime(&candle.date);
                bool is_market_session = false;
                switch (zone)
                {
                    case MarketSessionZone::LONDON:
                        is_market_session = time->tm_hour >= 8 && time->tm_hour <= 12;
                        break;
                    case MarketSessionZone::NEW_YORK:
                        is_market_session = time->tm_hour >= 14 && time->tm_hour <= 20;
                        break;
                    case MarketSessionZone::TOKYO:
                        is_market_session = time->tm_hour >= 2 && time->tm_hour <= 8;
                        break;
                }
                result.push_back(is_market_session ? 1.0 : 0.0);
            }

            return result; },

        normalize_data);
}

// *********************************************************************************************

/**
 * @brief Construct a new WeekDay object.
 *
 * @param offset Offset value. Default is 0.
 */
WeekDay::WeekDay(int offset) : Indicator("Week Day", "week-day-" + std::to_string(offset), offset) {}

/**
 * @brief Get the days of the candle.
 *
 * @param candles Vector of Candle data.
 * @return std::vector<double> Vector containing the number of each candle day (0-6).
 */
std::vector<double> WeekDay::calculate(const std::vector<Candle> &candles, bool normalize_data) const
{
    return Indicator::calculate(
        candles, [this](std::vector<Candle> candles) -> std::vector<double>
        {
            std::vector<double> result;
            result.reserve(candles.size());

            for (const auto &candle : candles)
            {
                // Convert time_t to std::tm
                std::time_t t = candle.date;
                std::tm *time = std::localtime(&t);

                // Extract the weekday (Sunday is 0, Monday is 1, etc.)
                result.push_back(time->tm_wday);
            }
            
            return result; },

        normalize_data);
}
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
                tm *time = localtime(&candle.date);
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
                tm *time = localtime(&candle.date);
                bool is_nfp_week = time->tm_mday >= 1 && time->tm_mday <= 7;
                result.push_back(is_nfp_week ? 1.0 : 0.0);
            }

            return result; },

        normalize_data);
}

// *********************************************************************************************

/**
 * @brief Construct a new MarketSession object.
 *
 * @param zone Market session zone.
 * @param offset Offset value. Default is 0.
 */
MarketSession::MarketSession(std::string zone, int offset) : Indicator(zone == "new-york" ? "New York Market Session" : zone == "london" ? "London Market Session"
                                                                                                                                         : "Tokyo Market Session",
                                                                       "market-session-" + zone + "-" + std::to_string(offset), offset),
                                                             zone(zone) {}

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
                tm *time = localtime(&candle.date);
                bool is_market_session = false;

                if (zone == "london") {
                    is_market_session = time->tm_hour >= 8 && time->tm_hour <= 12;
                } else if (zone == "new-york") {
                    is_market_session = time->tm_hour >= 14 && time->tm_hour <= 20;
                } else if (zone == "tokyo") {
                    is_market_session = time->tm_hour >= 2 && time->tm_hour <= 8;
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
 * @param day Week day.
 * @param offset Offset value. Default is 0.
 */
WeekDay::WeekDay(std::string day, int offset) : Indicator("Week Day (" + day + ")", "week-day-" + day + "-" + std::to_string(offset), offset), day(day) {}

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

            int attempt_day = 0;
            if (day == "sunday") {
                attempt_day = 0;
            } else if (day == "monday") {
                attempt_day = 1;
            } else if (day == "tuesday") {
                attempt_day = 2;
            } else if (day == "wednesday") {
                attempt_day = 3;
            } else if (day == "thursday") {
                attempt_day = 4;
            } else if (day == "friday") {
                attempt_day = 5;
            } else if (day == "saturday") {
                attempt_day = 6;
            }

            for (const auto &candle : candles)
            {
                // Convert time_t to std::tm
                tm *time = localtime(&candle.date);

                // Extract the weekday (Sunday is 0, Monday is 1, etc.)
                int candle_day = time->tm_wday;

                // Check if the candle's date falls on the specified day
                result.push_back(candle_day == attempt_day ? 1.0 : 0.0);
            }
            
            return result; },

        normalize_data);
}
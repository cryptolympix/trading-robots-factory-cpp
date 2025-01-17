#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <ctime>
#include <vector>
#include "../utils/date_conversion.hpp"
#include "../types.hpp"
#include "schedule.hpp"

/**
 * @brief Get the schedule for the given day.
 *
 * @param day The day of the week (0 = Sunday, 1 = Monday, ..., 6 = Saturday).
 * @param trading_schedule The trading schedule for each day of the week.
 * @return The schedule for the given day.
 */
std::vector<bool> get_day_schedule(const int day, const TradingSchedule &trading_schedule)
{
    std::vector<bool> day_schedule;
    switch (day)
    {
    case 0:
        day_schedule = trading_schedule.sunday;
        break;
    case 1:
        day_schedule = trading_schedule.monday;
        break;
    case 2:
        day_schedule = trading_schedule.tuesday;
        break;
    case 3:
        day_schedule = trading_schedule.wednesday;
        break;
    case 4:
        day_schedule = trading_schedule.thursday;
        break;
    case 5:
        day_schedule = trading_schedule.friday;
        break;
    case 6:
        day_schedule = trading_schedule.saturday;
        break;
    default:
        day_schedule = trading_schedule.sunday; // Default to Monday if invalid day
        break;
    }

    return day_schedule;
}

/**
 * @brief Check if the given date and time are within the specified trading schedule.
 *
 * @param date The date and time to be checked against the trading schedule.
 * @param trading_schedule The trading schedule for each day of the week.
 * @return True if the provided date and time are within the trading schedule; False otherwise.
 */
bool is_on_trading_schedule(const time_t date, const TradingSchedule &trading_schedule)
{
    std::tm date_tm = time_t_to_tm(date);
    int day = date_tm.tm_wday;
    std::vector<bool> day_schedule = get_day_schedule(day, trading_schedule);
    return day_schedule[date_tm.tm_hour];
}

/**
 * @brief Check if there is at least an hour of trading session for the given day.
 * @param trading_schedule The trading schedule for each day of the week.
 * @return True if there is at least an hour of trading session for the given day; False otherwise.
 */
bool has_session_for_day(const int day, const TradingSchedule &trading_schedule)
{
    std::vector<bool> day_schedule = get_day_schedule(day, trading_schedule);

    for (int i = 0; i < 24; i++)
    {
        if (day_schedule[i])
        {
            return true;
        }
    }
    return false;
}

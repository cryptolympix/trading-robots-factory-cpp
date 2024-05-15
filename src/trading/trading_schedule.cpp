#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <ctime>
#include <vector>
#include "../utils/date_conversion.hpp"
#include "../types.hpp"
#include "trading_schedule.hpp"

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

    return day_schedule[date_tm.tm_hour];
}

#include <ctime>
#include "../types.hpp"
#include "trading_schedule.hpp"

/**
 * @brief Check if the given date and time are within the specified trading schedule.
 *
 * @param date The date and time to be checked against the trading schedule.
 * @param trading_schedule The trading schedule for each day of the week.
 * @return True if the provided date and time are within the trading schedule; False otherwise.
 */
bool is_on_trading_schedule(const std::tm &date, const TradingSchedule &trading_schedule)
{
    int day = date.tm_wday;

    std::vector<bool> day_schedule;
    switch (day)
    {
    case 0:
        day_schedule = trading_schedule.monday;
        break;
    case 1:
        day_schedule = trading_schedule.tuesday;
        break;
    case 2:
        day_schedule = trading_schedule.wednesday;
        break;
    case 3:
        day_schedule = trading_schedule.thursday;
        break;
    case 4:
        day_schedule = trading_schedule.friday;
        break;
    case 5:
        day_schedule = trading_schedule.saturday;
        break;
    case 6:
        day_schedule = trading_schedule.sunday;
        break;
    default:
        day_schedule = trading_schedule.monday; // Default to Monday if invalid day
        break;
    }

    return day_schedule[date.tm_hour];
}

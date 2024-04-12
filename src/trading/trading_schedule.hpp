#ifndef TRADING_SCHEDULE_H
#define TRADING_SCHEDULE_H

#include <array>
#include <ctime>
#include "../types.hpp"

/**
 * @brief Check if the given date and time are within the specified trading schedule.
 *
 * @param date The date and time to be checked against the trading schedule.
 * @param trading_schedule The trading schedule for each day of the week.
 * @return True if the provided date and time are within the trading schedule; False otherwise.
 */
bool is_on_trading_schedule(const time_t date, const TradingSchedule &trading_schedule);

#endif // TRADING_SCHEDULE_H

#ifndef TRADING_SCHEDULE_H
#define TRADING_SCHEDULE_H

#include <array>
#include <ctime>
#include "../types.hpp"

/**
 * @brief Get the schedule for the given day.
 *
 * @param day The day of the week (0 = Sunday, 1 = Monday, ..., 6 = Saturday).
 * @param trading_schedule The trading schedule for each day of the week.
 * @return The schedule for the given day.
 */
std::vector<bool> get_day_schedule(const int day, const TradingSchedule &trading_schedule);

/**
 * @brief Check if the given date and time are within the specified trading schedule.
 *
 * @param date The date and time to be checked against the trading schedule.
 * @param trading_schedule The trading schedule for each day of the week.
 * @return True if the provided date and time are within the trading schedule; False otherwise.
 */
bool is_on_trading_schedule(const time_t date, const TradingSchedule &trading_schedule);

/**
 * @brief Check if there is at least an hour of trading session for the given day.
 * @param trading_schedule The trading schedule for each day of the week.
 * @return True if there is at least an hour of trading session for the given day; False otherwise.
 */
bool has_session_for_day(const int day, const TradingSchedule &trading_schedule);

#endif // TRADING_SCHEDULE_H

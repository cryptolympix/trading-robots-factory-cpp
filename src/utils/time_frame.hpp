#ifndef TIMEFRAME_H
#define TIMEFRAME_H

#include <regex>
#include <stdexcept>
#include <string>
#include <vector>
#include "../types.hpp"

/**
 * @brief Get the number of minutes for the specified timeframe.
 *
 * @param time_frame The timeframe for which the number of minutes is requested.
 * @return int The number of minutes corresponding to the specified timeframe.
 */
int get_time_frame_in_minutes(TimeFrame time_frame);

/**
 * @brief Check if the given time matches the specified format for the given timeframe.
 *
 * @param time The time string to be checked.
 * @param time_frame The timeframe for which the time format is validated.
 * @return true If the time string matches the expected format.
 * @return false Otherwise.
 */
bool match_time_frame(const std::string &time, TimeFrame time_frame);

/**
 * @brief Determine the highest (maximum) timeframe from the given list.
 *
 * @param time_frames The list of timeframes to evaluate.
 * @return TimeFrame The highest timeframe from the list.
 */
TimeFrame highest_time_frame(const std::vector<TimeFrame> &time_frames);

/**
 * @brief Determine the lowest (minimum) timeframe from the given list.
 *
 * @param time_frames The list of timeframes to evaluate.
 * @return TimeFrame The lowest timeframe from the list.
 */
TimeFrame lowest_time_frame(const std::vector<TimeFrame> &time_frames);

/**
 * @brief Convert a timeframe to a string representation.
 * @return std::string The string representation of the timeframe.
 */
std::string time_frame_to_string(TimeFrame time_frame);

/**
 * @brief Convert a string representation of a timeframe to a TimeFrame enum.
 * @return TimeFrame The TimeFrame enum corresponding to the string representation.
 */
TimeFrame time_frame_from_string(const std::string &time_frame);

#endif /* TIMEFRAME_H */

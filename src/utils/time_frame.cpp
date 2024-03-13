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
int get_time_frame_value(TimeFrame time_frame)
{
    switch (time_frame)
    {
    case TimeFrame::M1:
        return 1;
    case TimeFrame::M5:
        return 5;
    case TimeFrame::M15:
        return 15;
    case TimeFrame::M30:
        return 30;
    case TimeFrame::H1:
        return 60;
    case TimeFrame::H4:
        return 60 * 4;
    case TimeFrame::H12:
        return 60 * 12;
    case TimeFrame::D1:
        return 60 * 24;
    default:
        throw std::runtime_error("The time frame is invalid.");
    }
}

/**
 * @brief Check if the given time matches the specified format for the given timeframe.
 *
 * @param time The time string to be checked.
 * @param time_frame The timeframe for which the time format is validated.
 * @return true If the time string matches the expected format.
 * @return false Otherwise.
 */
bool match_time_frame(const std::string &time, TimeFrame time_frame)
{
    switch (time_frame)
    {
    case TimeFrame::M5:
        return std::regex_match(time, std::regex("^(0[0-9]|1[0-9]|2[0-3]):[0-5][05]:(00)$"));
    case TimeFrame::M15:
        return std::regex_match(time, std::regex("^(0[0-9]|1[0-9]|2[0-3]):(00|15|30|45):(00)$"));
    case TimeFrame::M30:
        return std::regex_match(time, std::regex("^(0[0-9]|1[0-9]|2[0-3]):(00|30):(00)$"));
    case TimeFrame::H1:
        return std::regex_match(time, std::regex("^(0[0-9]|1[0-9]|2[0-3]):(00):(00)$"));
    case TimeFrame::H4:
        return std::regex_match(time, std::regex("^(0[048]|1[26]|2[0]):(00):(00)$"));
    case TimeFrame::H12:
        return std::regex_match(time, std::regex("^(00|12):(00):(00)$"));
    case TimeFrame::D1:
        return std::regex_match(time, std::regex("^(00):(00):(00)$"));
    default:
        return false;
    }
}

/**
 * @brief Determine the highest (maximum) timeframe from the given list.
 *
 * @param time_frames The list of timeframes to evaluate.
 * @return TimeFrame The highest timeframe from the list.
 */
TimeFrame highest_time_frame(const std::vector<TimeFrame> &time_frames)
{
    std::vector<TimeFrame> time_frames_copy = time_frames;
    if (time_frames_copy.empty())
    {
        throw std::runtime_error("At least one time frame must be passed to the arguments.");
    }
    else if (time_frames_copy.size() == 1)
    {
        return time_frames_copy[0];
    }
    else
    {
        std::sort(time_frames_copy.begin(), time_frames_copy.end(), [](TimeFrame a, TimeFrame b)
                  { return get_time_frame_value(a) > get_time_frame_value(b); });
        return time_frames_copy[0];
    }
}

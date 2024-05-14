#include <ctime>
#include <string>
#include <sstream>
#include <iomanip>
#include "date.hpp"

/**
 * @brief Convert time_t to string.
 * @param time Time in time_t format.
 * @param format Format of the time.
 * @return Time in string format.
 */
std::string time_t_to_string(time_t time, const std::string &format)
{
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), format.c_str());
    return ss.str();
}

/**
 * @brief Convert string to time_t.
 * @param time Time in string format.
 * @param format Format of the time.
 * @return Time in time_t format.
 */
time_t string_to_time_t(const std::string &time, const std::string &format)
{
    std::tm tm = {};
    std::istringstream ss(time);
    ss >> std::get_time(&tm, format.c_str());
    return std::mktime(&tm);
}

/**
 * @brief Convert time_t to std::tm.
 * @param time Time in time_t format.
 * @return Time in std::tm format.
 */
std::tm time_t_to_tm(time_t time)
{
    return *std::localtime(&time);
}

/**
 * @brief Convert std::tm to string.
 * @param tm Time in std::tm format.
 * @param format Format of the time.
 * @return Time in string format.
 */
std::tm string_to_tm(const std::string &time, const std::string &format)
{
    std::tm tm = {};
    std::istringstream ss(time);
    ss >> std::get_time(&tm, format.c_str());
    return tm;
}
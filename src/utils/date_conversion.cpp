#define _CRT_SECURE_NO_WARNINGS

#include <ctime>
#include <string>
#include <sstream>
#include <iomanip>
#include "date_conversion.hpp"

/**
 * @brief Convert time_t to string.
 * @param time Time in time_t format.
 * @param format Format of the time.
 * @return Time in string format.
 */
std::string time_t_to_string(time_t time, const std::string &format)
{
    std::stringstream ss;
#ifdef _WIN32
    ss << std::put_time(std::gmtime(&time), format.c_str());
#else
    ss << std::put_time(std::localtime(&time), format.c_str());
#endif
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
    tm.tm_isdst = -1; // This is important to avoid daylight saving time issues.
    std::istringstream ss(time);
    ss >> std::get_time(&tm, format.c_str());
    return tm_to_time_t(tm);
}

/**
 * @brief Convert time_t to std::tm.
 * @param time Time in time_t format.
 * @return Time in std::tm format.
 */
std::tm time_t_to_tm(time_t time)
{
#ifdef _WIN32
    return *std::gmtime(&time);
#else
    return *std::localtime(&time);
#endif
}

/**
 * @brief Convert std::tm to time_t.
 * @param time Time in std::tm format.
 * @return Time in time_t format.
 */
time_t tm_to_time_t(const std::tm &time)
{
    return std::mktime(const_cast<std::tm *>(&time));
}

/**
 * @brief Convert std::tm to string.
 * @param time Time in std::tm format.
 * @param format Format of the time.
 * @return Time in string format.
 */
std::string tm_to_string(const std::tm &time, const std::string &format)
{
    std::stringstream ss;
    ss << std::put_time(&time, format.c_str());
    return ss.str();
}

/**
 * @brief Convert string to std::tm.
 * @param time Time in string format.
 * @param format Format of the time.
 * @return Time in std::tm format.
 */
std::tm string_to_tm(const std::string &time, const std::string &format)
{
    std::tm tm = {};
    tm.tm_isdst = -1; // This is important to avoid daylight saving time issues.
    std::istringstream ss(time);
    ss >> std::get_time(&tm, format.c_str());
    tm.tm_isdst = -1;
    return tm;
}

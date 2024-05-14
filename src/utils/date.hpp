#ifndef DATE_HPP
#define DATE_HPP

#include <ctime>
#include <string>

/**
 * @brief Convert time_t to string.
 * @param time Time in time_t format.
 * @param format Format of the time.
 * @return Time in string format.
 */
std::string time_t_to_string(time_t time, const std::string &format = "%Y-%m-%d %H:%M:%S");

/**
 * @brief Convert string to time_t.
 * @param time Time in string format.
 * @param format Format of the time.
 * @return Time in time_t format.
 */
time_t string_to_time_t(const std::string &time, const std::string &format = "%Y-%m-%d %H:%M:%S");

/**
 * @brief Convert time_t to std::tm.
 * @param time Time in time_t format.
 * @return Time in std::tm format.
 */
std::tm time_t_to_tm(time_t time);

/**
 * @brief Convert std::tm to string.
 * @param tm Time in std::tm format.
 * @param format Format of the time.
 * @return Time in string format.
 */
std::tm string_to_tm(const std::string &time, const std::string &format);

#endif // DATE_HPP
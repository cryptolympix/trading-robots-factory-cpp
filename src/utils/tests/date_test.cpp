#include <gtest/gtest.h>
#include <ctime>
#include <string>
#include <sstream>
#include <iomanip>
#include "../date.hpp"

TEST(DateConversionTest, TimeTToString)
{
    // Define a time_t value representing a specific time (e.g., Jan 15, 2024, 00:30:00 AM)
    time_t time = 1705275000;
    // Define the expected string format
    std::string format = "%Y-%m-%d %H:%M:%S";

    // Call the function to convert time_t to string
    std::string result = time_t_to_string(time, format);

    // Define the expected result
    std::string expected = "2024-01-15 00:30:00";

    // Check if the result matches the expected value
    EXPECT_EQ(result, expected);
}

TEST(DateConversionTest, StringToTimeT)
{
    // Define a string representing a specific time (e.g., Jan 15, 2024, 00:30:00 AM)
    std::string timeString = "2024-01-15 00:30:00";
    // Define the expected string format
    std::string format = "%Y-%m-%d %H:%M:%S";

    // Call the function to convert string to time_t
    time_t result = string_to_time_t(timeString, format);

    // Define the expected result (time_t value for the specified time)
    time_t expected = 1705275000;

    // Check if the result matches the expected value
    EXPECT_EQ(result, expected);
}

TEST(DateConversionTest, TimeTToTm)
{
    // Define a time_t value representing a specific time (e.g., Jan 15, 2024, 00:30:00 AM)
    time_t time = 1705275000;

    // Call the function to convert time_t to std::tm
    std::tm result = time_t_to_tm(time);

    // Define the expected result (std::tm structure representing the specified time)
    std::tm expected = {};
    expected.tm_year = 2024 - 1900; // Years since 1900
    expected.tm_mon = 0;            // Months since January (0-11)
    expected.tm_mday = 15;          // Day of the month (1-31)
    expected.tm_hour = 0;           // Hours since midnight (0-23)
    expected.tm_min = 30;           // Minutes after the hour (0-59)
    expected.tm_sec = 0;            // Seconds after the minute (0-60)

    // Check if the result matches the expected value
    EXPECT_EQ(result.tm_year, expected.tm_year);
    EXPECT_EQ(result.tm_mon, expected.tm_mon);
    EXPECT_EQ(result.tm_mday, expected.tm_mday);
    EXPECT_EQ(result.tm_hour, expected.tm_hour);
    EXPECT_EQ(result.tm_min, expected.tm_min);
    EXPECT_EQ(result.tm_sec, expected.tm_sec);
}

TEST(DateConversionTest, StringToTm)
{
    // Define a string representing a specific time (e.g., Jan 15, 2024, 00:30:00 AM)
    std::string timeString = "2024-01-15 00:30:00";
    // Define the expected string format
    std::string format = "%Y-%m-%d %H:%M:%S";

    // Call the function to convert string to std::tm
    std::tm result = string_to_tm(timeString, format);

    // Define the expected result (std::tm structure representing the specified time)
    std::tm expected = {};
    expected.tm_year = 2024 - 1900; // Years since 1900
    expected.tm_mon = 0;            // Months since January (0-11)
    expected.tm_mday = 15;          // Day of the month (1-31)
    expected.tm_hour = 0;           // Hours since midnight (0-23)
    expected.tm_min = 30;           // Minutes after the hour (0-59)
    expected.tm_sec = 0;            // Seconds after the minute (0-60)

    // Check if the result matches the expected value
    EXPECT_EQ(result.tm_year, expected.tm_year);
    EXPECT_EQ(result.tm_mon, expected.tm_mon);
    EXPECT_EQ(result.tm_mday, expected.tm_mday);
    EXPECT_EQ(result.tm_hour, expected.tm_hour);
    EXPECT_EQ(result.tm_min, expected.tm_min);
    EXPECT_EQ(result.tm_sec, expected.tm_sec);
}

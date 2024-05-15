#include <gtest/gtest.h>
#include <ctime>
#include <string>
#include <sstream>
#include <iomanip>
#include "../date_conversion.hpp"

TEST(DateConversionTest, TimeTAndString)
{
    std::time_t currentTime = std::time(nullptr);
    std::string format = "%Y-%m-%d %H:%M:%S %Z"; // Format including time zone
    std::string currentTimeStr = time_t_to_string(currentTime, format);
    std::time_t convertedBackTime = string_to_time_t(currentTimeStr, format);
    ASSERT_LT(std::difftime(currentTime, convertedBackTime), 1);
}

TEST(DateConversionTest, TmAndString)
{
    std::tm customTime = {};
    customTime.tm_year = 2024 - 1900; // Year since 1900
    customTime.tm_mon = 4;            // May (0-based, so 4 means May)
    customTime.tm_mday = 15;          // 15th day
    customTime.tm_hour = 12;          // 12:00
    customTime.tm_min = 30;           // 30 minutes
    customTime.tm_sec = 45;           // 45 seconds
    customTime.tm_isdst = -1;         // Daylight Saving Time flag (let system determine)

    std::string format = "%Y-%m-%d %H:%M:%S %Z"; // Format including time zone
    std::string customTimeStr = tm_to_string(customTime, format);
    std::cout << "Custom time (tm to string): " << customTimeStr << std::endl;

    std::tm convertedBackTm = string_to_tm(customTimeStr, format);
    std::string convertedBackTmStr = tm_to_string(convertedBackTm, format);
    std::cout << "Converted back time (string to tm): " << convertedBackTmStr << std::endl;
    ASSERT_EQ(customTimeStr, convertedBackTmStr);
}

TEST(DateConversionTest, TmAndTimeT)
{
    std::tm customTime = {};
    customTime.tm_year = 2024 - 1900; // Year since 1900
    customTime.tm_mon = 4;            // May (0-based, so 4 means May)
    customTime.tm_mday = 15;          // 15th day
    customTime.tm_hour = 12;          // 12:00
    customTime.tm_min = 30;           // 30 minutes
    customTime.tm_sec = 45;           // 45 seconds
    customTime.tm_isdst = -1;         // Daylight Saving Time flag (let system determine)

    std::time_t customTimeTimeT = tm_to_time_t(customTime);
    std::tm convertedBackTm = time_t_to_tm(customTimeTimeT);
    ASSERT_EQ(customTime.tm_year, convertedBackTm.tm_year);
    ASSERT_EQ(customTime.tm_mon, convertedBackTm.tm_mon);
    ASSERT_EQ(customTime.tm_mday, convertedBackTm.tm_mday);
    ASSERT_EQ(customTime.tm_hour, convertedBackTm.tm_hour);
    ASSERT_EQ(customTime.tm_min, convertedBackTm.tm_min);
    ASSERT_EQ(customTime.tm_sec, convertedBackTm.tm_sec);
}

TEST(DateConversionTest, TimeZoneHandling)
{
    std::tm customTime = {};
    customTime.tm_year = 2024 - 1900; // Year since 1900
    customTime.tm_mon = 4;            // May (0-based, so 4 means May)
    customTime.tm_mday = 15;          // 15th day
    customTime.tm_hour = 12;          // 12:00
    customTime.tm_min = 30;           // 30 minutes
    customTime.tm_sec = 45;           // 45 seconds
    customTime.tm_isdst = -1;         // Daylight Saving Time flag (let system determine)

    customTime.tm_hour = 14; // 14:00 to test different times
    std::string timeZoneFormat = "%Y-%m-%d %H:%M:%S %Z";
    std::string customTimeWithTZStr = tm_to_string(customTime, timeZoneFormat);

    std::tm convertedBackTmWithTZ = string_to_tm(customTimeWithTZStr, timeZoneFormat);
    std::string convertedBackTmWithTZStr = tm_to_string(convertedBackTmWithTZ, timeZoneFormat);
    ASSERT_EQ(customTimeWithTZStr, convertedBackTmWithTZStr);
}

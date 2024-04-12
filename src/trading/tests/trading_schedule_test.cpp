#include <gtest/gtest.h>
#include <vector>
#include <ctime>
#include "../trading_schedule.hpp"
#include "../../types.hpp"

// Test fixture for trading schedule
class TradingScheduleTest : public ::testing::Test
{
protected:
    TradingSchedule schedule;

    void SetUp() override
    {
        std::vector<bool> working_day = {false,
                                         false,
                                         false,
                                         false,
                                         false,
                                         false,
                                         false,
                                         true,
                                         true,
                                         true,
                                         true,
                                         true,
                                         true,
                                         true,
                                         true,
                                         true,
                                         true,
                                         true,
                                         true,
                                         true,
                                         true,
                                         false,
                                         false,
                                         false};

        std::vector<bool> rest_day = {false,
                                      false,
                                      false,
                                      false,
                                      false,
                                      false,
                                      false,
                                      false,
                                      false,
                                      false,
                                      false,
                                      false,
                                      false,
                                      false,
                                      false,
                                      false,
                                      false,
                                      false,
                                      false,
                                      false,
                                      false,
                                      false,
                                      false,
                                      false};

        schedule.monday = working_day;
        schedule.tuesday = working_day;
        schedule.wednesday = working_day;
        schedule.thursday = working_day;
        schedule.friday = working_day;
        schedule.saturday = rest_day;
        schedule.sunday = rest_day;
    }
};

// Test case for is_on_trading_schedule function
TEST_F(TradingScheduleTest, TestIsOnTradingSchedule)
{
    std::tm date = {};

    // Cannot trade just before the opening time schedule
    date.tm_year = 2023 - 1900;
    date.tm_mon = 10;
    date.tm_mday = 6;
    date.tm_wday = 1;
    date.tm_hour = 6;
    date.tm_min = 0;
    date.tm_sec = 0;
    ASSERT_FALSE(is_on_trading_schedule(std::mktime(&date), schedule));

    // Can trade just after the opening time schedule
    date.tm_year = 2023 - 1900;
    date.tm_mon = 10;
    date.tm_mday = 6;
    date.tm_wday = 1;
    date.tm_hour = 7;
    date.tm_min = 0;
    date.tm_sec = 0;
    ASSERT_TRUE(is_on_trading_schedule(std::mktime(&date), schedule));

    // Can trade just before the closing time schedule
    date.tm_year = 2023 - 1900;
    date.tm_mon = 10;
    date.tm_mday = 6;
    date.tm_wday = 1;
    date.tm_hour = 20;
    date.tm_min = 0;
    date.tm_sec = 0;
    ASSERT_TRUE(is_on_trading_schedule(std::mktime(&date), schedule));

    // Cannot trade after the closing time schedule
    date.tm_year = 2023 - 1900;
    date.tm_mon = 10;
    date.tm_mday = 6;
    date.tm_wday = 1;
    date.tm_hour = 21;
    date.tm_min = 0;
    date.tm_sec = 0;
    ASSERT_FALSE(is_on_trading_schedule(std::mktime(&date), schedule));

    // Cannot trade on Saturday
    date.tm_year = 2023 - 1900;
    date.tm_mon = 10;
    date.tm_mday = 4;
    date.tm_wday = 6;
    date.tm_hour = 12;
    date.tm_min = 0;
    date.tm_sec = 0;
    ASSERT_FALSE(is_on_trading_schedule(std::mktime(&date), schedule));

    // Cannot trade on Sunday
    date.tm_year = 2023 - 1900;
    date.tm_mon = 10;
    date.tm_mday = 5;
    date.tm_wday = 0;
    date.tm_hour = 17;
    date.tm_min = 0;
    date.tm_sec = 0;
    ASSERT_FALSE(is_on_trading_schedule(std::mktime(&date), schedule));

    // Can trade on Friday during the schedule
    date.tm_year = 2023 - 1900;
    date.tm_mon = 10;
    date.tm_mday = 3;
    date.tm_wday = 5;
    date.tm_hour = 12;
    date.tm_min = 0;
    date.tm_sec = 0;
    ASSERT_TRUE(is_on_trading_schedule(std::mktime(&date), schedule));

    // Can trade on Thursday during the schedule
    date.tm_year = 2023 - 1900;
    date.tm_mon = 10;
    date.tm_mday = 2;
    date.tm_wday = 4;
    date.tm_hour = 17;
    date.tm_min = 0;
    date.tm_sec = 0;
    ASSERT_TRUE(is_on_trading_schedule(std::mktime(&date), schedule));

    // Can trade on Wednesday during the schedule
    date.tm_year = 2023 - 1900;
    date.tm_mon = 10;
    date.tm_mday = 1;
    date.tm_wday = 3;
    date.tm_hour = 9;
    date.tm_min = 0;
    date.tm_sec = 0;
    ASSERT_TRUE(is_on_trading_schedule(std::mktime(&date), schedule));
}
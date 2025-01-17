#include <gtest/gtest.h>
#include <vector>
#include <ctime>
#include "../schedule.hpp"
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

TEST_F(TradingScheduleTest, TestGetDaySchedule)
{
    ASSERT_EQ(get_day_schedule(0, schedule), schedule.sunday);
    ASSERT_EQ(get_day_schedule(1, schedule), schedule.monday);
    ASSERT_EQ(get_day_schedule(2, schedule), schedule.tuesday);
    ASSERT_EQ(get_day_schedule(3, schedule), schedule.wednesday);
    ASSERT_EQ(get_day_schedule(4, schedule), schedule.thursday);
    ASSERT_EQ(get_day_schedule(5, schedule), schedule.friday);
    ASSERT_EQ(get_day_schedule(6, schedule), schedule.saturday);
}

// Test case for is_on_trading_schedule function
TEST_F(TradingScheduleTest, TestIsOnTradingSchedule)
{
    std::tm date = {};

    for (int i = 0; i < 24; i++)
    {
        date.tm_year = 2023 - 1900;
        date.tm_mon = 10;
        date.tm_mday = 6;
        date.tm_wday = 1;
        date.tm_hour = i;
        date.tm_min = 0;
        date.tm_sec = 0;
        ASSERT_EQ(is_on_trading_schedule(std::mktime(&date), schedule), i >= 7 && i <= 20);
    }

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

TEST_F(TradingScheduleTest, TestHasSessionForDay)
{
    ASSERT_FALSE(has_session_for_day(0, schedule));
    ASSERT_TRUE(has_session_for_day(1, schedule));
    ASSERT_TRUE(has_session_for_day(2, schedule));
    ASSERT_TRUE(has_session_for_day(3, schedule));
    ASSERT_TRUE(has_session_for_day(4, schedule));
    ASSERT_TRUE(has_session_for_day(5, schedule));
    ASSERT_FALSE(has_session_for_day(6, schedule));
}
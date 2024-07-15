#include <gtest/gtest.h>
#include <vector>
#include <ctime>
#include <chrono>
#include <iostream>
#include "../builder.hpp"
#include "../../types.hpp"
#include "../time.hpp"

TEST(TimeIndicatorsTest, Hour)
{
    // Create an instance of the Hour class
    Hour hour;

    // Expected hour
    int expected_hour = 5;

    // Create test data
    tm datetime = {0};
    datetime.tm_year = 2023 - 1900;
    datetime.tm_mon = 0;
    datetime.tm_mday = 1;
    datetime.tm_hour = expected_hour;
    time_t date = mktime(&datetime);

    std::vector<Candle> test_candles = {
        {date, 1, 1, 1, 1, 0, 0, 0}};

    // Call the calculate method
    std::vector<double> result = hour.calculate(test_candles, false);

    // Validate the result
    ASSERT_EQ(result.back(), expected_hour);

    Hour *created_open_indicator = static_cast<Hour *>(create_indicator_from_id(hour.id, hour.params));
    std::vector<double> result_hour_indicator = created_open_indicator->calculate(test_candles);

    ASSERT_NE(created_open_indicator, nullptr);
    ASSERT_EQ(result_hour_indicator, result);
}

TEST(TimeIndicatorsTest, Minute)
{
    // Create an instance of the Minute class
    Minute minute;

    // Expected minute
    int expected_minute = 30;

    // Create test data
    tm datetime = {0};
    datetime.tm_year = 2023 - 1900;
    datetime.tm_mon = 0;
    datetime.tm_mday = 1;
    datetime.tm_hour = 0;
    datetime.tm_min = expected_minute;
    time_t date = mktime(&datetime);

    std::vector<Candle> test_candles = {
        {date, 1, 1, 1, 1, 0, 0, 0}};

    // Call the calculate method
    std::vector<double> result = minute.calculate(test_candles, false);

    // Validate the result
    ASSERT_EQ(result.back(), expected_minute);

    Minute *created_open_indicator = static_cast<Minute *>(create_indicator_from_id(minute.id, minute.params));
    std::vector<double> result_minute_indicator = created_open_indicator->calculate(test_candles);

    ASSERT_NE(created_open_indicator, nullptr);
    ASSERT_EQ(result_minute_indicator, result);
}

TEST(TimeIndicatorsTest, NFPWeek)
{
    // Create an instance of the NFPWeek class
    NFPWeek nfpWeek;
    NFPWeek *created_nfp_week = static_cast<NFPWeek *>(create_indicator_from_id(nfpWeek.id, nfpWeek.params));

    // NFP Week
    tm datetime_nfp = {0};
    datetime_nfp.tm_year = 2023 - 1900;
    datetime_nfp.tm_mon = 0;
    datetime_nfp.tm_mday = 6;
    time_t date_nfp = mktime(&datetime_nfp);

    std::vector<Candle> test_candles_nfp = {
        {date_nfp, 1, 1, 1, 1, 0, 0, 0}};

    // Call the calculate method
    std::vector<double> result_nfp = nfpWeek.calculate(test_candles_nfp, false);

    // Validate the result for NFP Week
    ASSERT_EQ(result_nfp.back(), 1);
    ASSERT_NE(created_nfp_week, nullptr);
    ASSERT_EQ(created_nfp_week->calculate(test_candles_nfp), result_nfp);

    // Not NFP Week
    tm datetime_not_nfp = {0};
    datetime_not_nfp.tm_year = 2023 - 1900;
    datetime_not_nfp.tm_mon = 0;
    datetime_not_nfp.tm_mday = 13;
    time_t date_not_nfp = mktime(&datetime_not_nfp);

    std::vector<Candle> test_candles_not_nfp = {
        {date_not_nfp, 1, 1, 1, 1, 0, 0, 0}};

    // Call the calculate method
    std::vector<double> result_not_nfp = nfpWeek.calculate(test_candles_not_nfp, false);

    // Validate the result for Not NFP Week
    ASSERT_EQ(result_not_nfp.back(), 0);
    ASSERT_NE(created_nfp_week, nullptr);
    ASSERT_EQ(created_nfp_week->calculate(test_candles_not_nfp), result_not_nfp);
}

TEST(TimeIndicatorsTest, MarketSession)
{
    // London
    MarketSession london("london");
    MarketSession *created_london = static_cast<MarketSession *>(create_indicator_from_id(london.id, london.params));
    std::vector<Candle> test_candles_london;
    for (int h = 7; h < 14; ++h)
    {
        tm datetime = {0};
        datetime.tm_year = 2023 - 1900;
        datetime.tm_mon = 0;
        datetime.tm_mday = 1;
        datetime.tm_hour = h;
        time_t date = mktime(&datetime);
        test_candles_london.push_back({date, 1, 1, 1, 1, 0, 0, 0});
    }
    // Call the calculate method
    std::vector<double> result_london = london.calculate(test_candles_london, false);

    // Validate the result for London
    ASSERT_EQ(result_london, std::vector<double>({0, 1, 1, 1, 1, 1, 0}));
    ASSERT_NE(created_london, nullptr);
    ASSERT_EQ(created_london->calculate(test_candles_london), result_london);

    // New York
    MarketSession newYork("new-york");
    MarketSession *created_new_york = static_cast<MarketSession *>(create_indicator_from_id(newYork.id, newYork.params));
    std::vector<Candle> test_candles_newyork;
    for (int h = 13; h < 22; ++h)
    {
        tm datetime = {0};
        datetime.tm_year = 2023 - 1900;
        datetime.tm_mon = 0;
        datetime.tm_mday = 1;
        datetime.tm_hour = h;
        time_t date = mktime(&datetime);
        test_candles_newyork.push_back({date, 1, 1, 1, 1, 0, 0, 0});
    }
    // Call the calculate method
    std::vector<double> result_newyork = newYork.calculate(test_candles_newyork, false);

    // Validate the result for New York
    ASSERT_EQ(result_newyork, std::vector<double>({0, 1, 1, 1, 1, 1, 1, 1, 0}));
    ASSERT_NE(created_new_york, nullptr);
    ASSERT_EQ(created_new_york->calculate(test_candles_newyork), result_newyork);

    // Tokyo
    MarketSession tokyo("tokyo");
    MarketSession *created_tokyo = static_cast<MarketSession *>(create_indicator_from_id(tokyo.id, tokyo.params));
    std::vector<Candle> test_candles_tokyo;
    for (int h = 1; h < 10; ++h)
    {
        tm datetime = {0};
        datetime.tm_year = 2023 - 1900;
        datetime.tm_mon = 0;
        datetime.tm_mday = 1;
        datetime.tm_hour = h;
        time_t date = mktime(&datetime);
        test_candles_tokyo.push_back({date, 1, 1, 1, 1, 0, 0, 0});
    }
    // Call the calculate method
    std::vector<double> result_tokyo = tokyo.calculate(test_candles_tokyo, false);

    // Validate the result for Tokyo
    ASSERT_EQ(result_tokyo, std::vector<double>({0, 1, 1, 1, 1, 1, 1, 1, 0}));
    ASSERT_NE(created_tokyo, nullptr);
    ASSERT_EQ(created_tokyo->calculate(test_candles_tokyo), result_tokyo);
}

TEST(TimeIndicatorsTest, WeekDay)
{
    // Create an instance of the WeekDay class
    WeekDay week_day_sunday("sunday");
    WeekDay week_day_monday("monday");
    WeekDay week_day_tuesday("tuesday");
    WeekDay week_day_wednesday("wednesday");
    WeekDay week_day_thursday("thursday");
    WeekDay week_day_friday("friday");
    WeekDay week_day_saturday("saturday");

    WeekDay *created_sunday = static_cast<WeekDay *>(create_indicator_from_id(week_day_sunday.id, week_day_sunday.params));
    WeekDay *created_monday = static_cast<WeekDay *>(create_indicator_from_id(week_day_monday.id, week_day_monday.params));
    WeekDay *created_tuesday = static_cast<WeekDay *>(create_indicator_from_id(week_day_tuesday.id, week_day_tuesday.params));
    WeekDay *created_wednesday = static_cast<WeekDay *>(create_indicator_from_id(week_day_wednesday.id, week_day_wednesday.params));
    WeekDay *created_thursday = static_cast<WeekDay *>(create_indicator_from_id(week_day_thursday.id, week_day_thursday.params));
    WeekDay *created_friday = static_cast<WeekDay *>(create_indicator_from_id(week_day_friday.id, week_day_friday.params));
    WeekDay *created_saturday = static_cast<WeekDay *>(create_indicator_from_id(week_day_saturday.id, week_day_saturday.params));

    // Create test data for the days of the week
    std::vector<Candle> test_candles_weekday;
    for (int d = 1; d <= 7; ++d)
    {
        tm datetime = {0};
        datetime.tm_year = 2023 - 1900;
        datetime.tm_mon = 0;
        datetime.tm_mday = d;
        datetime.tm_hour = 0;
        time_t date = mktime(&datetime);
        test_candles_weekday.push_back({date, 1, 1, 1, 1, 0, 0, 0});
    }

    // Call the calculate method
    std::vector<double> result_sunday = week_day_sunday.calculate(test_candles_weekday, false);
    std::vector<double> result_monday = week_day_monday.calculate(test_candles_weekday, false);
    std::vector<double> result_tuesday = week_day_tuesday.calculate(test_candles_weekday, false);
    std::vector<double> result_wednesday = week_day_wednesday.calculate(test_candles_weekday, false);
    std::vector<double> result_thursday = week_day_thursday.calculate(test_candles_weekday, false);
    std::vector<double> result_friday = week_day_friday.calculate(test_candles_weekday, false);
    std::vector<double> result_saturday = week_day_saturday.calculate(test_candles_weekday, false);

    // Validate the result for WeekDay
    ASSERT_EQ(result_sunday, std::vector<double>({1, 0, 0, 0, 0, 0, 0}));
    ASSERT_EQ(result_monday, std::vector<double>({0, 1, 0, 0, 0, 0, 0}));
    ASSERT_EQ(result_tuesday, std::vector<double>({0, 0, 1, 0, 0, 0, 0}));
    ASSERT_EQ(result_wednesday, std::vector<double>({0, 0, 0, 1, 0, 0, 0}));
    ASSERT_EQ(result_thursday, std::vector<double>({0, 0, 0, 0, 1, 0, 0}));
    ASSERT_EQ(result_friday, std::vector<double>({0, 0, 0, 0, 0, 1, 0}));
    ASSERT_EQ(result_saturday, std::vector<double>({0, 0, 0, 0, 0, 0, 1}));

    ASSERT_NE(created_sunday, nullptr);
    ASSERT_EQ(created_sunday->calculate(test_candles_weekday), result_sunday);
    ASSERT_NE(created_monday, nullptr);
    ASSERT_EQ(created_monday->calculate(test_candles_weekday), result_monday);
    ASSERT_NE(created_tuesday, nullptr);
    ASSERT_EQ(created_tuesday->calculate(test_candles_weekday), result_tuesday);
    ASSERT_NE(created_wednesday, nullptr);
    ASSERT_EQ(created_wednesday->calculate(test_candles_weekday), result_wednesday);
    ASSERT_NE(created_thursday, nullptr);
    ASSERT_EQ(created_thursday->calculate(test_candles_weekday), result_thursday);
    ASSERT_NE(created_friday, nullptr);
    ASSERT_EQ(created_friday->calculate(test_candles_weekday), result_friday);
    ASSERT_NE(created_saturday, nullptr);
    ASSERT_EQ(created_saturday->calculate(test_candles_weekday), result_saturday);
}

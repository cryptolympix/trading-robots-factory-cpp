#include <gtest/gtest.h>
#include <vector>
#include <ctime>
#include <chrono>
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
}

TEST(TimeIndicatorsTest, NFPWeek)
{
    // Create an instance of the NFPWeek class
    NFPWeek nfpWeek;

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
}

TEST(TimeIndicatorsTest, MarketSession)
{
    // London
    MarketSession london(MarketSessionZone::LONDON);
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

    // New York
    MarketSession newYork(MarketSessionZone::NEW_YORK);
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

    // Tokyo
    MarketSession tokyo(MarketSessionZone::TOKYO);
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
}

TEST(TimeIndicatorsTest, WeekDay)
{
    // Create an instance of the WeekDay class
    WeekDay weekDay;

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
    std::vector<double> result_weekday = weekDay.calculate(test_candles_weekday, false);

    // Validate the result for WeekDay
    ASSERT_EQ(result_weekday, std::vector<double>({0, 1, 2, 3, 4, 5, 6}));
}

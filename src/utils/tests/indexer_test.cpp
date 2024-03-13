#include <gtest/gtest.h>
#include "../indexer.hpp"
#include "../time_frame.hpp"
#include "../../types.hpp"
#include <chrono>

class TestIndexer : public ::testing::Test
{
protected:
    Indexer *indexer;
    time_t initial_date;
    int window;

    void SetUp() override
    {
        // 2023-01-01 01:00:00
        std::tm date = {
            .tm_year = 2023 - 1900,
            .tm_mon = 1,
            .tm_mday = 1,
            .tm_hour = 1,
            .tm_min = 0,
            .tm_sec = 0};

        initial_date = std::mktime(&date);
        window = 2;

        // Mock data for candles
        CandlesData candles;
        for (int i = 0; i < 5; ++i)
        {
            candles[TimeFrame::H1].push_back(Candle{initial_date + i * 3600, 1.0, 2.0, 0.5, 1.5});
        }

        indexer = new Indexer(candles, window);
    }

    void TearDown() override
    {
        delete indexer;
    }
};

TEST_F(TestIndexer, TestIncrementIndexes)
{
    std::pair<int, int> before_indexes = indexer->get_indexes(TimeFrame::H1);

    // Mock date for update
    std::time_t mock_date = initial_date + 3600; // Equivalent to 2023-01-01 02:00:00
    std::chrono::system_clock::time_point new_date = std::chrono::system_clock::from_time_t(mock_date);

    // Call the function to update indexes
    indexer->update_indexes(new_date);

    // Get the indexes for H1 timeframe and check if they are updated correctly
    std::pair<int, int> after_indexes = indexer->get_indexes(TimeFrame::H1);
    EXPECT_EQ(after_indexes.first, before_indexes.first);
    EXPECT_EQ(after_indexes.second, before_indexes.second + 1);
}

TEST_F(TestIndexer, TestIndexesRespectWindow)
{
    // Mock date for update
    std::time_t mock_date = initial_date + window * 3600; // Equivalent to 2023-01-01 02:00:00
    std::chrono::system_clock::time_point new_date = std::chrono::system_clock::from_time_t(mock_date);

    // Call the function to update indexes
    indexer->update_indexes(new_date);

    // Get the indexes for H1 timeframe and check if they are updated correctly
    std::pair<int, int> indexes = indexer->get_indexes(TimeFrame::H1);
    EXPECT_EQ(indexes.second - indexes.first, window);
}

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

TEST_F(TestIndexer, TestIndexes)
{
    std::pair<int, int> indexes = indexer->get_indexes(TimeFrame::H1);

    // Mock date for update
    std::time_t mock_date = initial_date;

    for (int i = 0; i < 5; ++i)
    {
        // New date to update indexes
        time_t new_date = mock_date + i * get_time_frame_value(TimeFrame::H1) * 60;

        // Call the function to update indexes
        indexer->update_indexes(new_date);

        // Get the indexes for H1 timeframe and check if they are updated correctly
        int start_index = std::get<0>(indexer->get_indexes(TimeFrame::H1));
        int end_index = std::get<1>(indexer->get_indexes(TimeFrame::H1));

        EXPECT_EQ(start_index, std::max(0, indexes.second - window + i));
        EXPECT_EQ(end_index, indexes.second + i);

        if (end_index >= window)
        {
            EXPECT_EQ(end_index - start_index, window);
        }
        else
        {
            EXPECT_EQ(end_index - start_index, end_index);
        }
    }
}

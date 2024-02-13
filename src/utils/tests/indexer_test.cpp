#include <gtest/gtest.h>
#include "../indexer.hpp"
#include "../time_frame.hpp"
#include "../../types.hpp"

class TestIndexer : public ::testing::Test
{
protected:
    Indexer *indexer;
    time_t initial_date;
    int window;

    void SetUp() override
    {
        initial_date = 1672531200; // Equivalent to 2023-01-01 01:00:00
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
    // Mock date for update
    std::time_t mock_date = initial_date + 3600; // Equivalent to 2023-01-01 02:00:00

    // Call the function to update indexes
    indexer->update_indexes(mock_date);

    // Get the indexes for H1 timeframe and check if they are updated correctly
    auto indexes = indexer->get_indexes(TimeFrame::H1);
    EXPECT_EQ(indexes.first, 0);
    EXPECT_EQ(indexes.second, 1);
}

TEST_F(TestIndexer, TestIndexesRespectWindow)
{
    // Mock date for update
    std::time_t mock_date = initial_date + window * 3600;

    // Call the function to update indexes
    indexer->update_indexes(mock_date);

    // Get the indexes for H1 timeframe and check if they are updated correctly
    auto indexes = indexer->get_indexes(TimeFrame::H1);
    EXPECT_EQ(indexes.first, 0);
    EXPECT_EQ(indexes.second, window);
}

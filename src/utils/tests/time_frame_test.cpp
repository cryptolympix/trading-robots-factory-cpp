#include <gtest/gtest.h>
#include "../time_frame.hpp"

// Test fixture class
class TimeFrameTest : public ::testing::Test
{
protected:
    // SetUp function is called before each test
    void SetUp() override
    {
        // Set up code, if any
    }

    // TearDown function is called after each test
    void TearDown() override
    {
        // Tear down code, if any
    }
};

// Test case for get_time_frame_value function
TEST_F(TimeFrameTest, GetTimeFrameValue)
{
    // Test each time frame and check if the returned value is correct
    EXPECT_EQ(get_time_frame_value(TimeFrame::M1), 1);
    EXPECT_EQ(get_time_frame_value(TimeFrame::M5), 5);
    EXPECT_EQ(get_time_frame_value(TimeFrame::M15), 15);
    EXPECT_EQ(get_time_frame_value(TimeFrame::M30), 30);
    EXPECT_EQ(get_time_frame_value(TimeFrame::H1), 60);
    EXPECT_EQ(get_time_frame_value(TimeFrame::H4), 60 * 4);
    EXPECT_EQ(get_time_frame_value(TimeFrame::H12), 60 * 12);
    EXPECT_EQ(get_time_frame_value(TimeFrame::D1), 60 * 24);
    EXPECT_THROW(get_time_frame_value(static_cast<TimeFrame>(-1)), std::runtime_error);
}

// Test case for match_time_frame function
TEST_F(TimeFrameTest, MatchTimeFrame)
{
    // Test TimeFrame::M5
    for (int i = 0; i < 24; ++i)
    {
        std::string hour = (i < 10) ? "0" + std::to_string(i) : std::to_string(i);
        for (int j = 0; j < 45; j += 5)
        {
            std::string minutes = (j < 10) ? "0" + std::to_string(j) : std::to_string(j);
            ASSERT_TRUE(match_time_frame(hour + ":" + minutes + ":00", TimeFrame::M5));
        }
    }
    ASSERT_FALSE(match_time_frame("04:15:10", TimeFrame::M5));
    ASSERT_FALSE(match_time_frame("14:58:00", TimeFrame::M5));

    // Test TimeFrame::M15
    for (int i = 0; i < 24; ++i)
    {
        std::string hour = (i < 10) ? "0" + std::to_string(i) : std::to_string(i);
        for (int j = 0; j < 45; j += 15)
        {
            std::string minutes = (j < 10) ? "0" + std::to_string(j) : std::to_string(j);
            ASSERT_TRUE(match_time_frame(hour + ":" + minutes + ":00", TimeFrame::M15));
        }
    }
    ASSERT_FALSE(match_time_frame("02:10:00", TimeFrame::M15));
    ASSERT_FALSE(match_time_frame("23:42:00", TimeFrame::M15));

    // Test TimeFrame::M30
    for (int i = 0; i < 24; ++i)
    {
        std::string hour = (i < 10) ? "0" + std::to_string(i) : std::to_string(i);
        ASSERT_TRUE(match_time_frame(hour + ":00:00", TimeFrame::M30));
        ASSERT_TRUE(match_time_frame(hour + ":30:00", TimeFrame::M30));
    }
    ASSERT_FALSE(match_time_frame("08:30:10", TimeFrame::M30));
    ASSERT_FALSE(match_time_frame("08:12:00", TimeFrame::M30));

    // Test TimeFrame::H1
    for (int i = 0; i < 24; ++i)
    {
        std::string hour = (i < 10) ? "0" + std::to_string(i) : std::to_string(i);
        ASSERT_TRUE(match_time_frame(hour + ":00:00", TimeFrame::H1));
    }
    ASSERT_FALSE(match_time_frame("08:30:00", TimeFrame::H1));
    ASSERT_FALSE(match_time_frame("15:15:15", TimeFrame::H1));

    // Test TimeFrame::H4
    for (int i = 0; i < 24; i += 4)
    {
        std::string hour = (i < 10) ? "0" + std::to_string(i) : std::to_string(i);
        ASSERT_TRUE(match_time_frame(hour + ":00:00", TimeFrame::H4));
    }
    ASSERT_FALSE(match_time_frame("02:00:00", TimeFrame::H4));
    ASSERT_FALSE(match_time_frame("10:30:00", TimeFrame::H4));

    // Test TimeFrame::H12
    for (int i = 0; i < 24; i += 12)
    {
        std::string hour = (i < 10) ? "0" + std::to_string(i) : std::to_string(i);
        ASSERT_TRUE(match_time_frame(hour + ":00:00", TimeFrame::H12));
    }
    ASSERT_FALSE(match_time_frame("02:00:00", TimeFrame::H12));
    ASSERT_FALSE(match_time_frame("10:30:00", TimeFrame::H12));
    ASSERT_TRUE(match_time_frame("00:00:00", TimeFrame::D1));
}

// Test case for highest_time_frame function
TEST_F(TimeFrameTest, HighestTimeFrame)
{
    // Test with an empty vector (should throw runtime_error)
    EXPECT_THROW(highest_time_frame({}), std::runtime_error);

    // Test with a single time frame (should return the same time frame)
    EXPECT_EQ(highest_time_frame({TimeFrame::H1}), TimeFrame::H1);

    // Test with multiple time frames
    EXPECT_EQ(highest_time_frame({TimeFrame::M1, TimeFrame::H4, TimeFrame::M15}), TimeFrame::H4);
    EXPECT_EQ(highest_time_frame({TimeFrame::M30, TimeFrame::H1, TimeFrame::M5}), TimeFrame::H1);
    EXPECT_EQ(highest_time_frame({TimeFrame::D1, TimeFrame::M15, TimeFrame::M30}), TimeFrame::D1);
}

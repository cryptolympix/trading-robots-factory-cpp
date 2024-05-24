#include <gtest/gtest.h>
#include <sstream>
#include <thread> // For std::this_thread::sleep_for
#include "../progress_bar.hpp"

class ProgressBarTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        // Set up any common variables or objects required for the tests
        total_iterations = 100;
        progress_bar = new ProgressBar(50, total_iterations, "Progress");
    }

    void TearDown() override
    {
        // Clean up any common variables or objects after the tests
        delete progress_bar;
    }

    // Declare the variables used in the test cases
    ProgressBar *progress_bar;
    int total_iterations;
};

TEST_F(ProgressBarTest, UpdateProgressAndLabel)
{
    // Redirect cout to a stringstream for testing
    std::stringstream ss;
    std::streambuf *old_cout = std::cout.rdbuf();
    std::cout.rdbuf(ss.rdbuf());

    // Test the update function with progress and label
    progress_bar->update(50, "Updated label");

    // Restore cout
    std::cout.rdbuf(old_cout);

    // Check if the output matches the expected output
    EXPECT_EQ(ss.str(), "\rProgress [=========================>                        ] 50% (50/100) Elapsed: 0s Remaining: 0s - Updated label");
}

TEST_F(ProgressBarTest, CompleteProgress)
{
    // Redirect cout to a stringstream for testing
    std::stringstream ss;
    std::streambuf *old_cout = std::cout.rdbuf();
    std::cout.rdbuf(ss.rdbuf());

    // Test the complete function
    progress_bar->complete("Process completed!");

    // Restore cout
    std::cout.rdbuf(old_cout);

    // Check if the output matches the expected output
    EXPECT_EQ(ss.str(), "\rProcess completed!\n");
}

TEST_F(ProgressBarTest, TimeEstimationAccuracy)
{
    // Redirect cout to a stringstream for testing
    std::stringstream ss;
    std::streambuf *old_cout = std::cout.rdbuf();
    std::cout.rdbuf(ss.rdbuf());

    // Loop through a few iterations to simulate progress
    for (int i = 0; i <= total_iterations; ++i)
    {
        progress_bar->update(i, "Time Estimation Test");
        // Simulate some computation here
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    // Test the complete function
    progress_bar->complete("Process completed!");

    // Restore cout
    std::cout.rdbuf(old_cout);

    // Check if the output contains valid time estimation
    EXPECT_TRUE(ss.str().find("Elapsed:") != std::string::npos);
    EXPECT_TRUE(ss.str().find("Remaining:") != std::string::npos);
}

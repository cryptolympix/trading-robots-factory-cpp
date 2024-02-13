#include <gtest/gtest.h>
#include "../logger.hpp"

// Fixture for Logger tests
class LoggerTest : public ::testing::Test
{
protected:
    Logger *logger;

    void SetUp() override
    {
        // Initialize logger with a temporary log file
        logger = new Logger("TestLogger", "test_log.txt");
    }

    void TearDown() override
    {
        // Close the logger
        logger->close();
        delete logger;

        // Delete the log file
        std::remove("test_log.txt");
    }
};

// Test case for logging informational messages
TEST_F(LoggerTest, TestInfoLogging)
{
    // Log an informational message
    logger->info("This is an informational message");

    // Assert that the log file contains the expected message
    std::ifstream log_file("test_log.txt");
    std::string line;
    bool message_found = false;
    while (std::getline(log_file, line))
    {
        if (line.find("INFO: This is an informational message") != std::string::npos)
        {
            message_found = true;
            break;
        }
    }
    log_file.close();
    ASSERT_TRUE(message_found);
}

// Test case for logging warning messages
TEST_F(LoggerTest, TestWarningLogging)
{
    // Log a warning message
    logger->warning("This is a warning message");

    // Assert that the log file contains the expected message
    std::ifstream log_file("test_log.txt");
    std::string line;
    bool message_found = false;
    while (std::getline(log_file, line))
    {
        if (line.find("WARNING: This is a warning message") != std::string::npos)
        {
            message_found = true;
            break;
        }
    }
    log_file.close();
    ASSERT_TRUE(message_found);
}

// Test case for logging error messages
TEST_F(LoggerTest, TestErrorLogging)
{
    // Log an error message
    logger->error("This is an error message");

    // Assert that the log file contains the expected message
    std::ifstream log_file("test_log.txt");
    std::string line;
    bool message_found = false;
    while (std::getline(log_file, line))
    {
        if (line.find("ERROR: This is an error message") != std::string::npos)
        {
            message_found = true;
            break;
        }
    }
    log_file.close();
    ASSERT_TRUE(message_found);
}

// Test case for closing the logger
TEST_F(LoggerTest, TestLoggerClose)
{
    // Close the logger
    logger->close();

    // Assert that the file handler is closed
    ASSERT_FALSE(logger->file_handler.is_open());
}

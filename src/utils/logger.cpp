#include <string>
#include <iostream>
#include <fstream>
#include "logger.hpp"

/**
 * @brief Initializes the Logger.
 *
 * @param log_file The relative or absolute path to the log file (default is 'logs/log.txt').
 */
Logger::Logger(const std::string &log_file) : file_path(std::filesystem::absolute(log_file))
{
    // Check if the directory exists
    std::filesystem::path log_dir = std::filesystem::path(file_path).parent_path();
    if (!std::filesystem::exists(log_dir))
    {
        try
        {
            std::filesystem::create_directories(log_dir);
        }
        catch (const std::filesystem::filesystem_error &e)
        {
            std::cerr << "Error creating directories: " << e.what() << std::endl;
        }
    }

    // Open the log file
    file_handler.open(file_path, std::ios::out | std::ios::app);
    if (!file_handler.is_open())
    {
        std::cerr << "Failed to open log file: " << file_path << std::endl;
    }
}

/**
 * @brief Logs an informational message.
 *
 * @param message The informational message to be logged.
 */
void Logger::info(const std::string &message)
{
    file_handler << "INFO: " << message << std::endl;
}

/**
 * @brief Logs a warning message.
 *
 * @param message The warning message to be logged.
 */
void Logger::warning(const std::string &message)
{
    file_handler << "WARNING: " << message << std::endl;
}

/**
 * @brief Logs an error message.
 *
 * @param message The error message to be logged.
 */
void Logger::error(const std::string &message)
{
    file_handler << "ERROR: " << message << std::endl;
}

/**
 * @brief Closes the file handler to release the file descriptor.
 */
void Logger::close()
{
    file_handler.close();
}

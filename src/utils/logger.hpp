#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>

/**
 * @brief The Logger class provides a simple interface for logging messages to a file.
 */
class Logger
{
public:
    std::string file_path;      // The absolute path to the log file.
    std::ofstream file_handler; // The file handler for logging.

    /**
     * @brief Default constructor for the Logger class.
     */
    Logger();

    /**
     * @brief Initializes the Logger.
     *
     * @param log_file The relative or absolute path to the log file (default is 'logs/log.txt').
     */
    Logger(const std::string &log_file = "logs/log.txt");

    /**
     * @brief Logs an informational message.
     *
     * @param message The informational message to be logged.
     */
    void info(const std::string &message);

    /**
     * @brief Logs a warning message.
     *
     * @param message The warning message to be logged.
     */
    void warning(const std::string &message);

    /**
     * @brief Logs an error message.
     *
     * @param message The error message to be logged.
     */
    void error(const std::string &message);

    /**
     * @brief Closes the file handler to release the file descriptor.
     */
    void close();
};

#endif /* LOGGER_H */

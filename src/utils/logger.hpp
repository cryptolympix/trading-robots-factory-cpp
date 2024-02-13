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
    std::string name;           /**< The name of the logger. */
    std::string file_path;      /**< The absolute path to the log file. */
    int level;                  /**< The logging level. */
    std::ofstream file_handler; /**< The file handler for logging. */

    /**
     * @brief Initializes the Logger.
     *
     * @param name The name of the logger.
     * @param log_file The relative or absolute path to the log file (default is 'logs/log.txt').
     * @param level The logging level (default is logging.DEBUG).
     */
    Logger(const std::string &name, const std::string &log_file = "logs/log.txt", int level = 20);

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

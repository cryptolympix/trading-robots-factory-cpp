#include <random>
#include <string>
#include <sstream>
#include <ctime>
#include <chrono>
#include <iomanip>
#include "uid.hpp"

/**
 * @brief Generate a unique ID of the specified size.
 *
 * @param size The size of the generated ID.
 * @return std::string The generated unique ID.
 */
std::string generate_uid(int size)
{
    const std::string characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, characters.size() - 1);

    std::string uid;
    for (int i = 0; i < size; ++i)
    {
        uid += characters[dis(gen)];
    }
    return uid;
}

/**
 * @brief Generate a unique ID based on the current date and time.
 *
 * @return std::string The generated unique ID.
 */
std::string generate_date_uid()
{
    auto now = std::chrono::system_clock::now();
    time_t date = std::chrono::system_clock::to_time_t(now);
    std::stringstream date_string;
    date_string << std::put_time(std::localtime(&date), "%Y%m%d%H%M%S");
    return date_string.str();
}
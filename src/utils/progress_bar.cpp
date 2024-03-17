#include <iostream>
#include <string>
#include <chrono>
#include "progress_bar.hpp"

/**
 * @brief Constructs a progress bar.
 * @param width The width of the progress bar.
 * @param total_iterations The total number of iterations.
 * @param label The label for the progress bar.
 */
ProgressBar::ProgressBar(int width, int total_iterations, std::string label)
    : width(width), total_iterations(total_iterations), label(label) {}

/**
 * @brief Updates the progress bar with the iteration count, and optional label update.
 * @param current_iteration The current iteration count.
 * @param new_label Optional new label to display.
 */
void ProgressBar::update(int current_iteration, std::string new_label)
{
    float progress = static_cast<float>(current_iteration) / total_iterations;
    int pos = static_cast<int>(width * progress);
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_time = now - start_time;
    double estimated_time_remaining = (elapsed_time.count() / progress) * (1 - progress);

    std::cout << "\r";
    std::cout << label << " [";
    for (int i = 0; i < width; ++i)
    {
        if (i < pos)
            std::cout << "=";
        else if (i == pos)
            std::cout << ">";
        else
            std::cout << " ";
    }
    std::cout << "] " << int(progress * 100.0) << "% (" << current_iteration << "/" << total_iterations << ")"
              << " Elapsed: " << format_time(elapsed_time.count())
              << " Remaining: " << format_time(estimated_time_remaining);
    if (!new_label.empty())
    {
        std::cout << " - " << new_label;
    }
    std::cout.flush();
}

/**
 * @brief Completes the progress bar display with an optional completion message.
 * @param message Optional message to display upon completion.
 */
void ProgressBar::complete(std::string message)
{
    std::cout << "\r";
    if (!message.empty())
    {
        std::cout << message << std::endl;
    }
    else
    {
        std::cout << std::endl;
    }
}

/**
 * @brief Formats a time duration in seconds to a human-readable format.
 * @param seconds The time duration in seconds.
 * @return The formatted time string.
 */
std::string ProgressBar::format_time(double seconds)
{
    int hours = static_cast<int>(seconds / 3600);
    seconds -= hours * 3600;
    int minutes = static_cast<int>(seconds / 60);
    seconds -= minutes * 60;

    std::string formatted_time;
    if (hours > 0)
        formatted_time += std::to_string(hours) + "h ";
    if (minutes > 0)
        formatted_time += std::to_string(minutes) + "m ";
    formatted_time += std::to_string(static_cast<int>(seconds)) + "s";
    return formatted_time;
}

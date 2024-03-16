#ifndef PROGRESS_BAR_HPP
#define PROGRESS_BAR_HPP

#include <iostream>
#include <string>

/**
 * @brief A simple class to display a text-based progress bar with iteration count and label.
 */
class ProgressBar
{
public:
    /**
     * @brief Constructs a progress bar.
     * @param width The width of the progress bar.
     * @param total_iterations The total number of iterations.
     * @param label The label for the progress bar.
     */
    ProgressBar(int width, int total_iterations, std::string label);

    /**
     * @brief Updates the progress bar with current progress, iteration count, and optional label update.
     * @param progress The current progress value (between 0.0 and 1.0).
     * @param current_iteration The current iteration count.
     * @param new_label Optional new label to display.
     */
    void update(float progress, int current_iteration, std::string new_label = "");

    /**
     * @brief Completes the progress bar display with an optional completion message.
     * @param message Optional message to display upon completion.
     */
    void complete(std::string message = "");

private:
    int width;                                                                           // The width of the progress bar.
    int total_iterations;                                                                // The total number of iterations.
    std::string label;                                                                   // The label for the progress bar.
    std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now(); // The start time of the progress bar.

    /**
     * @brief Formats a time duration in seconds to a human-readable format.
     * @param seconds The time duration in seconds.
     * @return The formatted time string.
     */
    std::string format_time(double seconds);
};

#endif // PROGRESS_BAR_HPP

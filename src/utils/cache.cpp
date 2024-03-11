#include <iostream>
#include <fstream>
#include <unordered_map>
#include <stdexcept>
#include "cache.hpp"

/**
 * @brief Cache a dictionary to a file.
 *
 * @tparam T Type of dictionary values.
 * @param data Dictionary to be cached.
 * @param file_path File path where the dictionary will be cached.
 */
template <typename T>
void cache_dictionary(const std::unordered_map<std::string, T> &data, const std::string &file_path)
{
    // Check if the directory exists, create it if it doesn't
    std::size_t found = file_path.find_last_of("/\\");
    if (found != std::string::npos)
    {
        std::string dir = file_path.substr(0, found);
        system(("mkdir -p " + dir).c_str());
    }

    // Write dictionary to file
    std::ofstream file(file_path, std::ios::binary);
    if (file.is_open())
    {
        for (const auto &pair : data)
        {
            file << pair.first << ' ' << pair.second << '\n';
        }
        file.close();
    }
    else
    {
        throw std::runtime_error("Unable to open file for writing: " + file_path);
    }
}

/**
 * @brief Load a cached dictionary from a file.
 *
 * @tparam T Type of dictionary values.
 * @param file_path File path from which to load the dictionary.
 * @return std::unordered_map<std::string, T> Loaded dictionary.
 */
template <typename T>
std::unordered_map<std::string, T> load_cached_dictionary(const std::string &file_path)
{
    std::unordered_map<std::string, T> data;

    try
    {
        // Check if the directory exists, create it if it doesn't
        std::size_t found = file_path.find_last_of("/\\");
        if (found != std::string::npos)
        {
            std::string dir = file_path.substr(0, found);
            system(("mkdir -p " + dir).c_str());
        }

        // Read dictionary from file
        std::ifstream file(file_path, std::ios::binary);
        if (file.is_open())
        {
            std::string key;
            T value;
            while (file >> key >> value)
            {
                data[key] = value;
            }
            file.close();
        }
        else
        {
            throw std::runtime_error("Unable to open file for reading: " + file_path);
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "An error occurred while loading the file " << file_path << ": " << e.what() << std::endl;
    }

    return data;
}

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <unordered_map>
#include <string>
#include "../types.hpp"
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
            // Write key size and key
            std::size_t key_size = pair.first.size();
            file.write(reinterpret_cast<const char *>(&key_size), sizeof(key_size));
            file.write(pair.first.c_str(), key_size);

            // Write value
            file.write(reinterpret_cast<const char *>(&pair.second), sizeof(T));
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
        std::ifstream file(file_path, std::ios::binary);
        if (file.is_open())
        {
            while (!file.eof())
            {
                // Read key size
                std::size_t key_size;
                file.read(reinterpret_cast<char *>(&key_size), sizeof(key_size));
                if (file.eof()) // Check if reached end of file after attempting to read key size
                    break;

                // Read key
                std::string key;
                key.resize(key_size);
                file.read(&key[0], key_size);

                // Read value
                T value;
                file.read(reinterpret_cast<char *>(&value), sizeof(T));

                // Store key-value pair in the map
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

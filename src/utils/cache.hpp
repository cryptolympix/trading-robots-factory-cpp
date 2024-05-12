#ifndef CACHE_HPP
#define CACHE_HPP

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <unordered_map>
#include <string>
#include "../types.hpp"

/**
 * @brief Templated class for caching dictionary data to a file and loading it back.
 *
 * This class provides functionality to cache a dictionary of key-value pairs to a file
 * and to load the cached data back into memory.
 */
class Cache
{
public:
    std::string file_path;                  // The file path where the dictionary will be cached.
    std::map<std::string, CachedData> data; // The dictionary data to be cached.

public:
    /**
     * @brief Constructor for the Cache class.
     *
     * Initializes the Cache object with the specified file path.
     *
     * @param file_path The file path where the dictionary will be cached.
     */
    Cache(const std::string &file_path);

    /**
     * @brief Caches the dictionary to the file specified in the constructor.
     *
     * This function writes the entire Cache object to the file, including
     * the dictionary data and the file path.
     */
    void create();

    /**
     * @brief Loads the cached dictionary from the file specified in the constructor.
     *
     * This function reads the entire Cache object from the file, including
     * the dictionary data and the file path.
     *
     * @return Cache The loaded Cache object.
     */
    static Cache *load(const std::string &file_path);

    /**
     * @brief Check if the cache file exists.
     *
     * @return bool True if the cache file exists, false otherwise.
     */
    bool exist();

    /**
     * @brief Get the value of a key in the dictionary.
     *
     * @param key The key whose value is to be retrieved.
     * @return The value of the key.
     */
    CachedData get(const std::string &key);

    /**
     * @brief Set the value of a key in the dictionary.
     *
     * @param key The key whose value is to be set.
     * @param value The value to be set.
     */
    void add(const std::string &key, CachedData value);

    /**
     * @brief Check if a key exists in the dictionary.
     *
     * @param key The key to check.
     * @return bool True if the key exists, false otherwise.
     */
    bool has(const std::string &key);
};

#endif // CACHE_HPP

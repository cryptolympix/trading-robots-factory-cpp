#ifndef CACHE_HPP
#define CACHE_HPP

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <unordered_map>
#include <string>
#include "../types.hpp"

/**
 * @brief Cache a dictionary to a file.
 *
 * @tparam T Type of dictionary values.
 * @param data Dictionary to be cached.
 * @param file_path File path where the dictionary will be cached.
 */
template <typename T>
void cache_dictionary(const std::unordered_map<std::string, T> &data, const std::string &file_path);

/**
 * @brief Load a cached dictionary from a file.
 *
 * @tparam T Type of dictionary values.
 * @param file_path File path from which to load the dictionary.
 * @return std::unordered_map<std::string, T> Loaded dictionary.
 */
template <typename T>
std::unordered_map<std::string, T> load_cached_dictionary(const std::string &file_path);

#endif /* CACHE_HPP */
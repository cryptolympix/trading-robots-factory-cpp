#include <iostream>
#include <fstream>
#include <stdexcept>
#include <unordered_map>
#include <string>
#include <filesystem>
#include "../libs/json.hpp"
#include "cache.hpp"
#include "time_frame.hpp"
#include "../types.hpp"

/**
 * @brief Constructor for the Cache class.
 *
 * Initializes the Cache object with the specified file path.
 *
 * @param file_path The file path where the dictionary will be cached.
 */
Cache::Cache(const std::string &file_path) : file_path(file_path) {}

/**
 * @brief Caches the dictionary to the file specified in the constructor.
 *
 * This function writes the entire Cache object to the file, including
 * the dictionary data and the file path.
 */
void Cache::create()
{
    // Check if the directory exists, create it if it doesn't
    std::string directory = this->file_path.substr(0, this->file_path.find_last_of("/"));
    if (directory != this->file_path && !std::filesystem::exists(directory))
    {
        std::filesystem::create_directories(directory);
    }

    // Write the entire object to the file
    std::ofstream file(file_path, std::ios::binary);
    if (file.is_open())
    {
        nlohmann::json json_data;

        // Map for converting timeframes to strings
        std::map<TimeFrame, std::string> tf_map = {
            {TimeFrame::M1, "M1"},
            {TimeFrame::M5, "M5"},
            {TimeFrame::M15, "M15"},
            {TimeFrame::M30, "M30"},
            {TimeFrame::H1, "H1"},
            {TimeFrame::H4, "H4"},
            {TimeFrame::D1, "D1"}};

        for (auto const &[date, value] : data)
        {
            for (auto const &[tf, candles] : value.candles)
            {
                for (auto const &candle : candles)
                {
                    json_data[date]["candles"][tf_map[tf]] = nlohmann::json::array();
                    json_data[date]["candles"][tf_map[tf]].push_back({
                        {"date", candle.date},
                        {"open", candle.open},
                        {"high", candle.high},
                        {"low", candle.low},
                        {"close", candle.close},
                        {"volume", candle.volume},
                        {"tick_volume", candle.tick_volume},
                        {"spread", candle.spread},
                    });
                }
            }

            for (auto const &[tf, indicators] : value.indicators)
            {
                json_data[date]["indicators"][tf_map[tf]] = nlohmann::json::object();
                for (auto const &[id, values] : indicators)
                {
                    json_data[date]["indicators"][tf_map[tf]][id] = nlohmann::json::array();
                    for (auto const &value : values)
                    {
                        json_data[date]["indicators"][tf_map[tf]][id].push_back(value);
                    }
                }
            }

            json_data[date]["base_currency_conversion_rate"] = value.base_currency_conversion_rate;
        }

        file << json_data.dump();
        file.close();
    }
    else
    {
        throw std::runtime_error("Unable to open file for writing: " + file_path);
    }
}

/**
 * @brief Loads the cached dictionary from the file specified in the constructor.
 *
 * This function reads the entire Cache object from the file, including
 * the dictionary data and the file path.
 *
 * @return Cache The loaded Cache object.
 */
Cache *Cache::load(const std::string &file_path)
{
    std::ifstream file(file_path, std::ios::binary);
    if (file.is_open())
    {
        Cache *cache = new Cache(file_path);
        nlohmann::json json_data;
        file >> json_data;

        // Map for converting strings to timeframes
        std::map<std::string, TimeFrame> tf_map = {
            {"M1", TimeFrame::M1},
            {"M5", TimeFrame::M5},
            {"M15", TimeFrame::M15},
            {"M30", TimeFrame::M30},
            {"H1", TimeFrame::H1},
            {"H4", TimeFrame::H4},
            {"D1", TimeFrame::D1}};

        for (auto const &[date, value] : json_data.items())
        {
            CacheData cache_data;
            for (auto const &[tf, candles] : value["candles"].items())
            {
                for (auto const &candle : candles)
                {
                    cache_data.candles[tf_map[tf]].push_back({
                        candle["date"],
                        candle["open"],
                        candle["high"],
                        candle["low"],
                        candle["close"],
                        candle["volume"],
                        candle["tick_volume"],
                        candle["spread"],
                    });
                }
            }

            for (auto const &[tf, indicators] : value["indicators"].items())
            {
                for (auto const &[id, values] : indicators.items())
                {
                    std::vector<double> indicator_values;
                    for (auto const &value : values)
                    {
                        indicator_values.push_back(value);
                    }
                    cache_data.indicators[tf_map[tf]][id] = indicator_values;
                }
            }

            cache_data.base_currency_conversion_rate = value["base_currency_conversion_rate"];

            cache->add(date, cache_data);
        }

        file.close();
        return cache;
    }
    else
    {
        throw std::runtime_error("Unable to open file for reading: " + file_path);
    }
}

/**
 * @brief Check if the cache file exists.
 *
 * @return bool True if the cache file exists, false otherwise.
 */
bool Cache::exist()
{
    return std::ifstream(this->file_path).good();
}

/**
 * @brief Get the value of a key in the dictionary.
 *
 * @param key The key whose value is to be retrieved.
 * @return CacheData The value of the key.
 */
CacheData Cache::get(const std::string &key)
{
    if (!this->has(key))
    {
        throw std::runtime_error("Key not found: " + key);
    }
    return data[key];
}

/**
 * @brief Set the value of a key in the dictionary.
 *
 * @param key The key whose value is to be set.
 * @param value The value to be set.
 */
void Cache::add(const std::string &key, CacheData value)
{
    data[key] = value;
}

/**
 * @brief Check if a key exists in the dictionary.
 *
 * @param key The key to check.
 * @return bool True if the key exists, false otherwise.
 */
bool Cache::has(const std::string &key)
{
    return data.find(key) != data.end();
}

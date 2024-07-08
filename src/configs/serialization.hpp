#ifndef CONFIGS_SERIALIZATION_HPP
#define CONFIGS_SERIALIZATION_HPP

#include <string>
#include <vector>
#include <map>
#include "../libs/json.hpp"
#include "../types.hpp"

/**
 * @brief Create a JSON object from a Config object
 * @return JSON object
 */
nlohmann::json config_to_json(const Config &config);

/**
 * @brief Create a Config object from a JSON object
 * @return Config object
 */
Config config_from_json(const nlohmann::json &json);

/**
 * @brief Check if two Config objects are the same
 * @return True if the Config objects are the same, false otherwise
 */
bool is_same_config(const Config &config1, const Config &config2);

#endif // CONFIGS_SERIALIZATION_HPP

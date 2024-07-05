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

#endif // CONFIGS_SERIALIZATION_HPP

#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include "config.hpp"

// Function to load configuration from a text file
neat::Config neat::load_config(const std::string &filename)
{
    neat::Config config;

    std::ifstream file(filename);
    if (file.is_open())
    {
        std::string line;
        while (std::getline(file, line))
        {
            if (line.empty() || line[0] == '#')
                // Skip empty lines and comments
                continue;

            // Parse key-value pair
            std::istringstream iss(line);
            std::string key, value;
            std::getline(iss, key, '=');
            std::getline(iss, value);

            // Remove leading and trailing whitespaces from key and value
            key.erase(0, key.find_first_not_of(" \t"));
            key.erase(key.find_last_not_of(" \t") + 1);
            value.erase(0, value.find_first_not_of(" \t"));
            value.erase(value.find_last_not_of(" \t") + 1);

            // Populate NeatConfig based on key-value pairs
            if (key == "population_size")
                config.population_size = std::stoi(value);
            else if (key == "fitness_threshold")
                config.fitness_threshold = std::stod(value);
            else if (key == "no_fitness_termination")
                config.no_fitness_termination = (value == "true");
            else if (key == "reset_on_extinction")
                config.reset_on_extinction = (value == "true");
            else if (key == "activation_default")
                config.activation_default = value;
            else if (key == "activation_mutate_rate")
                config.activation_mutate_rate = std::stod(value);
            else if (key == "num_inputs")
                config.num_inputs = std::stoi(value);
            else if (key == "num_outputs")
                config.num_outputs = std::stoi(value);
            else if (key == "compatibility_disjoint_coefficient")
                config.compatibility_disjoint_coefficient = std::stod(value);
            else if (key == "compatibility_weight_coefficient")
                config.compatibility_weight_coefficient = std::stod(value);
            else if (key == "conn_add_prob")
                config.conn_add_prob = std::stod(value);
            else if (key == "conn_delete_prob")
                config.conn_delete_prob = std::stod(value);
            else if (key == "enabled_default")
                config.enabled_default = (value == "true");
            else if (key == "enabled_mutate_rate")
                config.enabled_mutate_rate = std::stod(value);
            else if (key == "initial_connections")
                config.initial_connections = value;
            else if (key == "node_add_prob")
                config.node_add_prob = std::stod(value);
            else if (key == "node_delete_prob")
                config.node_delete_prob = std::stod(value);
            else if (key == "weight_init_mean")
                config.weight_init_mean = std::stod(value);
            else if (key == "weight_init_stdev")
                config.weight_init_stdev = std::stod(value);
            else if (key == "weight_init_type")
                config.weight_init_type = value;
            else if (key == "weight_max_value")
                config.weight_max_value = std::stod(value);
            else if (key == "weight_min_value")
                config.weight_min_value = std::stod(value);
            else if (key == "weight_mutate_rate")
                config.weight_mutate_rate = std::stod(value);
            else if (key == "weight_replace_rate")
                config.weight_replace_rate = std::stod(value);
            else if (key == "max_stagnation")
                config.max_stagnation = std::stoi(value);
            else if (key == "species_elitism")
                config.species_elitism = std::stoi(value);
            else if (key == "elitism")
                config.elitism = std::stoi(value);
            else if (key == "survival_threshold")
                config.survival_threshold = std::stod(value);
            else if (key == "min_species_size")
                config.min_species_size = std::stoi(value);
            else if (key == "compatibility_threshold")
                config.compatibility_threshold = std::stod(value);
            else if (key == "bad_species_threshold")
                config.bad_species_threshold = std::stod(value);
            else
                std::cerr << "Unknown key: " << key << std::endl;
        }
        file.close();
    }
    else
        std::cerr << "Unable to open file: " << filename << std::endl;

    return config;
}

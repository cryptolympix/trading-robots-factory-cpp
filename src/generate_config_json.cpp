#include <iostream>
#include <fstream>
#include <string>
#include <filesystem> // For std::filesystem
#include "configs/configs.hpp"
#include "configs/serialization.hpp"
#include "libs/json.hpp"
#include "types.hpp"

namespace fs = std::filesystem;

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <config_name>" << std::endl;
        return 1;
    }

    // Get the configuration name from the command line arguments.
    std::string config_name = argv[1];

    if (configs.find(config_name) == configs.end())
    {
        std::cerr << "The configuration name " << config_name << " does not exist." << std::endl;

        std::vector<std::string> config_names;
        for (const auto &[config_name, config] : configs)
        {
            config_names.push_back(config_name);
        }

        std::cerr << "Available configurations: " << std::endl;
        for (const std::string &config_name : config_names)
        {
            std::cerr << "  - " << config_name << std::endl;
        }

        return 1;
    }

    // Load the default configuration.
    Config config = configs[config_name];

    // Convert the Config object to a JSON object.
    nlohmann::json json_config = config_to_json(config);

    // Ensure the directory exists.
    std::string config_directory = "./configs/";
    try
    {
        if (!fs::exists(config_directory))
        {
            fs::create_directories(config_directory);
        }
    }
    catch (const fs::filesystem_error &e)
    {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
        return 1;
    }

    // Create the JSON configuration to a file.
    std::string config_file_path = config_directory + config.general.name + "_" + config.general.version + ".json";
    std::ofstream file(config_file_path);

    if (!file)
    {
        std::cerr << "Unable to create file " << config_file_path << std::endl;
        return 1;
    }

    file << json_config.dump(4);
    file.close();

    return 0;
}

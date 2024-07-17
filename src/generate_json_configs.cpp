#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include "configs/configs_list.hpp"
#include "configs/serialization.hpp"
#include "libs/json.hpp"
#include "types.hpp"

namespace fs = std::filesystem;

int main(int argc, char *argv[])
{
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

    for (Config config : configs)
    {
        // Convert the Config object to a JSON object.
        nlohmann::json json_config = config_to_json(config);

        // Path to the configuration file.
        std::string config_file_path = config_directory + config.general.name + "_" + config.general.version + ".json";

        if (fs::exists(config_file_path))
        {
            std::cerr << "Configuration file " << config_file_path << " already exists" << std::endl;
            return 1;
        }

        // Create the JSON configuration to a file.
        std::ofstream file(config_file_path);

        if (!file)
        {
            std::cerr << "Unable to create file " << config_file_path << std::endl;
            return 1;
        }

        file << json_config.dump(4);
        file.close();

        std::cout << "📁 Created configuration file: " << config_file_path << std::endl;
    }

    return 0;
}

#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include "configs/configs.hpp"
#include "configs/serialization.hpp"
#include "libs/json.hpp"
#include "types.hpp"

namespace fs = std::filesystem;

int main()
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

        std::cout << "📁 Created configuration file: " << config_file_path << std::endl;
    }

    return 0;
}

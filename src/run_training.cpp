#include <filesystem>
#include <iostream>
#include <string>
#include "training.hpp"

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        std::cerr << "Usage: " << argv[0] << " <id> <config_file_path>" << std::endl;
        return 1;
    }

    // Get the id and configuration path from the command line arguments.
    std::string id = argv[1];
    std::filesystem::path config_file_path = argv[2];

    Training training(id, config_file_path, false);
    training.prepare();
    training.run();
    return 0;
}

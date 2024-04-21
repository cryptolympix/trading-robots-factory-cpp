#include <ctime>
#include <chrono>
#include "configs/test.hpp"
#include "training.hpp"

int main()
{
    auto now = std::chrono::system_clock::now();
    time_t date = std::chrono::system_clock::to_time_t(now);
    std::stringstream date_string;
    date_string << std::put_time(std::localtime(&date), "%Y%m%d%H%M%S");
    std::string id = date_string.str();

    Training training(id, test_config, true);
    training.prepare();
    training.run();
    return 0;
}

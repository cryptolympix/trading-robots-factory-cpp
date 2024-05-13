#include <ctime>
#include <chrono>
#include <sstream>
#include <iomanip>
#include "configs/default.hpp"
#include "training.hpp"

int main()
{
    std::string id = generate_date_uid();
    Training training(id, __config__, true);
    training.prepare();
    training.run();

    return 0;
}

#include "configs/default.hpp"
#include "training.hpp"

int main()
{
    // std::string id = generate_date_uid();
    std::string id = "1";
    Training training(id, __config__, false);
    training.prepare();
    training.run();
    return 0;
}

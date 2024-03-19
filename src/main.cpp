#include "configs/test.hpp"
#include "training.hpp"

int main()
{
    Training training(test_config, true);
    training.prepare();
    training.run();
    return 0;
}

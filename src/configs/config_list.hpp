#ifndef ALL_CONFIGS_HPP
#define ALL_CONFIGS_HPP

#include <ctime>
#include "../types.hpp"
#include "../trading/schedule.hpp"
#include "../neat/config.hpp"

// ========= Configs ========= //
// #include "./custom/Example.hpp"
#include "./custom/Forex.hpp"
// ============================ //

std::vector<Config> configs = {
    Forex,
};

#endif // CONFIG_LIST_HPP
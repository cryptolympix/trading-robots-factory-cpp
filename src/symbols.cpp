#include <string>
#include <unordered_map>
#include "types.hpp"

/**
 * @brief Mapping of symbol names to their corresponding SymbolInfos objects.
 */
std::unordered_map<std::string, SymbolInfos> symbol_infos = {
    {"EURUSD", {.asset = "EUR", .base = "USD", .decimal_places = 5, .point_value = 0.0001, .contract_size = 100000, .min_lot_size = 0, .max_lot_size = 0, .lot_size_step = 0.01, .commission_per_lot = 2.6, .commission_base = "EUR"}}};

#ifndef SYMBOL_H
#define SYMBOL_H

#include <string>
#include <unordered_map>
#include "types.hpp"

/**
 * @brief Mapping of symbol names to their corresponding SymbolInfos objects.
 */
extern std::unordered_map<std::string, SymbolInfos> symbol_infos;

#endif // SYMBOL_H

#ifndef SYMBOL_INFO_HPP
#define SYMBOL_INFO_HPP

#include <string>
#include <unordered_map>
#include "types.hpp"

/**
 * @brief Mapping of symbol names to their corresponding SymbolInfos objects.
 */
extern std::unordered_map<std::string, SymbolInfo> symbol_infos;

#endif // SYMBOL_INFO_HPP

#pragma once

#include <unordered_map>
#include <vector>

#include "types.h"

namespace multiplayer {
void clear();
s32 getKeyEvent();
extern std::unordered_map<s32, s8> g_SymbolToPlayer;
extern std::unordered_map<s8, std::vector<s32>> g_PlayerToSymbols;
extern std::unordered_map<s8, s32> g_PlayerToLastUsedSymbol;
}
#include "multiplayer.h"

#include <algorithm>
#include <cstring>

#include "babil.h"
#include "menu/player_select.h"
#include "pad_manager.h"
#include "patches.h"
#include "util.h"

using namespace pad_manager;
using namespace babil;

namespace multiplayer {

std::unordered_map<s32, s8> g_SymbolToPlayer;
std::unordered_map<s8, std::vector<s32>> g_PlayerToSymbols;
std::unordered_map<s8, s32> g_PlayerToLastUsedSymbol;

bool partyNeedsUpdate() {
    for (s8 i = 0; i < 5; i++) {
        pl::Player::cls* member = pl::PlayerParty::memberForOrder(pl::PlayerParty::playerPartyInstance_, i);
        if (member == pl::PlayerParty::InvalidPlayer) {
            continue;
        }
        if (g_SymbolToPlayer.find(member->m_PlayerType) == g_SymbolToPlayer.end()) {
            player_select::open();
            return true;
        }
    }
    return false;
}
void clear() {
    g_SymbolToPlayer.clear();
    g_PlayerToSymbols.clear();
    g_PlayerToLastUsedSymbol.clear();
}

void setSymbol(s32 symbol, world::WorldStateScheduler::cls* wss, world::WorldState::cls* ws, const char* currentState) {
    patches::game::g_NextSymbol                                                                               = symbol;
    *reinterpret_cast<u32*>(reinterpret_cast<u8*>(&part::WorldPart::wpInstance_->m_WorldStateContext) + 0x58) = 0;
    world::WorldStateScheduler::wssAddStateSchedule(wss, "chgsym");
    world::WorldStateScheduler::wssAddStateSchedule(wss, currentState);
    *reinterpret_cast<bool*>(reinterpret_cast<u8*>(ws) + 0x77) = true;
    world::WorldState::wsSetEnd(ws);
}

bool isUsableSymbol(s32 symbol) {
    pl::Player::cls* player       = pl::PlayerParty::member(pl::PlayerParty::playerPartyInstance_, (pl::PLAYER_TYPES)symbol);
    ys::Condition::cls* condition = pl::Player::condition(player);
    return !ys::Condition::isNotBattleCondition(condition);
}

s32 getSymbolForPad(int pad) {
    auto it = g_PlayerToLastUsedSymbol.find(pad);
    if (it == g_PlayerToLastUsedSymbol.end())
        return -1;

    if (isUsableSymbol(it->second))
        return it->second;

    const auto& characters = g_PlayerToSymbols[pad];
    size_t idx             = std::distance(characters.begin(), std::find(characters.begin(), characters.end(), it->second));
    for (s8 i = 0; i < characters.size() - 1; i++) {
        idx         = (idx + 1) % characters.size();
        auto symbol = characters[idx];
        if (isUsableSymbol(symbol)) {
            return symbol;
        }
    }
    return -1;
}

s32 changeSymbol(s32 symbol) {
    const auto& characters = g_PlayerToSymbols[g_CurrentPad];
    auto it                = std::find(characters.begin(), characters.end(), symbol);
    if (it == characters.end()) {
        debugPrintf("patches::game::next_symbol: %d\n", patches::game::g_NextSymbol);
        return g_PlayerToLastUsedSymbol[g_CurrentPad];
    }

    size_t idx = std::distance(characters.begin(), it);
    for (size_t i = 0; i < characters.size(); i++) {
        idx             = (idx + 1) % characters.size();
        s32 next_symbol = characters[idx];
        if (isUsableSymbol(next_symbol)) {
            return next_symbol;
        }
    }

    return symbol;
}

s32 getMove(world::WorldStateScheduler::cls* wss, world::WorldState::cls* ws) {
    const char* state_name = world::WorldState::wsGetString(ws);

    s32 id = sys::GameParameter::fieldSymbolID(sys::GameParameter::gpInstance_);
    s8 idx = 0;
    if (g_SymbolToPlayer.find(id) != g_SymbolToPlayer.end()) {
        idx = g_SymbolToPlayer[id];
    }

    g_CurrentPad = idx;

    s32 mask = 0;

    s32 kHeld = getHeld(g_Pads[idx]);
    g_LockedUntilRelease[idx] &= kHeld;
    kHeld &= ~g_LockedUntilRelease[idx];
    mask |= kHeld & (FF4Button_Up | FF4Button_Down | FF4Button_Left | FF4Button_Right | FF4Button_Cancel | FF4Button_Select | FF4Button_Next | FF4Button_Menu);

    s32 kDown = getDown(g_Pads[idx]);
    kDown &= ~g_LockedUntilRelease[idx];
    if (kDown & FF4Button_Prev) {
        g_CurrentPad    = idx;
        s32 next_symbol = changeSymbol(id);
        if (next_symbol == id)
            return mask;
        g_PlayerToLastUsedSymbol[g_CurrentPad] = next_symbol;
        patches::game::g_NextSymbol            = next_symbol;
        return mask | FF4Button_Prev;
    }

    if (mask != 0)
        return mask;

    for (s8 i = 0; i < g_ConnectedPads; i++) {
        if (i == idx)
            continue;
        if (g_PlayerToLastUsedSymbol.find(i) != g_PlayerToLastUsedSymbol.end()) {
            kHeld = getHeld(g_Pads[i]);
            g_LockedUntilRelease[i] &= kHeld;
            kHeld &= ~g_LockedUntilRelease[i];
            if (kHeld & (FF4Button_Up | FF4Button_Down | FF4Button_Left | FF4Button_Right | FF4Button_Cancel | FF4Button_Select | FF4Button_Next | FF4Button_Menu | FF4Button_Prev)) {
                s32 symbol = getSymbolForPad(i);
                if (symbol != -1) {
                    g_CurrentPad = i;
                    setSymbol(symbol, wss, ws, state_name);
                    mask |= kHeld & (FF4Button_Up | FF4Button_Down | FF4Button_Left | FF4Button_Right | FF4Button_Cancel | FF4Button_Select | FF4Button_Next | FF4Button_Menu);
                    g_LockedUntilRelease[i] |= kHeld & FF4Button_Prev;
                    return mask;
                }
            }
        }
    }

    return mask;
}

s32 getWorldMenu() {
    s32 mask = getHeld(g_Pads[g_CurrentPad]);
    g_LockedUntilRelease[g_CurrentPad] &= mask;
    mask &= ~g_LockedUntilRelease[g_CurrentPad];
    return mask;
}

s32 getWorldPart() {
    if (partyNeedsUpdate()) {
        return 0;
    }

    world::WorldStateScheduler::cls* wss = patches::game::wssInstance_;
    if (wss == nullptr)
        return -1;
    world::WorldState::cls* ws = world::WorldStateScheduler::wssGetCurrentState(wss);
    if (ws == nullptr)
        return -1;

    const char* state_name = world::WorldState::wsGetString(ws);
    if (strcmp(state_name, "world move") == 0 || strcmp(state_name, "vehicle move") == 0) {
        return getMove(wss, ws);
    } else {
        s32 mask = getHeld(g_Pads[g_CurrentPad]);
        g_LockedUntilRelease[g_CurrentPad] &= mask;
        mask &= ~g_LockedUntilRelease[g_CurrentPad];
        return mask;
        // if (strcmp(state_name, "world menu") == 0) {
        //     return getWorldMenu();
        // }
    }

    return -1;
}

s32 getBattlePart() {
    if (partyNeedsUpdate()) {
        return 0;
    }

    s8 player = g_SymbolToPlayer[patches::game::g_LastBattlePlayerId];
    s32 mask  = getHeld(g_Pads[player]);
    g_LockedUntilRelease[player] &= mask;
    mask &= ~g_LockedUntilRelease[player];

    return mask;
}

s32 getKeyEvent() {
    u64 kHeld;
    u64 kDown;
    if (g_ConnectedPads < 2)
        return -1;

    auto current_part = sys::GGlobal::getCurrentPart();
    if (current_part == BabilGamePart_WorldPart) {
        return getWorldPart();
    } else if (current_part == BabilGamePart_BattlePart) {
        return getBattlePart();
    }

    return -1;
}
}
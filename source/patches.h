#pragma once

#include <switch.h>

#include <vector>

#include "babil.h"

namespace patches {
namespace game {
    void patch();
    int getFieldSymbolID();
    void test();
    extern s32 g_NextSymbol;
    extern s32 g_LastBattlePlayerId;
    extern babil::world::WSMove::cls* WSMoveInstance_;
    extern babil::world::WorldStateScheduler::cls* wssInstance_;
    extern babil::Font::cls* FontInstance_;
}
namespace jni {
    extern s8 fake_env[0x1000];
    void patch();
}
}
#pragma once

#include "types.h"

namespace game {
void start();
void init();
void launch();
extern bool g_menuOpen;
extern bool g_playerSelectOpen;
extern bool g_encountersEnabled;
extern float g_gilMultiplier;
extern float g_expMultiplier;
extern bool g_saveAnywhere;
extern float g_FakeTouch[2];
extern bool g_UseFakeTouch;
extern bool g_Launched;
extern u64 g_MaxUpdatesPerFrame;
extern bool g_ShouldRebuildFonts;
constexpr float IMGUI_SCALE = 1.5f;
}
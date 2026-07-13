#include "imgui_impl_switch.h"

#include <switch.h>

#include "../game.h"
#include "imgui.h"

static u64 g_LastTick = 0;

void ImGui_ImplSwitch_Init() {
    ImGuiIO& io            = ImGui::GetIO();
    io.DisplaySize         = ImVec2(1920.0f, 1080.0f);
    io.BackendPlatformName = "imgui_impl_switch";
    io.BackendFlags |= ImGuiBackendFlags_HasGamepad;
    g_LastTick = armGetSystemTick();
}

void ImGui_ImplSwitch_NewFrame() {
    ImGuiIO& io = ImGui::GetIO();

    u64 now      = armGetSystemTick();
    io.DeltaTime = (float)(now - g_LastTick) / (float)armGetSystemTickFreq();
    if (io.DeltaTime <= 0.0f)
        io.DeltaTime = 1.0f / 60.0f;
    g_LastTick = now;

    HidTouchScreenState touch;
    hidGetTouchScreenStates(&touch, 1);

    if (touch.count > 0) {
        io.AddMousePosEvent((float)touch.touches[0].x * game::IMGUI_SCALE, (float)touch.touches[0].y * game::IMGUI_SCALE);
        io.AddMouseButtonEvent(0, true);
    } else {
        io.AddMouseButtonEvent(0, false);
    }
}
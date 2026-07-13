#include "menu.h"

#include <GLES/gl.h>

#include <unordered_map>
#include <vector>

#include "../babil.h"
#include "../bridge.h"
#include "../config.h"
#include "../game.h"
#include "../imgui/imgui.h"
#include "../pad_manager.h"
#include "../stb_image.h"
#include "tab.h"
#include "tabs/about.h"
#include "tabs/achievements.h"
#include "tabs/config.h"
#include "tabs/credits.h"
#include "tabs/fonts.h"
#include "tabs/launch.h"
#include "tabs/mods.h"

using namespace babil;
using namespace pad_manager;

namespace menu {

static std::vector<Tab*> g_Tabs;
static int currentTab = 0;

static std::unordered_map<u32, float> g_HoldTimes;
static float g_DeltaTime = 0.0f;

static constexpr u32 kAllButtons[] = {
    FF4Button_Select,
    FF4Button_Cancel,
    FF4Button_DebugA,
    FF4Button_DebugB,
    FF4Button_Right,
    FF4Button_Left,
    FF4Button_Up,
    FF4Button_Down,
    FF4Button_ChangeMainCharacter,
    FF4Button_Next,
    FF4Button_Menu,
    FF4Button_Prev,
};

void init() {
    g_Tabs.push_back(new tabs::Launch());
    g_Tabs.push_back(new tabs::Achievements());
    g_Tabs.push_back(new tabs::Config());
    g_Tabs.push_back(new tabs::Mods());
    g_Tabs.push_back(new tabs::Fonts());
    g_Tabs.push_back(new tabs::About());
    g_Tabs.push_back(new tabs::Credits());
}

void update() {
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(config::screen_width, config::screen_height), ImGuiCond_Always);
    ImGui::SetNextWindowBgAlpha(0.9f);
    ImGui::SetNextWindowFocus();

    g_DeltaTime = 1.0f / bridge::framerate;

    if (game::g_Launched && currentTab == 0)
        currentTab = 1;

    auto special_keybinds = getSpecialKeybinds(g_Pads[0]);
    if (!(padGetButtons(&g_Pads[0]) & special_keybinds[SpecialButton_Modifier])) {
        s32 kDown = getDown(g_Pads[0]);
        s32 kHeld = getHeld(g_Pads[0]);

        for (u32 key : kAllButtons) {
            if (!(kDown & key))
                continue;
            if (g_Tabs[currentTab]->down(key))
                continue;

            switch (key) {
                case FF4Button_Prev:
                    if (currentTab > game::g_Launched ? 1 : 0) {
                        currentTab--;
                        if (currentTab == 1 && !game::g_Launched)
                            currentTab = 0;
                        playMenuSE(3);
                    }
                    break;
                case FF4Button_Next:
                    if (currentTab < (int)g_Tabs.size() - 1) {
                        currentTab++;
                        if (currentTab == 1 && !game::g_Launched)
                            currentTab = 2;
                        playMenuSE(3);
                    }
                    break;
                case FF4Button_Cancel:
                    if (game::g_Launched)
                        close();
                    break;
                default:
                    break;
            }
        }

        for (u32 key : kAllButtons) {
            if (kHeld & key) {
                float& holdTime = g_HoldTimes[key];
                holdTime += g_DeltaTime;
                g_Tabs[currentTab]->held(key, holdTime);
            } else {
                g_HoldTimes[key] = 0.0f;
            }
        }
    }

    if (!game::g_menuOpen) {
        return;
    }

    ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;
    if (game::g_Launched)
        flags |= ImGuiWindowFlags_NoTitleBar;

    if (ImGui::Begin("Final Fantasy IV Launcher", nullptr, flags)) {
        if (ImGui::BeginTabBar("TabBar")) {
            for (int i = 0; i < g_Tabs.size(); i++) {
                if (i == 0 && game::g_Launched)
                    continue;
                if (i == 1 && !game::g_Launched)
                    continue;
                Tab* tab                = g_Tabs[i];
                ImGuiTabItemFlags flags = ImGuiTabItemFlags_None;
                ImVec4 color;
                if (currentTab == i) {
                    flags |= ImGuiTabItemFlags_SetSelected;
                    color = ImGui::GetStyleColorVec4(ImGuiCol_TabHovered);
                } else {
                    color = ImGui::GetStyleColorVec4(ImGuiCol_Tab);
                }
                ImGui::PushStyleColor(ImGuiCol_Tab, color);
                ImGui::PushStyleColor(ImGuiCol_TabHovered, color);
                ImGui::PushStyleColor(ImGuiCol_TabActive, color);
                bool tab_open = ImGui::BeginTabItem(tab->m_Name.c_str(), nullptr, flags);
                ImGui::PopStyleColor(3);
                if (ImGui::IsItemClicked()) {
                    currentTab = i;
                }
                if (tab_open) {
                    if (ImGui::BeginChild("Tab Child", ImVec2(0.0f, 0.0f), ImGuiChildFlags_None)) {
                        tab->update();
                    }
                    ImGui::EndChild();
                    ImGui::EndTabItem();
                }
            }
            ImGui::EndTabBar();
        }
    }
    ImGui::End();
}

void preUpdate() {
    g_Tabs[currentTab]->preUpdate();
}

void postUpdate() {
    g_Tabs[currentTab]->postUpdate();
}

void close() {
    game::g_menuOpen = false;
    lockAllUntilRelease();
    if (config::marked_for_write)
        config::write_config();
    if (pad_manager::marked_for_write)
        pad_manager::write_controls();

    playMenuSE(5);
}

void playMenuSE(s32 index) {
    ds::snd::SEHandle::cls handle;
    ds::snd::SEHandle::Ctor(&handle);
    ds::snd::SEHandle::Play(&handle, 0, index, 100, 0);
    ds::snd::SEHandle::Dtor(&handle);
}
}
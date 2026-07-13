#include "keybind.h"

#include <switch.h>

#include "../../imgui/imgui.h"
#include "../../pad_manager.h"
#include "../menu.h"

namespace menu::elements {

constexpr const char* ButtonNames[] = {
    "A",
    "B",
    "X",
    "Y",
    "Left Stick",
    "Right Stick",
    "L",
    "R",
    "ZL",
    "ZR",
    "+",
    "-",
    "D-Pad Left",
    "D-Pad Up",
    "D-Pad Right",
    "D-Pad Down",
    "Left Stick Left",
    "Left Stick Up",
    "Left Stick Right",
    "Left Stick Down",
    "Right Stick Left",
    "Right Stick Up",
    "Right Stick Right",
    "Right Stick Down",
    "Left SL",
    "Left SR",
    "Right SL",
    "Right SR",
    "Poké Ball Button",
    "Verification",
    "Handheld Left B",
    "C Left",
    "C Up",
    "C Right",
    "C Down"
};

constexpr u64 AllowedKeybinds = HidNpadButton_A
    | HidNpadButton_B
    | HidNpadButton_X
    | HidNpadButton_Y
    | HidNpadButton_StickL
    | HidNpadButton_StickR
    | HidNpadButton_L
    | HidNpadButton_R
    | HidNpadButton_ZL
    | HidNpadButton_ZR
    | HidNpadButton_Plus
    | HidNpadButton_Minus
    | HidNpadButton_Left
    | HidNpadButton_Up
    | HidNpadButton_Right
    | HidNpadButton_Down
    | HidNpadButton_LeftSL
    | HidNpadButton_LeftSR
    | HidNpadButton_RightSL
    | HidNpadButton_RightSR;

const char* GetButtonName(u64 key) {
    if (key == 0) {
        return "Unbound";
    }

    if ((key & (key - 1)) != 0) {
        return "Unknown";
    }

    const auto idx = std::__countr_zero(key);

    if (idx >= std::size(ButtonNames)) {
        return "Unknown";
    }

    return ButtonNames[idx];
}

Keybind::Keybind(const char* name, u64* keyptr, std::function<void(u64)> callback, u32 flags)
    : m_Name(name)
    , m_KeyPtr(keyptr)
    , m_Key(*keyptr)
    , m_Callback(callback)
    , m_Flags(flags) { };

void Keybind::update() {
    ImGui::PushID(m_Name.c_str());
    ImVec4 color;
    if (m_IsFocused) {
        color = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
    } else {
        color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    }
    ImGui::PushStyleColor(ImGuiCol_Text, color);
    ImGui::Text("%s:", m_Name.c_str());
    ImGui::PopStyleColor();
    ImGui::SameLine();
    if (m_Active)
        color = ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered);
    else
        color = ImGui::GetStyleColorVec4(ImGuiCol_Button);
    ImGui::PushStyleColor(ImGuiCol_Button, color);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, color);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, color);
    if (m_Active) {
        s32 kDown = padGetButtonsDown(&pad_manager::g_Pads[0]);
        if (kDown & AllowedKeybinds) {
            m_Key = kDown & -kDown;
            m_Callback(m_Key);
            m_Active = false;
        }
        if (ImGui::Button("Press Any Button")) {
            m_Active = false;
        }
    } else {
        m_Key = *m_KeyPtr;
        if (ImGui::Button(GetButtonName(m_Key))) {
            m_Active = true;
        }
    }
    ImGui::PopStyleColor(3);
    ImGui::PopID();

    if (m_Activate)
        m_Active = true;
    m_Activate = false;
}
bool Keybind::down(u32 key) {
    if (m_Active)
        return true;
    else if (key == pad_manager::FF4Button_Select) {
        playMenuSE(1);
        m_Activate = true;
        return true;
    } else if (key == pad_manager::FF4Button_Menu && m_Flags & KeybindFlags_AllowUnbind) {
        playMenuSE(2);
        m_Key = pad_manager::HidNpadButton_None;
        m_Callback(m_Key);
        return true;
    }
    return false;
}
}
#include "pad_manager.h"

#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>

#include "achievement.h"
#include "babil.h"
#include "bridge.h"
#include "config.h"
#include "game.h"
#include "menu/menu.h"
#include "multiplayer.h"
#include "patches.h"
#include "stdext.h"
#include "switch/services/hid.h"
#include "util.h"

using namespace babil;

namespace pad_manager {
PadState g_Pads[MAX_PLAYERS];
int g_CurrentPad                      = 0;
bool connected[MAX_PLAYERS]           = { false, false, false, false, false };
s8 g_ConnectedPads                    = 0;
s32 g_LockedUntilRelease[MAX_PLAYERS] = { 0, 0, 0, 0, 0 };
bool wasPausedByOverlay               = false;
bool marked_for_write                 = false;

HidNpadButton g_ProControllerCoreKeybinds[NUM_FF4_BUTTONS] = {
    HidNpadButton_A,
    HidNpadButton_B,
    HidNpadButton_Minus,
    HidNpadButton_Plus,
    HidNpadButton_Right,
    HidNpadButton_Left,
    HidNpadButton_Up,
    HidNpadButton_Down,
    HidNpadButton_ZR,
    HidNpadButton_ZL,
    HidNpadButton_X,
    HidNpadButton_Y,
};

HidNpadButton g_ProControllerSpecialKeybinds[NUM_SPECIAL_BUTTONS] = {
    HidNpadButton_L,
    HidNpadButton_Left,
    HidNpadButton_Right,
    HidNpadButton_Plus,
    HidNpadButton_X,
    HidNpadButton_Y,
};

HidNpadButton g_JoyLeftHorizontalCoreKeybinds[NUM_FF4_BUTTONS] = {
    HidNpadButton_Down,
    HidNpadButton_Left,
    HidNpadButton_None,
    HidNpadButton_None,
    HidNpadButton_StickLDown,
    HidNpadButton_StickLUp,
    HidNpadButton_StickLRight,
    HidNpadButton_StickLLeft,
    HidNpadButton_LeftSR,
    HidNpadButton_LeftSL,
    HidNpadButton_Right,
    HidNpadButton_Up,
};

HidNpadButton g_JoyLeftHorizontalSpecialKeybinds[NUM_SPECIAL_BUTTONS] = {
    HidNpadButton_Minus,
    HidNpadButton_Up,
    HidNpadButton_Down,
    HidNpadButton_None,
    HidNpadButton_Right,
    HidNpadButton_None,
};

HidNpadButton g_JoyLeftVerticalCoreKeybinds[NUM_FF4_BUTTONS] = {
    HidNpadButton_Right,
    HidNpadButton_Down,
    HidNpadButton_LeftSL,
    HidNpadButton_LeftSR,
    HidNpadButton_StickLRight,
    HidNpadButton_StickLLeft,
    HidNpadButton_StickLUp,
    HidNpadButton_StickLDown,
    HidNpadButton_ZL,
    HidNpadButton_L,
    HidNpadButton_Up,
    HidNpadButton_Left,
};

HidNpadButton g_JoyLeftVerticalSpecialKeybinds[NUM_SPECIAL_BUTTONS] = {
    HidNpadButton_Minus,
    HidNpadButton_Left,
    HidNpadButton_Right,
    HidNpadButton_None,
    HidNpadButton_Down,
    HidNpadButton_None,
};

HidNpadButton g_JoyRightHorizontalCoreKeybinds[NUM_FF4_BUTTONS] = {
    HidNpadButton_X,
    HidNpadButton_A,
    HidNpadButton_None,
    HidNpadButton_None,
    HidNpadButton_StickRUp,
    HidNpadButton_StickRDown,
    HidNpadButton_StickRLeft,
    HidNpadButton_StickRRight,
    HidNpadButton_RightSR,
    HidNpadButton_RightSL,
    HidNpadButton_Y,
    HidNpadButton_B,
};

HidNpadButton g_JoyRightHorizontalSpecialKeybinds[NUM_SPECIAL_BUTTONS] = {
    HidNpadButton_Plus,
    HidNpadButton_B,
    HidNpadButton_X,
    HidNpadButton_None,
    HidNpadButton_Y,
    HidNpadButton_None,
};

HidNpadButton g_JoyRightVerticalCoreKeybinds[NUM_FF4_BUTTONS] = {
    HidNpadButton_A,
    HidNpadButton_B,
    HidNpadButton_RightSL,
    HidNpadButton_RightSR,
    HidNpadButton_StickRRight,
    HidNpadButton_StickRLeft,
    HidNpadButton_StickRUp,
    HidNpadButton_StickRDown,
    HidNpadButton_ZR,
    HidNpadButton_R,
    HidNpadButton_X,
    HidNpadButton_Y,
};

HidNpadButton g_JoyRightVerticalSpecialKeybinds[NUM_SPECIAL_BUTTONS] = {
    HidNpadButton_Plus,
    HidNpadButton_Y,
    HidNpadButton_A,
    HidNpadButton_None,
    HidNpadButton_X,
    HidNpadButton_None,
};

void init() {
    hidInitialize();

    padConfigureInput(MAX_PLAYERS, HidNpadStyleSet_NpadStandard);
    hidSetNpadJoyHoldType(HidNpadJoyHoldType_Vertical);
    padInitialize(&g_Pads[0], HidNpadIdType_No1, HidNpadIdType_Handheld);
    padInitialize(&g_Pads[1], HidNpadIdType_No2);
    padInitialize(&g_Pads[2], HidNpadIdType_No3);
    padInitialize(&g_Pads[3], HidNpadIdType_No4);
    padInitialize(&g_Pads[4], HidNpadIdType_No5);

    for (int i = 0; i < MAX_PLAYERS; i++) {
        padUpdate(&g_Pads[i]);
        connected[i] = padIsConnected(&g_Pads[i]);
        if (connected[i])
            g_ConnectedPads++;
    }
}

void exit() {
    hidExit();
}

void update() {
    bool showCtrlSup = false;
    for (int i = 0; i < MAX_PLAYERS; i++) {
        padUpdate(&g_Pads[i]);
        bool isConnected = padIsConnected(&g_Pads[i]);
        if (isConnected != connected[i]) {
            connected[i] = isConnected;
            showCtrlSup  = true;
        }
    }

    if (showCtrlSup || !connected[0]) {
        Result rc;
        HidLaControllerSupportArg arg;
        hidLaCreateControllerSupportArg(&arg);
        arg.hdr.player_count_min                     = 1;
        arg.hdr.player_count_max                     = MAX_PLAYERS;
        arg.hdr.enable_left_justify                  = 1;
        arg.hdr.enable_permit_joy_dual               = 1;
        arg.hdr.enable_single_mode                   = !config::multiplayer_enabled;
        HidLaControllerSupportResultInfo result_info = { };
        hidLaShowControllerSupportForSystem(&result_info, &arg, false);
        g_ConnectedPads = result_info.player_count;
        for (int i = 0; i < MAX_PLAYERS; i++) {
            padUpdate(&g_Pads[i]);
            connected[i] = padIsConnected(&g_Pads[i]);
        }
        g_CurrentPad = 0;

        if (config::multiplayer_enabled) {
            multiplayer::clear();
        }
    }
}

s32 getCoreButtons(HidNpadButton keybinds[NUM_FF4_BUTTONS], u64 buttons) {
    s32 mask = 0;
    for (u8 i = 0; i < NUM_FF4_BUTTONS; i++) {
        if (buttons & keybinds[i])
            mask |= BIT(i);
    }
    return mask;
}

void setCoreKeybind(HidNpadButton keybinds[NUM_FF4_BUTTONS], u32 ff4_button, HidNpadButton button) {
    HidNpadButton current = keybinds[std::__countr_zero(ff4_button)];
    if (current != HidNpadButton_None) {
        for (u8 i = 0; i < NUM_FF4_BUTTONS; i++) {
            if (keybinds[i] == button)
                keybinds[i] = current;
        }
    }
    keybinds[std::__countr_zero(ff4_button)] = button;
}

void setSpecialKeybind(HidNpadButton keybinds[NUM_SPECIAL_BUTTONS], u32 special_button, HidNpadButton button) {
    keybinds[special_button] = button;
}

s32 getButtons(u32 styleSet, u64 buttons) {
    if (styleSet == HidNpadStyleTag_NpadJoyLeft) {
        if (config::joycon_hold_style == 0)
            return getCoreButtons(g_JoyLeftVerticalCoreKeybinds, buttons);
        else
            return getCoreButtons(g_JoyLeftHorizontalCoreKeybinds, buttons);
    }
    if (styleSet == HidNpadStyleTag_NpadJoyRight) {
        if (config::joycon_hold_style == 0)
            return getCoreButtons(g_JoyRightVerticalCoreKeybinds, buttons);
        else
            return getCoreButtons(g_JoyRightHorizontalCoreKeybinds, buttons);
    }

    s32 mask = getCoreButtons(g_ProControllerCoreKeybinds, buttons);

    if (config::use_left_stick) {
        if (buttons & HidNpadButton_StickLLeft)
            mask |= FF4Button_Left;
        if (buttons & HidNpadButton_StickLRight)
            mask |= FF4Button_Right;
        if (buttons & HidNpadButton_StickLUp)
            mask |= FF4Button_Up;
        if (buttons & HidNpadButton_StickLDown)
            mask |= FF4Button_Down;
    }

    if (config::use_right_stick) {
        if (buttons & HidNpadButton_StickRLeft)
            mask |= FF4Button_Left;
        if (buttons & HidNpadButton_StickRRight)
            mask |= FF4Button_Right;
        if (buttons & HidNpadButton_StickRUp)
            mask |= FF4Button_Up;
        if (buttons & HidNpadButton_StickRDown)
            mask |= FF4Button_Down;
    }

    return mask;
}

s32 getHeld(const PadState& pad) {
    u32 styleSet = padGetStyleSet(&pad);
    u64 kHeld    = padGetButtons(&pad);

    return getButtons(styleSet, kHeld);
}

s32 getDown(const PadState& pad) {
    u32 styleSet = padGetStyleSet(&pad);
    u64 kDown    = padGetButtonsDown(&pad);

    return getButtons(styleSet, kDown);
}

bool isModifierPressed = false;

HidNpadButton* getSpecialKeybinds(const PadState& pad) {
    u32 styleSet = padGetStyleSet(&pad);
    if (styleSet == HidNpadStyleTag_NpadJoyLeft) {
        if (config::joycon_hold_style == 0)
            return g_JoyLeftVerticalSpecialKeybinds;
        else
            return g_JoyLeftHorizontalSpecialKeybinds;
    } else if (styleSet == HidNpadStyleTag_NpadJoyRight) {
        if (config::joycon_hold_style == 0)
            return g_JoyRightVerticalSpecialKeybinds;
        else
            return g_JoyRightHorizontalSpecialKeybinds;
    } else {
        return g_ProControllerSpecialKeybinds;
    }
};

void handleSpecialKeys() {
    u64 kHeld = padGetButtons(&g_Pads[0]);
    u64 kDown = padGetButtonsDown(&g_Pads[0]);

    isModifierPressed = false;

    HidNpadButton* keybinds = getSpecialKeybinds(g_Pads[0]);

    if (kHeld & keybinds[SpecialButton_Modifier]) {
        isModifierPressed = true;

        if (kDown & keybinds[SpecialButton_HalfGameSpeed]) {
            bridge::halfFPSMultiplier();
        }
        if (kDown & keybinds[SpecialButton_DoubleGameSpeed]) {
            bridge::doubleFPSMultiplier();
        }
        if (kDown & keybinds[SpecialButton_OpenMenu]) {
            game::g_menuOpen = !game::g_menuOpen;
            if (game::g_menuOpen == true) {
                achievement::reset_progress_cache();
                ::menu::playMenuSE(4);
            } else {
                ::menu::close();
            }
        }
        if (kDown & keybinds[SpecialButton_ChocoboMenu]) {
            auto current_part = sys::GGlobal::getCurrentPart();
            if (current_part == BabilGamePart_WorldPart) {
                world::WorldStateScheduler::cls* wss = patches::game::wssInstance_;
                if (wss != nullptr) {
                    world::WorldState::cls* ws = world::WorldStateScheduler::wssGetCurrentState(wss);
                    if (ws != nullptr) {
                        const char* state_name = world::WorldState::wsGetString(ws);
                        if (strcmp(state_name, "world move") == 0) {
                            ::menu::playMenuSE(4);
                            babil::world::WorldStateScheduler::wssAddStateSchedule(wss, "chokobo_menu");
                            babil::world::WorldState::wsSetEnd(ws);
                        }
                    }
                }
            }
        }
        return;
    }
}

s32 getKeyEvent() {
    bool overlayOpen = game::g_menuOpen || game::g_playerSelectOpen;
    bool isPaused    = btl::BattlePart::instance_->m_IsPaused;

    if (overlayOpen) {
        if (!isPaused) {
            wasPausedByOverlay                     = true;
            btl::BattlePart::instance_->m_IsPaused = true;
        }
        return 0;
    } else {
        if (wasPausedByOverlay) {
            wasPausedByOverlay = false;
            if (isPaused) {
                btl::BattlePart::instance_->m_IsPaused = false;
            }
        }
    }
    HidNpadButton* keybinds = getSpecialKeybinds(g_Pads[0]);
    u64 kHeld               = padGetButtons(&g_Pads[0]);
    u64 kDown               = padGetButtonsDown(&g_Pads[0]);
    if (kHeld & keybinds[SpecialButton_BattlePause]) {
        if (isModifierPressed || !config::require_modifier_for_battle_pause) {
            auto current_part = sys::GGlobal::getCurrentPart();
            if (current_part == BabilGamePart_BattlePart) {
                if (kHeld & keybinds[SpecialButton_BattlePause]) {
                    game::g_UseFakeTouch = true;
                    game::g_FakeTouch[0] = 0.88f;
                    game::g_FakeTouch[1] = 0.92f;
                }

                // if (kDown & keybinds[SpecialButton_BattlePause]) {
                //     if (btl::BattlePart::instance_->m_IsPaused) {
                //         game::g_UseFakeTouch = true;
                //         game::g_FakeTouch[0] = 0.88f;
                //         game::g_FakeTouch[1] = 0.92f;
                //     } else {
                //         ds::snd::SEHandle::cls handle;
                //         ds::snd::SEHandle::Ctor(&handle);
                //         ds::snd::SEHandle::Play(&handle, 0, 9, 127, 0);
                //         ds::snd::SEHandle::Dtor(&handle);
                //         btl::BattlePart::instance_->m_ShouldPause = true;
                //     }
                // }
            }
        }
    }

    if (isModifierPressed)
        return 0;

    s32 mask = -1;
    if (config::multiplayer_enabled)
        mask = multiplayer::getKeyEvent();
    if (mask == -1) {
        mask = getHeld(g_Pads[0]);
        g_LockedUntilRelease[0] &= mask;
        mask &= ~g_LockedUntilRelease[0];
    }

    if (config::swap_in_vehicle) {
        auto current_part = sys::GGlobal::getCurrentPart();
        if (current_part == BabilGamePart_WorldPart) {
            world::WorldStateScheduler::cls* wss = patches::game::wssInstance_;
            if (wss != nullptr) {
                world::WorldState::cls* ws = world::WorldStateScheduler::wssGetCurrentState(wss);
                if (ws != nullptr) {
                    const char* state_name = world::WorldState::wsGetString(ws);
                    if (strcmp(state_name, "vehicle move") == 0) {
                        bool select = mask & FF4Button_Select;
                        bool cancel = mask & FF4Button_Cancel;
                        mask &= ~(FF4Button_Select | FF4Button_Cancel);
                        if (select)
                            mask |= FF4Button_Cancel;
                        if (cancel)
                            mask |= FF4Button_Select;
                    }
                }
            }
        }
    }

    return mask;
}

void lockAllUntilRelease() {
    for (s8 i = 0; i < MAX_PLAYERS; i++) {
        g_LockedUntilRelease[i] = FF4Button_Up | FF4Button_Down | FF4Button_Left | FF4Button_Right | FF4Button_Cancel | FF4Button_Select | FF4Button_Next | FF4Button_Menu | FF4Button_Prev | FF4Button_DebugA | FF4Button_DebugB | FF4Button_ChangeMainCharacter;
    }
}

struct ButtonNameMap
{
    const char* name;
    HidNpadButton value;
};

static const ButtonNameMap ButtonNames[] = {
    { "None", HidNpadButton_None },
    { "A", HidNpadButton_A },
    { "B", HidNpadButton_B },
    { "X", HidNpadButton_X },
    { "Y", HidNpadButton_Y },
    { "StickL", HidNpadButton_StickL },
    { "StickR", HidNpadButton_StickR },
    { "L", HidNpadButton_L },
    { "R", HidNpadButton_R },
    { "ZL", HidNpadButton_ZL },
    { "ZR", HidNpadButton_ZR },
    { "Plus", HidNpadButton_Plus },
    { "Minus", HidNpadButton_Minus },
    { "Left", HidNpadButton_Left },
    { "Up", HidNpadButton_Up },
    { "Right", HidNpadButton_Right },
    { "Down", HidNpadButton_Down },
    { "StickLLeft", HidNpadButton_StickLLeft },
    { "StickLUp", HidNpadButton_StickLUp },
    { "StickLRight", HidNpadButton_StickLRight },
    { "StickLDown", HidNpadButton_StickLDown },
    { "StickRLeft", HidNpadButton_StickRLeft },
    { "StickRUp", HidNpadButton_StickRUp },
    { "StickRRight", HidNpadButton_StickRRight },
    { "StickRDown", HidNpadButton_StickRDown },
    { "LeftSL", HidNpadButton_LeftSL },
    { "LeftSR", HidNpadButton_LeftSR },
    { "RightSL", HidNpadButton_RightSL },
    { "RightSR", HidNpadButton_RightSR }
};

constexpr const char* FF4ButtonNames[NUM_FF4_BUTTONS] = {
    "select",
    "cancel",
    "debug_a",
    "debug_b",
    "right",
    "left",
    "up",
    "down",
    "change_main_character",
    "next",
    "menu",
    "prev"
};

constexpr const char* SpecialButtonNames[NUM_SPECIAL_BUTTONS] = {
    "modifier",
    "half_game_speed",
    "double_game_speed",
    "battle_pause",
    "open_menu",
    "chocobo_menu"
};

struct ControllerSection
{
    const char* section_name;
    HidNpadButton* core_keybinds;
    HidNpadButton* special_keybinds;
};

static ControllerSection Sections[] = {
    { "pro_controller", g_ProControllerCoreKeybinds, g_ProControllerSpecialKeybinds },
    { "joycon_left_vertical", g_JoyLeftVerticalCoreKeybinds, g_JoyLeftVerticalSpecialKeybinds },
    { "joycon_left_horizontal", g_JoyLeftHorizontalCoreKeybinds, g_JoyLeftHorizontalSpecialKeybinds },
    { "joycon_right_vertical", g_JoyRightVerticalCoreKeybinds, g_JoyRightVerticalSpecialKeybinds },
    { "joycon_right_horizontal", g_JoyRightHorizontalCoreKeybinds, g_JoyRightHorizontalSpecialKeybinds },
};

static const char* button_to_string(HidNpadButton b) {
    for (const auto& m : ButtonNames) {
        if (m.value == b)
            return m.name;
    }
    return "None";
}

static HidNpadButton string_to_button(const std::string& s) {
    for (const auto& m : ButtonNames) {
        if (s == m.name)
            return m.value;
    }
    debugPrintf("Unknown button name in controls file: %s \n", s.c_str());
    return HidNpadButton_None;
}

static void apply_keybind_entry(const std::string& section, const std::string& key, const std::string& value) {
    for (auto& s : Sections) {
        if (section != s.section_name)
            continue;
        for (int i = 0; i < NUM_FF4_BUTTONS; i++) {
            if (key == FF4ButtonNames[i]) {
                s.core_keybinds[i] = string_to_button(value);
                return;
            }
        }
        for (int i = 0; i < NUM_SPECIAL_BUTTONS; i++) {
            if (key == SpecialButtonNames[i]) {
                s.special_keybinds[i] = string_to_button(value);
                return;
            }
        }
    }
}

void read_controls(const char* file) {
    std::ifstream in(file);
    if (!in.is_open()) {
        debugPrintf("Failed to open controls file: %s \n", file);
        return;
    }

    std::string section;
    std::string line;

    while (std::getline(in, line)) {
        line = stdext::trim(line);

        if (line.empty() || line[0] == ';' || line[0] == '#')
            continue;

        if (line.front() == '[' && line.back() == ']') {
            section = line.substr(1, line.size() - 2);
            continue;
        }

        const auto eq = line.find('=');
        if (eq == std::string::npos)
            continue;

        const std::string key   = stdext::trim(line.substr(0, eq));
        const std::string value = stdext::trim(line.substr(eq + 1));

        apply_keybind_entry(section, key, value);
    }
}

void mark_for_write() {
    marked_for_write = true;
}

void write_controls() {
    marked_for_write = false;
    std::ofstream out(CONTROLS_PATH);
    if (!out.is_open()) {
        debugPrintf("Failed to open controls file for writing: %s \n", CONTROLS_PATH);
        return;
    }

    for (size_t s = 0; s < std::size(Sections); s++) {
        if (s > 0)
            out << "\n";
        out << "[" << Sections[s].section_name << "]\n";
        for (int i = 0; i < NUM_FF4_BUTTONS; i++) {
            out << FF4ButtonNames[i] << " = " << button_to_string(Sections[s].core_keybinds[i]) << "\n";
        }
        for (int i = 0; i < NUM_SPECIAL_BUTTONS; i++) {
            out << SpecialButtonNames[i] << " = " << button_to_string(Sections[s].special_keybinds[i]) << "\n";
        }
    }

    out.close();

    if (!config::user_save_separation)
        return;

    const std::string dest = std::string(config::save_filename) + "/controls.ini";

    std::ifstream src(CONTROLS_PATH, std::ios::binary);
    std::ofstream dst(dest, std::ios::binary);
    if (!src.is_open() || !dst.is_open()) {
        debugPrintf("Failed to copy controls to save folder: %s \n", dest.c_str());
        return;
    }
    dst << src.rdbuf();
}
}
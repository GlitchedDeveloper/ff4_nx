#pragma once

#include <switch.h>

#include <unordered_map>
#include <vector>

#include "types.h"

namespace pad_manager {

constexpr int NUM_FF4_BUTTONS     = 12;
constexpr int NUM_SPECIAL_BUTTONS = 6;

enum FF4Button : u32
{
    FF4Button_Select              = BIT(0),
    FF4Button_Cancel              = BIT(1),
    FF4Button_DebugA              = BIT(2),
    FF4Button_DebugB              = BIT(3),
    FF4Button_Right               = BIT(4),
    FF4Button_Left                = BIT(5),
    FF4Button_Up                  = BIT(6),
    FF4Button_Down                = BIT(7),
    FF4Button_Next                = BIT(8),
    FF4Button_Prev                = BIT(9),
    FF4Button_Menu                = BIT(10),
    FF4Button_ChangeMainCharacter = BIT(11)
};

enum SpecialButton : u32
{
    SpecialButton_Modifier,
    SpecialButton_HalfGameSpeed,
    SpecialButton_DoubleGameSpeed,
    SpecialButton_BattlePause,
    SpecialButton_OpenMenu,
    SpecialButton_ChocoboMenu,
};

constexpr int MAX_PLAYERS = 5;
extern PadState g_Pads[MAX_PLAYERS];
extern int g_CurrentPad;
extern s8 g_ConnectedPads;
extern s32 g_LockedUntilRelease[MAX_PLAYERS];
void init();
void exit();
void update();
void handleSpecialKeys();
s32 getKeyEvent();
s32 getHeld(const PadState& pad);
s32 getDown(const PadState& pad);
void lockAllUntilRelease();
void setCoreKeybind(HidNpadButton keybinds[NUM_FF4_BUTTONS], u32 ff4_button, HidNpadButton button);
HidNpadButton* getSpecialKeybinds(const PadState& pad);
void setSpecialKeybind(HidNpadButton keybinds[NUM_SPECIAL_BUTTONS], u32 special_button, HidNpadButton button);

constexpr HidNpadButton HidNpadButton_None = (HidNpadButton)0;

extern HidNpadButton g_ProControllerCoreKeybinds[NUM_FF4_BUTTONS];
extern HidNpadButton g_ProControllerSpecialKeybinds[NUM_SPECIAL_BUTTONS];
extern HidNpadButton g_JoyLeftHorizontalCoreKeybinds[NUM_FF4_BUTTONS];
extern HidNpadButton g_JoyLeftHorizontalSpecialKeybinds[NUM_SPECIAL_BUTTONS];
extern HidNpadButton g_JoyLeftVerticalCoreKeybinds[NUM_FF4_BUTTONS];
extern HidNpadButton g_JoyLeftVerticalSpecialKeybinds[NUM_SPECIAL_BUTTONS];
extern HidNpadButton g_JoyRightHorizontalCoreKeybinds[NUM_FF4_BUTTONS];
extern HidNpadButton g_JoyRightHorizontalSpecialKeybinds[NUM_SPECIAL_BUTTONS];
extern HidNpadButton g_JoyRightVerticalCoreKeybinds[NUM_FF4_BUTTONS];
extern HidNpadButton g_JoyRightVerticalSpecialKeybinds[NUM_SPECIAL_BUTTONS];

extern bool marked_for_write;

void read_controls(const char* file);
void mark_for_write();
void write_controls();
}
#include "special.h"

#include "../../../../pad_manager.h"
#include "../../../elements/keybind.h"

using namespace pad_manager;

namespace menu::tabs::controls {
using namespace elements;

struct KeybindOption
{
    const char* name;
    const SpecialButton button;
    const KeybindFlags flags = KeybindFlags_None;
};

constexpr KeybindOption Options[] = {
    { "Modifier", SpecialButton_Modifier },
    { "Open Menu", SpecialButton_OpenMenu },
    { "Half Game Speed", SpecialButton_HalfGameSpeed, KeybindFlags_AllowUnbind },
    { "Double Game Speed", SpecialButton_DoubleGameSpeed, KeybindFlags_AllowUnbind },
    { "Pause (Battle)", SpecialButton_BattlePause, KeybindFlags_AllowUnbind },
    { "Chocobo Menu (World)", SpecialButton_ChocoboMenu, KeybindFlags_AllowUnbind },
};

Special::Special(HidNpadButton keybinds[NUM_SPECIAL_BUTTONS])
    : Tab("Special") {
    for (auto& option : Options) {
        m_Elements.push_back(new Keybind(option.name, (u64*)&keybinds[option.button], [keybinds, option](u64 value) {
            setSpecialKeybind(keybinds, option.button, (HidNpadButton)value);
            mark_for_write(); }, option.flags));
    }
};
}
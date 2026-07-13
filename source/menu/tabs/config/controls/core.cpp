#include "core.h"

#include "../../../../pad_manager.h"
#include "../../../elements/keybind.h"

using namespace pad_manager;

namespace menu::tabs::controls {
using namespace elements;

struct KeybindOption
{
    const char* name;
    const FF4Button button;
    const KeybindFlags flags = KeybindFlags_None;
};

constexpr KeybindOption Options[] = {
    { "Select", FF4Button_Select },
    { "Cancel", FF4Button_Cancel },
    { "Left", FF4Button_Left },
    { "Right", FF4Button_Right },
    { "Up", FF4Button_Up },
    { "Down", FF4Button_Down },
    { "Prev page - Change targeted party", FF4Button_Prev },
    { "Map - Run away - Next page", FF4Button_Next },
    { "Menu - Skip character (battles)", FF4Button_Menu },
    { "Change main character - Enable auto battle", FF4Button_ChangeMainCharacter },
    { "Debug A", FF4Button_DebugA, KeybindFlags_AllowUnbind },
    { "Debug B", FF4Button_DebugB, KeybindFlags_AllowUnbind },
};

Core::Core(HidNpadButton keybinds[NUM_FF4_BUTTONS], bool isProController)
    : Tab("Core") {
    for (auto& option : Options) {
        if (!isProController && option.button & (FF4Button_Left | FF4Button_Right | FF4Button_Up | FF4Button_Down))
            continue;
        m_Elements.push_back(new Keybind(option.name, (u64*)&keybinds[std::__countr_zero((u32)option.button)], [keybinds, option](u64 value) {
            setCoreKeybind(keybinds, option.button, (HidNpadButton)value);
            mark_for_write(); }, option.flags));
    }
};
}

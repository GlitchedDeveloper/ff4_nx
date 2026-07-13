#include "joyconcontrols.h"

#include "core.h"
#include "special.h"

namespace menu::tabs::controls {
JoyConControls::JoyConControls(const char* name, HidNpadButton core_keybinds[pad_manager::NUM_FF4_BUTTONS], HidNpadButton special_keybinds[pad_manager::NUM_SPECIAL_BUTTONS])
    : TabContainer(name) {
    m_Elements.push_back(new Core(core_keybinds, false));
    m_Elements.push_back(new Special(special_keybinds));
};
}

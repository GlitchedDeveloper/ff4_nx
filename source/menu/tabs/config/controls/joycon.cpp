#include "joycon.h"

#include "../../../../pad_manager.h"
#include "core.h"
#include "joyconcontrols.h"
#include "special.h"

namespace menu::tabs::controls {
JoyCon::JoyCon(const char* name, HidNpadButton vertical_core_keybinds[pad_manager::NUM_FF4_BUTTONS], HidNpadButton vertical_special_keybinds[pad_manager::NUM_SPECIAL_BUTTONS], HidNpadButton horizontal_core_keybinds[pad_manager::NUM_FF4_BUTTONS], HidNpadButton horizontal_special_keybinds[pad_manager::NUM_SPECIAL_BUTTONS])
    : TabContainer(name) {
    m_Elements.push_back(new JoyConControls("Vertical", vertical_core_keybinds, vertical_special_keybinds));
    m_Elements.push_back(new JoyConControls("Horizontal", horizontal_core_keybinds, horizontal_special_keybinds));
};
}
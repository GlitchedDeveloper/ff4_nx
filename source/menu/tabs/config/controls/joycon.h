#pragma once

#include "../../../../pad_manager.h"
#include "../../../elements/tab_container.h"

namespace menu::tabs::controls {
class JoyCon : public elements::TabContainer
{
  public:
    JoyCon(const char* name, HidNpadButton vertical_core_keybinds[pad_manager::NUM_FF4_BUTTONS], HidNpadButton vertical_special_keybinds[pad_manager::NUM_SPECIAL_BUTTONS], HidNpadButton horizontal_core_keybinds[pad_manager::NUM_FF4_BUTTONS], HidNpadButton horizontal_special_keybinds[pad_manager::NUM_SPECIAL_BUTTONS]);
};
}
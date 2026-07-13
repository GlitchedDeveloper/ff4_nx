#pragma once

#include "../../../../pad_manager.h"
#include "../../../elements/tab_container.h"
namespace menu::tabs::controls {
class JoyConControls : public elements::TabContainer
{
  public:
    JoyConControls(const char* name, HidNpadButton core_keybinds[pad_manager::NUM_FF4_BUTTONS], HidNpadButton special_keybinds[pad_manager::NUM_SPECIAL_BUTTONS]);
};
}

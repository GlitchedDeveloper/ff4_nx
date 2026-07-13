#pragma once

#include <switch.h>

#include "../../../../pad_manager.h"
#include "../../../elements/container.h"
#include "../../../tab.h"

namespace menu::tabs::controls {
class Core : public elements::Container, public Tab
{
  public:
    Core(HidNpadButton keybinds[pad_manager::NUM_FF4_BUTTONS], bool isProController);
};
}

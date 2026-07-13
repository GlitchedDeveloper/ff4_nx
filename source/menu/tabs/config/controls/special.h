#pragma once

#include <switch.h>

#include "../../../../pad_manager.h"
#include "../../../elements/container.h"
#include "../../../tab.h"

namespace menu::tabs::controls {
class Special : public elements::Container, public Tab
{
  public:
    Special(HidNpadButton keybinds[pad_manager::NUM_SPECIAL_BUTTONS]);
};
}

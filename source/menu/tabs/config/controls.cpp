#include "controls.h"

#include "../../../pad_manager.h"
#include "controls/joycon.h"
#include "controls/pro.h"
#include "controls/settings.h"

namespace menu::tabs {
using namespace controls;
Controls::Controls()
    : TabContainer("Controls") {
    m_Elements.push_back(new Settings());
    m_Elements.push_back(new Pro());
    m_Elements.push_back(new JoyCon("Left Joy-Con", pad_manager::g_JoyLeftVerticalCoreKeybinds, pad_manager::g_JoyLeftVerticalSpecialKeybinds, pad_manager::g_JoyLeftHorizontalCoreKeybinds, pad_manager::g_JoyLeftHorizontalSpecialKeybinds));
    m_Elements.push_back(new JoyCon("Right Joy-Con", pad_manager::g_JoyRightVerticalCoreKeybinds, pad_manager::g_JoyRightVerticalSpecialKeybinds, pad_manager::g_JoyRightHorizontalCoreKeybinds, pad_manager::g_JoyRightHorizontalSpecialKeybinds));
};
}


#include "pro.h"

#include "../../../../pad_manager.h"
#include "core.h"
#include "pro_general.h"
#include "special.h"

namespace menu::tabs::controls {
Pro::Pro()
    : TabContainer("Pro Controller") {
    m_Elements.push_back(new General());
    m_Elements.push_back(new Core(pad_manager::g_ProControllerCoreKeybinds, true));
    m_Elements.push_back(new Special(pad_manager::g_ProControllerSpecialKeybinds));
};
}
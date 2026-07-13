#include "debug.h"

#include "debug/battle.h"

namespace menu::tabs::gameplay {
using namespace debug;
Debug::Debug()
    : TabContainer("Debug") {
    m_Elements.push_back(new Battle());
};
}

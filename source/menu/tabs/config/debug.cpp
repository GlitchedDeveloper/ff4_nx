#include "debug.h"

#include "../../../config.h"
#include "../../elements/boolean.h"

namespace menu::tabs {
using namespace elements;

Debug::Debug()
    : Tab("Debug") {
    m_Elements.push_back(new Boolean("Log to File", config::log_to_file, [](bool value) {
        config::log_to_file = value;
        config::mark_for_write();
    }));
};
}

#include "pro_general.h"

#include "../../../../config.h"
#include "../../../elements/boolean.h"

namespace menu::tabs::controls {
using namespace elements;

General::General()
    : Tab("General") {
    m_Elements.push_back(new Boolean("Use Left Stick", config::use_left_stick, [](bool value) {
        config::use_left_stick = value;
        config::mark_for_write();
    }));
    m_Elements.push_back(new Boolean("Use Right Stick", config::use_left_stick, [](bool value) {
        config::use_right_stick = value;
        config::mark_for_write();
    }));
};
}

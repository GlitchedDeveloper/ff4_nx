#include "misc.h"

#include "../../../../config.h"
#include "../../../elements/boolean.h"

namespace menu::tabs::gameplay {
using namespace elements;

Misc::Misc()
    : Tab("Misc") {
    m_Elements.push_back(new Boolean("Remove Gil Limit", config::remove_gil_limit, [](bool value) {
        config::remove_gil_limit = value;
        config::mark_for_write();
    }));
};
}
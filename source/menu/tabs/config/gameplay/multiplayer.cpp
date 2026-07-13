#include "multiplayer.h"

#include "../../../../config.h"
#include "../../../elements/boolean.h"

namespace menu::tabs::gameplay {
using namespace elements;

Multiplayer::Multiplayer()
    : Tab("Multiplayer") {
    m_Elements.push_back(new Boolean("Multiplayer", config::multiplayer_enabled, [](bool value) {
        config::multiplayer_enabled = value;
        config::mark_for_write();
    }));
};
}
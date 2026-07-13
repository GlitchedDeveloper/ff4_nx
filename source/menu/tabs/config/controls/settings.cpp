#include "settings.h"

#include "../../../../config.h"
#include "../../../elements/boolean.h"
#include "../../../elements/float.h"

namespace menu::tabs::controls {
using namespace elements;

Settings::Settings()
    : Tab("Settings") {
    m_Elements.push_back(new Float("World Rumble Intensity", 0.0f, 1.0f, 0.025f, config::world_rumble_intensity, [](float value) {
        config::world_rumble_intensity = value;
        config::mark_for_write();
    }));
    m_Elements.push_back(new Float("Battle Rumble Intensity", 0.0f, 1.0f, 0.025f, config::battle_rumble_intensity, [](float value) {
        config::battle_rumble_intensity = value;
        config::mark_for_write();
    }));
    m_Elements.push_back(new Boolean("Swap [Select] and [Cancel] while in vehicle", config::swap_in_vehicle, [](bool value) {
        config::swap_in_vehicle = value;
        config::mark_for_write();
    }));
    m_Elements.push_back(new Boolean("Require [Modifier] for [Pause (Battle)]", config::require_modifier_for_battle_pause, [](bool value) {
        config::require_modifier_for_battle_pause = value;
        config::mark_for_write();
    }));
};
}
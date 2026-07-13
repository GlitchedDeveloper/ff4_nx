#include "boosts.h"

#include "../../../../config.h"
#include "../../../../game.h"
#include "../../../elements/boolean.h"
#include "../../../elements/option.h"

namespace menu::tabs::gameplay {
using namespace elements;

int floatToMultiplier(float value) {
    if (value == 4.0f)
        return 3;
    if (value == 2.0f)
        return 2;
    if (value == 0.5f)
        return 0;
    return 1;
}
float multiplierToFloat(int value) {
    switch (value) {
        case 0:
            return 0.5f;
        case 2:
            return 2.0f;
        case 3:
            return 4.0f;
        default:
            return 1.0f;
    }
}

Boosts::Boosts()
    : Tab("Boosts") {
    m_Elements.push_back(new Boolean("Encounters", game::g_encountersEnabled, [](bool value) {
        if (!value)
            config::guaranteed_encounter = false;
        game::g_encountersEnabled = value;
        config::mark_for_write();
    }));
    const char* multipliers[] = { "0.5x", "1x", "2x", "4x" };
    m_Elements.push_back(new Option("EXP Multiplier", multipliers, std::size(multipliers), floatToMultiplier(game::g_expMultiplier), [](int value) {
        game::g_expMultiplier = multiplierToFloat(value);
        config::mark_for_write();
    }));
    m_Elements.push_back(new Option("Gil Multiplier", multipliers, std::size(multipliers), floatToMultiplier(game::g_gilMultiplier), [](int value) {
        game::g_gilMultiplier = multiplierToFloat(value);
        config::mark_for_write();
    }));
};
}
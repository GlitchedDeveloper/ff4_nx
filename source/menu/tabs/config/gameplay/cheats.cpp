#include "cheats.h"

#include "../../../../config.h"
#include "../../../../game.h"
#include "../../../elements/boolean.h"

namespace menu::tabs::gameplay {
using namespace elements;

Cheats::Cheats()
    : Tab("Cheats") {
    m_Elements.push_back(new Boolean("Save Anywhere", game::g_saveAnywhere, [](bool value) {
        game::g_saveAnywhere = value;
        config::mark_for_write();
    }));
    m_Elements.push_back(new Boolean("Remove NG+ Limit", config::remove_newgameplus_limit, [](bool value) {
        config::remove_newgameplus_limit = value;
        config::mark_for_write();
    }));
    m_Elements.push_back(new Boolean("Guaranteed Encounter", config::guaranteed_encounter, [](bool value) {
        if (value)
            game::g_encountersEnabled = true;
        config::guaranteed_encounter = value;
        config::mark_for_write();
    }));
    m_Elements.push_back(new Boolean("Equip Anything", config::equip_anything, [](bool value) {
        config::equip_anything = value;
        config::mark_for_write();
    }));
    m_Elements.push_back(new Boolean("Reobtainable Limit Break", config::reobtainable_limit_break, [](bool value) {
        config::reobtainable_limit_break = value;
        config::mark_for_write();
    }));
    m_Elements.push_back(new Boolean("Augmentless Stat Growth", config::augmentless_stat_growth, [](bool value) {
        config::augmentless_stat_growth = value;
        config::mark_for_write();
    }));
};
}
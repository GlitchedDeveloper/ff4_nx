#include "overlay.h"

#include "../../../../config.h"
#include "../../../elements/boolean.h"

namespace menu::tabs::visuals {
using namespace elements;

Overlay::Overlay()
    : Tab("Overlay") {
    m_Elements.push_back(new Boolean("Show FPS", config::show_fps, [](bool value) {
        config::show_fps = value;
        config::mark_for_write();
    }));
    m_Elements.push_back(new Boolean("Show Frame Time Graph", config::show_frame_graph, [](bool value) {
        config::show_frame_graph = value;
        config::mark_for_write();
    }));
    m_Elements.push_back(new Boolean("Show UPS", config::show_ups, [](bool value) {
        config::show_ups = value;
        config::mark_for_write();
    }));
    m_Elements.push_back(new Boolean("Show Update Time Graph", config::show_update_graph, [](bool value) {
        config::show_update_graph = value;
        config::mark_for_write();
    }));
    m_Elements.push_back(new Boolean("Show Achievement Popup", config::show_achievement_popup, [](bool value) {
        config::show_achievement_popup = value;
        config::mark_for_write();
    }));
    m_Elements.push_back(new Boolean("Show Game Speed Multiplier", config::show_game_speed_mult, [](bool value) {
        config::show_game_speed_mult = value;
        config::mark_for_write();
    }));
};
}
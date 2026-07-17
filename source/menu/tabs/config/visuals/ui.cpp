#include "ui.h"

#include "../../../../config.h"
#include "../../../elements/boolean.h"

namespace menu::tabs::visuals {
using namespace elements;

UI::UI()
    : Tab("User Interface") {
    m_Elements.push_back(new Boolean("Clean Title Screen", config::clean_title, [](bool value) {
        config::clean_title = value;
        config::mark_for_write();
    }));

    m_Elements.push_back(new Boolean("Remove Menu Button", config::remove_menu_button, [](bool value) {
        config::remove_menu_button = value;
        config::mark_for_write();
    }));

    m_Elements.push_back(new Boolean("Remove Map Button", config::remove_map_button, [](bool value) {
        config::remove_map_button = value;
        config::mark_for_write();
    }));

    m_Elements.push_back(new Boolean("Remove Back Button", config::remove_back_button, [](bool value) {
        config::remove_back_button = value;
        config::mark_for_write();
    }));

    m_Elements.push_back(new Boolean("Remove Optimize Button", config::remove_optimize_button, [](bool value) {
        config::remove_optimize_button = value;
        config::mark_for_write();
    }));

    m_Elements.push_back(new Boolean("Remove Remove Button", config::remove_remove_button, [](bool value) {
        config::remove_remove_button = value;
        config::mark_for_write();
    }));

    m_Elements.push_back(new Boolean("Remove Abilities Button", config::remove_abilities_button, [](bool value) {
        config::remove_abilities_button = value;
        config::mark_for_write();
    }));

    m_Elements.push_back(new Boolean("Remove Invert Button", config::remove_invert_button, [](bool value) {
        config::remove_invert_button = value;
        config::mark_for_write();
    }));

    m_Elements.push_back(new Boolean("Remove Skip Button", config::remove_skip_button, [](bool value) {
        config::remove_skip_button = value;
        config::mark_for_write();
    }));

    m_Elements.push_back(new Boolean("Remove Disembark Button", config::remove_disembark_button, [](bool value) {
        config::remove_disembark_button = value;
        config::mark_for_write();
    }));

    m_Elements.push_back(new Boolean("Remove Sort Button", config::remove_sort_button, [](bool value) {
        config::remove_sort_button = value;
        config::mark_for_write();
    }));
};
}
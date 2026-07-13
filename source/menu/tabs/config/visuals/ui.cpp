#include "ui.h"

#include "../../../../config.h"
#include "../../../elements/boolean.h"

namespace menu::tabs::visuals {
using namespace elements;

UI::UI()
    : Tab("User Interface") {
    m_Elements.push_back(new Boolean("Hide Back Button", config::hide_back_button, [](bool value) {
        config::hide_back_button = value;
        config::mark_for_write();
    }));
    m_Elements.push_back(new Boolean("Hide Vehicle Buttons", config::hide_vehicle_buttons, [](bool value) {
        config::hide_vehicle_buttons = value;
        config::mark_for_write();
    }));
};
}
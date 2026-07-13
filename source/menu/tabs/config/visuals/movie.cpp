#include "movie.h"

#include "../../../../config.h"
#include "../../../../imgui/imgui.h"
#include "../../../elements/boolean.h"

namespace menu::tabs::visuals {
using namespace elements;

Movie::Movie()
    : Tab("Movie") {
    m_Elements.push_back(new Boolean("Overclock CPU", config::overclock_movie_cpu, [](bool value) {
        config::overclock_movie_cpu = value;
        config::mark_for_write();
    }));
    m_Elements.push_back(new Boolean("Overclock MEM", config::overclock_movie_mem, [](bool value) {
        config::overclock_movie_mem = value;
        config::mark_for_write();
    }));
    m_Elements.push_back(new Boolean("Limit to 24fps", config::limit_movie_fps, [](bool value) {
        config::limit_movie_fps = value;
        config::mark_for_write();
    }));
};

void Movie::update() {
    ImGui::Text("The following settings will only be in effect while the opening movie is playing");
    ImGui::Separator();
    Container::update();
}
}
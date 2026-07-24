#include "launch.h"

#include <sys/dirent.h>

#include "../../game.h"
#include "../../imgui/imgui.h"
#include "../../pad_manager.h"

using namespace pad_manager;

namespace menu::tabs {
using namespace elements;

Launch::Launch()
    : Tab("Launch") {
    };

void Launch::update() {
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.0f, 1.0f));
    ImGui::Text("Press [SELECT] to launch Final Fantasy IV");
    ImGui::PopStyleColor();
}

bool Launch::down(u32 key) {
    if (key == FF4Button_Select) {
        game::launch();
        return true;
    }
    return false;
}
}
#include "fonts.h"

#include <sys/dirent.h>

#include <cstring>

#include "../../bridge.h"
#include "../../config.h"
#include "../../game.h"
#include "../../imgui/imgui.h"
#include "../../imgui/imgui_impl_gles1.h"
#include "../../pad_manager.h"

using namespace pad_manager;

namespace menu::tabs {
using namespace elements;

Fonts::Fonts()
    : Tab("Fonts") {
    DIR* dir = opendir(FONTS_DIR);
    if (dir) {
        struct dirent* ent;
        while ((ent = readdir(dir)) != NULL) {
            if (strstr(ent->d_name, ".ttf") != nullptr) {
                m_Fonts.push_back(ent->d_name);
            }
        }
        closedir(dir);
    }

    for (size_t i = 0; i < m_Fonts.size(); i++) {
        if (strcmp(config::font_filename, m_Fonts[i].c_str()) == 0) {
            m_Selected = m_CurrentSelected = i;
            break;
        }
    }
};

void Fonts::update() {
    for (size_t i = 0; i < m_Fonts.size(); i++) {
        bool checked = m_Selected == i;
        ImVec4 color;
        if (m_CurrentElement == i) {
            color = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);

            if (m_UpdateScroll) {
                ImGui::SetScrollHereY(0.5f);
                m_UpdateScroll = false;
            }
        } else {
            color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
        }
        ImVec4 baseColor = ImGui::GetStyleColorVec4(ImGuiCol_FrameBg);
        ImGui::PushStyleColor(ImGuiCol_CheckboxSelectedBg, baseColor);
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, baseColor);
        ImGui::PushStyleColor(ImGuiCol_FrameBgActive, baseColor);
        ImGui::PushStyleColor(ImGuiCol_Text, color);
        if (ImGui::Checkbox(m_Fonts[i].c_str(), &checked)) {
            if (checked) {
                m_Selected = i;
            }
        }
        ImGui::PopStyleColor(4);
    }
}

void Fonts::postUpdate() {
    if (m_Selected != m_CurrentSelected) {
        ImGuiIO& io = ImGui::GetIO();
        io.Fonts->Clear();

        std::string path = FONTS_DIR + m_Fonts[m_Selected];
        io.Fonts->AddFontFromFileTTF(path.c_str(), 32.0f);
        io.Fonts->Build();

        ImGui_ImplGLES1_CreateFontsTexture();

        strcpy(config::font_filename, m_Fonts[m_Selected].c_str());

        if (!game::g_Launched) {
            bridge::reinitFont();
        }

        m_CurrentSelected = m_Selected;

        config::write_config();
    }
}

bool Fonts::down(u32 key) {
    if (key == FF4Button_Up) {
        if (m_CurrentElement > 0) {
            m_CurrentElement--;
            m_UpdateScroll = true;
            return true;
        }
        return false;
    }
    if (key == FF4Button_Down) {
        if (m_CurrentElement < m_Fonts.size() - 1) {
            m_CurrentElement++;
            m_UpdateScroll = true;
            return true;
        }
        return false;
    }
    if (key == FF4Button_Select) {
        if (m_Selected != m_CurrentElement) {
            m_Selected = m_CurrentElement;
        }
        return true;
    }
    return false;
}
}
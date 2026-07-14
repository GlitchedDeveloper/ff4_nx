#include "fonts.h"

#include <cstring>

#include "../../bridge.h"
#include "../../config.h"
#include "../../game.h"
#include "../../imgui/imgui.h"
#include "../../imgui/imgui_impl_gles1.h"
#include "../../pad_manager.h"
#include "../elements/boolean.h"

using namespace pad_manager;

namespace menu::tabs {
using namespace elements;

Fonts::Fonts()
    : Tab("Fonts") {
    for (size_t i = 0; i < bridge::fonts.size(); i++) {
        bridge::FontFile& ff = bridge::fonts[i];
        const char* display  = ff.path.c_str() + sizeof(FONTS_DIR) - 1;
        m_Elements.push_back(new Boolean(
            display,
            ff.enabled,
            [this, &ff](bool value) {
                ff.enabled = value;
                m_Dirty    = true;
                config::mark_for_write();
            }));
        m_FontIndices.push_back(i);
    }
};

void Fonts::update() {
    if (m_CurrentElement < 0)
        m_CurrentElement = 0;
    if (m_CurrentElement >= m_Elements.size())
        m_CurrentElement = m_Elements.size() - 1;
    for (size_t i = 0; i < m_Elements.size(); i++) {
        Element* element = m_Elements[i];
        bool focused     = i == m_CurrentElement;
        element->setFocused(focused);
        if (focused && m_IsReordering) {
            ImGui::Indent();
        }
        if (focused)
            ImGui::SetScrollHereY();
        element->update();
        if (focused && m_IsReordering) {
            ImGui::Unindent();
        }
    }
}

void Fonts::unfocus() {
    if (!m_Dirty)
        return;
    m_Dirty = false;

    game::g_ShouldRebuildFonts = true;
}

void Fonts::rebuildImGuiAtlas() {
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->Clear();

    bool first = true;
    for (const bridge::FontFile& ff : bridge::fonts) {
        if (!ff.enabled)
            continue;
        ImFontConfig cfg;
        cfg.ExtraSizeScale = bridge::getFontEmScaleCorrection(ff.path.c_str()) * 2.0f / 3.0f;
        cfg.MergeMode      = !first;
        io.Fonts->AddFontFromFileTTF(ff.path.c_str(), IMGUI_FONT_SIZE * game::IMGUI_SCALE, &cfg);
        first = false;
    }
    if (first)
        io.Fonts->AddFontDefault();

    io.Fonts->Build();
    ImGui_ImplGLES1_CreateFontsTexture();

    if (!game::g_Launched) {
        bridge::reinitFont();
    }
}

bool Fonts::down(u32 key) {
    if (key == FF4Button_Select) {
        m_IsReordering = !m_IsReordering;
        if (!m_IsReordering) {
            m_Dirty = true;
            config::mark_for_write();
        }
        return true;
    }
    if (key == FF4Button_Cancel) {
        if (m_IsReordering) {
            m_IsReordering = false;
            config::mark_for_write();
            return true;
        }
        return false;
    }

    if (m_IsReordering) {
        if (key == FF4Button_Up) {
            if (m_CurrentElement > 0) {
                swap(m_CurrentElement, m_CurrentElement - 1);
                m_CurrentElement--;
            }
            return true;
        }
        if (key == FF4Button_Down) {
            if (m_CurrentElement < m_Elements.size() - 1) {
                swap(m_CurrentElement, m_CurrentElement + 1);
                m_CurrentElement++;
            }
            return true;
        }
        if (key == FF4Button_Left || key == FF4Button_Right || key == FF4Button_Prev || key == FF4Button_Next)
            return true;
        return false;
    }

    return Container::down(key);
}

void Fonts::swap(size_t from, size_t to) {
    std::swap(m_Elements[from], m_Elements[to]);
    std::swap(m_FontIndices[from], m_FontIndices[to]);

    size_t a = m_FontIndices[from];
    size_t b = m_FontIndices[to];
    std::swap(bridge::fonts[a], bridge::fonts[b]);

    std::swap(m_FontIndices[from], m_FontIndices[to]);
}
}

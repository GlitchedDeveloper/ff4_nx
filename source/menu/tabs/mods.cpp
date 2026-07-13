#include "mods.h"

#include "../../bridge.h"
#include "../../config.h"
#include "../../imgui/imgui.h"
#include "../../pad_manager.h"
#include "../elements/boolean.h"

using namespace pad_manager;

namespace menu::tabs {
using namespace elements;

Mods::Mods()
    : Tab("Mods") {
    for (size_t i = 0; i < bridge::OBBs.size(); i++) {
        bridge::OBBArchive& obb = bridge::OBBs[i];
        if (obb.path != OBB_FILE) {
            m_Elements.push_back(new Boolean(
                obb.path.c_str() + sizeof(OBB_MODS) - 1,
                obb.enabled,
                [&obb](bool value) {
                    obb.enabled = value;
                    config::mark_for_write();
                }));
            m_ObbIndices.push_back(i);
        }
    }
};

void Mods::update() {
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

bool Mods::down(u32 key) {
    if (key == FF4Button_Select) {
        m_IsReordering = !m_IsReordering;
        if (!m_IsReordering) {
            bridge::buildFileMap();
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

void Mods::swap(size_t from, size_t to) {
    std::swap(m_Elements[from], m_Elements[to]);
    std::swap(m_ObbIndices[from], m_ObbIndices[to]);

    size_t a = m_ObbIndices[from];
    size_t b = m_ObbIndices[to];
    std::swap(bridge::OBBs[a], bridge::OBBs[b]);

    std::swap(m_ObbIndices[from], m_ObbIndices[to]);
}
}
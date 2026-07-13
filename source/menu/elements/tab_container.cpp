#include "tab_container.h"

#include <vector>

#include "../../imgui/imgui.h"
#include "../../pad_manager.h"
#include "../menu.h"

namespace menu::elements {

TabContainer::TabContainer(const char* name)
    : Tab(name) { };

TabContainer::~TabContainer() { }

void TabContainer::update() {
    ImGui::PushID(m_Name.c_str());
    if (ImGui::BeginTabBar("TabBar")) {
        for (size_t i = 0; i < m_Elements.size(); i++) {
            Tab* tab                = dynamic_cast<Tab*>(m_Elements[i]);
            ImGuiTabItemFlags flags = ImGuiTabItemFlags_None;
            ImVec4 color;
            if (m_CurrentElement == i) {
                flags |= ImGuiTabItemFlags_SetSelected;
                color = ImGui::GetStyleColorVec4(ImGuiCol_TabHovered);
            } else {
                color = ImGui::GetStyleColorVec4(ImGuiCol_Tab);
            }
            ImGui::PushStyleColor(ImGuiCol_Tab, color);
            ImGui::PushStyleColor(ImGuiCol_TabHovered, color);
            ImGui::PushStyleColor(ImGuiCol_TabActive, color);
            bool tab_open = ImGui::BeginTabItem(tab->m_Name.c_str(), nullptr, flags);
            ImGui::PopStyleColor(3);
            if (ImGui::IsItemClicked()) {
                m_CurrentElement = i;
            }
            if (tab_open) {
                if (ImGui::BeginChild("Tab Child", ImVec2(0.0f, 0.0f), ImGuiChildFlags_None)) {
                    tab->setFocused(i == m_CurrentElement);
                    tab->update();
                }
                ImGui::EndChild();
                ImGui::EndTabItem();
            }
        }
        ImGui::EndTabBar();
    }
    ImGui::PopID();
}
bool TabContainer::down(u32 key) {
    if (key == pad_manager::FF4Button_Next) {
        if (m_CurrentElement < m_Elements.size() && m_Elements[m_CurrentElement]->down(key))
            return true;
        if (m_CurrentElement < m_Elements.size() - 1) {
            m_CurrentElement++;
            playMenuSE(3);
            return true;
        }
        return false;
    }
    if (key == pad_manager::FF4Button_Prev) {
        if (m_CurrentElement < m_Elements.size() && m_Elements[m_CurrentElement]->down(key))
            return true;
        if (m_CurrentElement > 0) {
            m_CurrentElement--;
            playMenuSE(3);
            return true;
        }
        return false;
    }
    if (m_CurrentElement >= m_Elements.size())
        return false;
    return m_Elements[m_CurrentElement]->down(key);
}
}
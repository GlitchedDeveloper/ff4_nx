#include "container.h"

#include <vector>

#include "../../imgui/imgui.h"
#include "../../pad_manager.h"
#include "../menu.h"

namespace menu::elements {
Container::Container() {
    m_CurrentElement = 0;
}
Container::~Container() {
    for (Element* element : m_Elements) {
        delete element;
    }
}
void Container::update() {
    if (m_CurrentElement < 0)
        m_CurrentElement = 0;
    if (m_CurrentElement >= m_Elements.size())
        m_CurrentElement = m_Elements.size() - 1;
    for (size_t i = 0; i < m_Elements.size(); i++) {
        Element* element = m_Elements[i];
        bool focused     = i == m_CurrentElement;
        element->setFocused(focused);
        element->update();
        if (focused)
            ImGui::SetScrollHereY();
    }
}
void Container::preUpdate() {
    if (m_CurrentElement >= m_Elements.size())
        return;
    m_Elements[m_CurrentElement]->preUpdate();
}
void Container::postUpdate() {
    if (m_CurrentElement >= m_Elements.size())
        return;
    m_Elements[m_CurrentElement]->postUpdate();
}
bool Container::down(u32 key) {
    if (m_CurrentElement >= m_Elements.size())
        return false;

    Element* elem = m_Elements[m_CurrentElement];

    if (key == pad_manager::FF4Button_Up) {
        if (elem->down(key))
            return true;
        if (m_CurrentElement > 0) {
            m_CurrentElement--;
            playMenuSE(3);
            return true;
        }
        return false;
    }
    if (key == pad_manager::FF4Button_Down) {
        if (elem->down(key))
            return true;
        if (m_CurrentElement < m_Elements.size() - 1) {
            m_CurrentElement++;
            playMenuSE(3);
            return true;
        }
        return false;
    }
    return elem->down(key);
}
bool Container::held(u32 key, float time) {
    if (m_CurrentElement >= m_Elements.size())
        return false;
    return m_Elements[m_CurrentElement]->held(key, time);
}
}
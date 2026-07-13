#include "float.h"

#include "../../imgui/imgui.h"
#include "../../pad_manager.h"

namespace menu::elements {

Float::Float(const char* name, float min, float max, float step, float value, std::function<void(float)> callback, u32 flags)
    : m_Name(name)
    , m_Min(min)
    , m_Max(max)
    , m_Step(step)
    , m_Value(value)
    , m_Callback(callback)
    , m_Flags(flags) { };

void Float::update() {
    ImGui::PushID(m_Name.c_str());
    ImVec4 color;
    if (m_IsFocused) {
        color = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
    } else {
        color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    }
    ImGui::PushStyleColor(ImGuiCol_Text, color);
    ImGui::Text("%s:", m_Name.c_str());
    ImGui::PopStyleColor();
    ImGui::SameLine();
    if (ImGui::SliderFloat("##value", &m_Value, m_Min, m_Max)) {
        m_Callback(m_Value);
    }
    ImGui::PopID();
}
bool Float::down(u32 key) {
    if (key == pad_manager::FF4Button_Right) {
        m_Value += m_Step;
        if (m_Value > m_Max)
            m_Value = m_Max;
        m_Callback(m_Value);
        return true;
    }
    if (key == pad_manager::FF4Button_Left) {
        m_Value -= m_Step;
        if (m_Value < m_Min)
            m_Value = m_Min;
        m_Callback(m_Value);
        return true;
    }
    return false;
}
}
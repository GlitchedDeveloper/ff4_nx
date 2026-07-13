#include "option.h"

#include "../../imgui/imgui.h"
#include "../../pad_manager.h"
#include "../menu.h"

namespace menu::elements {

static std::vector<std::string> vectorize(const char** options, size_t options_size) {
    std::vector<std::string> out;
    out.reserve(options_size);
    for (size_t i = 0; i < options_size; ++i) {
        out.emplace_back(options[i]);
    }
    return out;
}

Option::Option(const char* name, const char** options, size_t options_size, int state, std::function<void(int)> callback, u32 flags)
    : m_Name(name)
    , m_Options(vectorize(options, options_size))
    , m_Callback(callback)
    , m_State(state)
    , m_Flags(flags) { };

void Option::update() {
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
    if (m_Flags & OptionFlags_CondenseOptions) {
        ImGui::SameLine();
        color = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
        ImGui::PushStyleColor(ImGuiCol_Button, color);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, color);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, color);
        if (ImGui::ArrowButton("##left", ImGuiDir_Left)) {
            this->down(pad_manager::FF4Button_Left);
        }
        ImGui::PopStyleColor(3);
        ImGui::SameLine();
        color = ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered);
        ImGui::PushStyleColor(ImGuiCol_Button, color);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, color);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, color);
        ImGui::Button(m_Options[m_State].c_str());
        ImGui::PopStyleColor(3);
        ImGui::SameLine();
        color = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
        ImGui::PushStyleColor(ImGuiCol_Button, color);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, color);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, color);
        if (ImGui::ArrowButton("##right", ImGuiDir_Right)) {
            this->down(pad_manager::FF4Button_Right);
        }
        ImGui::PopStyleColor(3);
    } else {
        for (size_t i = 0; i < m_Options.size(); i++) {
            ImGui::SameLine();
            const std::string& option = m_Options[i];
            if (m_State == i)
                color = ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered);
            else
                color = ImGui::GetStyleColorVec4(ImGuiCol_Button);
            ImGui::PushStyleColor(ImGuiCol_Button, color);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, color);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, color);
            if (ImGui::Button(option.c_str())) {
                m_State = i;
                playMenuSE(3);
                m_Callback(m_State);
            }
            ImGui::PopStyleColor(3);
        }
    }
    if (m_Flags & OptionFlags_RequiresRestart) {
        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.78f, 0.78f, 0.78f, 1.0f));
        ImGui::Text("(Requires Restart)");
        ImGui::PopStyleColor();
    }
    ImGui::PopID();
}
bool Option::down(u32 key) {
    if (key == pad_manager::FF4Button_Right) {
        m_State++;
        if (m_State >= (long)m_Options.size()) {
            m_State -= m_Options.size();
        }
        playMenuSE(3);
        m_Callback(m_State);
        return true;
    }
    if (key == pad_manager::FF4Button_Left) {
        if (m_State < 1) {
            m_State += m_Options.size();
        }
        m_State--;
        playMenuSE(3);
        m_Callback(m_State);
        return true;
    }
    return false;
}
}
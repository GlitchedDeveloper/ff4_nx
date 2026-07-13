#include "achievements.h"

#include <cmath>
#include <cstdio>

#include "../../achievement.h"
#include "../../config.h"
#include "../../game.h"
#include "../../imgui/imgui.h"
#include "../../pad_manager.h"

using namespace pad_manager;

namespace menu::tabs {
using namespace elements;

Achievements::Achievements()
    : Tab("Achievements") { }

void Achievements::update() {
    const int count = achievement::get_count();
    m_VisibleIds.clear();

    int unlockedCount = 0;
    for (int i = 0; i < count; i++) {
        if (achievement::is_unlocked(i))
            unlockedCount++;

        bool secret   = achievement::is_secret(i);
        bool unlocked = achievement::is_unlocked(i);

        if (secret && !unlocked && !config::show_secret_achievements)
            continue;

        m_VisibleIds.push_back(i);
    }

    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.0f, 1.0f));
    ImGui::Text("Achievements Unlocked: %d / %d", unlockedCount, count);
    ImGui::PopStyleColor();

    if (m_VisibleIds.empty()) {
        ImGui::Separator();
        ImGui::Text("No achievements available.");
        return;
    }

    if (m_CurrentElement >= m_VisibleIds.size())
        m_CurrentElement = m_VisibleIds.size() - 1;

    const float iconSize         = 125.0f * game::IMGUI_SCALE;
    const float touchSensitivity = 0.3f;

    if (ImGui::GetIO().MouseDown[0] && ImGui::IsMouseDragging(0, 1.0f)) {
        if (!m_TouchStarted) {
            m_TouchStarted = true;
            m_TouchStartY  = ImGui::GetIO().MousePos.y;
        } else {
            float deltaY = ImGui::GetIO().MousePos.y - m_TouchStartY;
            if (std::fabsf(deltaY) > 20.0f) {
                if (deltaY > 0 && m_CurrentElement > 0) {
                    m_CurrentElement--;
                } else if (deltaY < 0 && m_CurrentElement < m_VisibleIds.size() - 1) {
                    m_CurrentElement++;
                }
                m_UpdateScroll = true;
                m_TouchStartY  = ImGui::GetIO().MousePos.y;
            }
        }
    } else if (ImGui::IsMouseClicked(0) && !m_TouchStarted) {
        float mousePos   = ImGui::GetIO().MousePos.y;
        float itemHeight = iconSize + ImGui::GetTextLineHeightWithSpacing() * 5;
        int clickedItem  = (int)((mousePos - ImGui::GetScrollY()) / itemHeight);

        if (clickedItem >= 0 && clickedItem < (int)m_VisibleIds.size()) {
            float itemY = ImGui::GetScrollY() + clickedItem * itemHeight;
            if (mousePos < itemY + itemHeight * touchSensitivity) {
                if (clickedItem > 0) {
                    m_CurrentElement = clickedItem - 1;
                    m_UpdateScroll   = true;
                }
            } else if (mousePos > itemY + itemHeight * (1.0f - touchSensitivity)) {
                if (clickedItem < (int)m_VisibleIds.size() - 1) {
                    m_CurrentElement = clickedItem + 1;
                    m_UpdateScroll   = true;
                }
            }
        }
    }

    if (!ImGui::GetIO().MouseDown[0] || ImGui::IsMouseReleased(0)) {
        m_TouchStarted = false;
    }

    for (size_t i = 0; i < m_VisibleIds.size(); i++) {
        const int id        = m_VisibleIds[i];
        const bool unlocked = achievement::is_unlocked(id);
        const bool secret   = achievement::is_secret(id);

        ImVec4 textColor;
        if (m_CurrentElement == i) {
            textColor = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
        } else {
            textColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
        }

        ImGui::PushID(id);

        ImGui::Separator();

        float imageStartY  = ImGui::GetCursorPosY();
        float imageBottomY = imageStartY + iconSize;

        unsigned int texture = achievement::get_texture(id);
        if (texture != 0) {
            ImGui::Image(static_cast<ImTextureID>(static_cast<uintptr_t>(texture)),
                ImVec2(iconSize, iconSize));
            if (m_CurrentElement == i) {
                if (m_UpdateScroll) {
                    ImGui::SetScrollHereY(0.5f);
                    m_UpdateScroll = false;
                }
            }
            ImGui::SameLine();
        }

        ImGui::BeginGroup();
        ImGui::PushStyleColor(ImGuiCol_Text, textColor);

        const char* name        = achievement::get_name(id);
        const char* description = achievement::get_description(id);
        ImGui::SetWindowFontScale(1.1f);
        ImGui::Text("%s", name);

        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.78f, 0.78f, 0.78f, 1.0f));
        ImGui::SetWindowFontScale(0.75f);

        ImGui::SetCursorPosY(ImGui::GetCursorPosY() - ImGui::GetStyle().ItemSpacing.y * 2.0f);

        if (description && description[0] != '\0')
            ImGui::TextWrapped("%s", description);
        ImGui::PopStyleColor();

        ImGui::PopStyleColor();

        ImGui::SetWindowFontScale(0.75f);

        if (texture != 0) {
            float progressBarHeight = ImGui::GetTextLineHeightWithSpacing() + ImGui::GetStyle().FramePadding.y * 2.0f;
            float currentY          = ImGui::GetCursorPosY();
            float targetY           = imageBottomY - progressBarHeight;
            if (currentY < targetY) {
                ImGui::Dummy(ImVec2(0, targetY - currentY));
            }
        }

        float progress = achievement::get_progress(id);
        if (progress < 0.0f)
            progress = 0.0f;
        if (progress > 100.0f)
            progress = 100.0f;

        char overlay[32];
        if (unlocked)
            snprintf(overlay, sizeof(overlay), "Unlocked");
        else
            snprintf(overlay, sizeof(overlay), "%d%%", (int)(progress));

        ImGui::ProgressBar(progress / 100.0f, ImVec2(-1, 0), overlay);

        ImGui::EndGroup();

        ImGui::SetWindowFontScale(1.0f);

        ImGui::PopID();
    }
}

bool Achievements::down(u32 key) {
    if (key == FF4Button_Up) {
        if (m_CurrentElement > 0) {
            m_CurrentElement--;
            m_UpdateScroll = true;
            m_LastMoveTime = 0.5f;
            return true;
        }
        m_LastMoveTime = 0.5f;
        return false;
    }
    if (key == FF4Button_Down) {
        if (m_CurrentElement < m_VisibleIds.size() - 1) {
            m_CurrentElement++;
            m_UpdateScroll = true;
            m_LastMoveTime = 0.5f;
            return true;
        }
        m_LastMoveTime = 0.5f;
        return false;
    }
    return false;
}

bool Achievements::held(u32 key, float time) {
    if (key == FF4Button_Up) {
        bool ret = false;
        while (m_LastMoveTime + 0.05f < time) {
            if (m_CurrentElement > 0) {
                m_CurrentElement--;
                m_UpdateScroll = true;
                ret            = true;
            }
            m_LastMoveTime += 0.05f;
        }
        return ret;
    }
    if (key == FF4Button_Down) {
        bool ret = false;
        while (m_LastMoveTime + 0.05f < time) {
            if (m_CurrentElement < m_VisibleIds.size() - 1) {
                m_CurrentElement++;
                m_UpdateScroll = true;
                ret            = true;
            }
            m_LastMoveTime += 0.05f;
        }
        return ret;
    }
    return false;
}
}
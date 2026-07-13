#include "credits.h"

#include "../../imgui/imgui.h"

namespace menu::tabs {
using namespace elements;

Credits::Credits()
    : Tab("Credits") {
    };

void RenderLicense(const char* license) {
    ImGui::SameLine();
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetTextLineHeight() * 0.25);
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
    ImGui::SetWindowFontScale(0.75f);
    ImGui::TextUnformatted(license);
    ImGui::SetWindowFontScale(1.0f);
    ImGui::PopStyleColor();
}

void Credits::update() {
    ImGui::TextUnformatted("Final Fantasy IV (3D Remake) Nintendo Switch Port");
    RenderLicense("MIT License");

    ImGui::SeparatorText("Development");
    ImGui::BulletText("GlitchedDeveloper - Main Developer");

    ImGui::SeparatorText("Based On");
    ImGui::Bullet();
    ImGui::TextUnformatted("givethesourceplox - Bully NX");
    RenderLicense("MIT License");
    ImGui::Bullet();
    ImGui::TextUnformatted("fgsfds - Max Payne Mobile Nintendo Switch port");
    RenderLicense("MIT License");
    ImGui::Bullet();
    ImGui::TextUnformatted("Rinnegatamante - Final Fantasy 4 Vita");
    RenderLicense("MIT License");

    ImGui::SeparatorText("Included Libraries");
    ImGui::Bullet();
    ImGui::TextUnformatted("Omar Cornut - Dear ImGui");
    RenderLicense("MIT License");
    ImGui::Bullet();
    ImGui::TextUnformatted("Alexis Engelke - Disarm");
    RenderLicense("BSD-3-Clause License");
    ImGui::Bullet();
    ImGui::TextUnformatted("Sean Barrett - stb");
    RenderLicense("Public Domain");
}
}
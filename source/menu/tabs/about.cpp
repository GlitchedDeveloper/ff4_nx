#include "about.h"

#include "../../imgui/imgui.h"

namespace menu::tabs {
using namespace elements;

About::About()
    : Tab("About") { };

void About::update() {
    ImGui::TextUnformatted("Final Fantasy IV (3D Remake) Nintendo Switch Port");
    ImGui::SameLine();
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetTextLineHeight() * 0.25);
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
    ImGui::SetWindowFontScale(0.75f);
    ImGui::TextUnformatted("Version 1.0.2");
    ImGui::SetWindowFontScale(1.0f);
    ImGui::PopStyleColor();

    ImGui::SeparatorText("Overview");
    ImGui::TextWrapped("This software is an unofficial, fan-made wrapper that allows for the AARCH64 Android version of Final Fantasy IV (3D Remake) to run natively on the Nintendo Switch with modern enhancements.");
    ImGui::SeparatorText("Legal Disclaimer");
    ImGui::TextWrapped("This project is not affiliated with, endorsed by, or sponsored by Square Enix. Final Fantasy IV and all related characters, assets, and trademarks are the property of Square Enix.");
    ImGui::SeparatorText("Open Source");
    ImGui::TextWrapped("The source code is available at https://github.com/GlitchedDeveloper/ff4_nx");

    ImGui::SeparatorText("Ephesians 2:4-5 NIV");
    ImGui::TextWrapped("'But because of his great love for us, God, who is rich in mercy, made us alive with Christ even when we were dead in transgressions-it is by grace you have been saved.'");

    ImGui::Indent();
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
    ImGui::SetWindowFontScale(0.5f);
    ImGui::TextWrapped("Scripture quotations taken from The Holy Bible, New International Version (R) NIV (R)");
    ImGui::TextWrapped("Copyright (c) 1973 1978 1984 2011 by Biblica, Inc. TM");
    ImGui::TextWrapped("Used by permission. All rights reserved worldwide.");
    ImGui::SetWindowFontScale(1.0f);
    ImGui::PopStyleColor();
    ImGui::Unindent();
}
}
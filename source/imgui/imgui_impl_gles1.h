#pragma once
#include "imgui.h"

bool ImGui_ImplGLES1_Init();
void ImGui_ImplGLES1_Shutdown();
void ImGui_ImplGLES1_NewFrame();
void ImGui_ImplGLES1_RenderDrawData(ImDrawData* draw_data);
bool ImGui_ImplGLES1_CreateFontsTexture();
void ImGui_ImplGLES1_DestroyFontsTexture();
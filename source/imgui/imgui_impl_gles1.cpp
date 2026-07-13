#include "imgui_impl_gles1.h"

#include <GLES/gl.h>

static GLuint g_FontTexture = 0;

bool ImGui_ImplGLES1_Init() {
    return true;
}

void ImGui_ImplGLES1_Shutdown() {
    ImGui_ImplGLES1_DestroyFontsTexture();
}

void ImGui_ImplGLES1_NewFrame() {
    if (!g_FontTexture)
        ImGui_ImplGLES1_CreateFontsTexture();
}

bool ImGui_ImplGLES1_CreateFontsTexture() {
    ImGuiIO& io = ImGui::GetIO();

    unsigned char* pixels;
    int width, height;
    io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

    glGenTextures(1, &g_FontTexture);
    glBindTexture(GL_TEXTURE_2D, g_FontTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
        GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    io.Fonts->SetTexID((ImTextureID)(uintptr_t)g_FontTexture);
    return true;
}

void ImGui_ImplGLES1_DestroyFontsTexture() {
    if (g_FontTexture) {
        glDeleteTextures(1, &g_FontTexture);
        ImGui::GetIO().Fonts->SetTexID(0);
        g_FontTexture = 0;
    }
}

void ImGui_ImplGLES1_RenderDrawData(ImDrawData* draw_data) {
    if (draw_data->DisplaySize.x <= 0 || draw_data->DisplaySize.y <= 0)
        return;

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrthof(draw_data->DisplayPos.x,
        draw_data->DisplayPos.x + draw_data->DisplaySize.x,
        draw_data->DisplayPos.y + draw_data->DisplaySize.y,
        draw_data->DisplayPos.y,
        -1.0f, 1.0f);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glEnable(GL_SCISSOR_TEST);
    glEnable(GL_TEXTURE_2D);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    ImVec2 clip_off = draw_data->DisplayPos;

    for (int n = 0; n < draw_data->CmdListsCount; n++) {
        const ImDrawList* cmd_list = draw_data->CmdLists[n];
        const ImDrawVert* vtx_buf  = cmd_list->VtxBuffer.Data;
        const ImDrawIdx* idx_buf   = cmd_list->IdxBuffer.Data;

        glVertexPointer(2, GL_FLOAT, sizeof(ImDrawVert),
            (const GLvoid*)((const char*)vtx_buf + offsetof(ImDrawVert, pos)));
        glTexCoordPointer(2, GL_FLOAT, sizeof(ImDrawVert),
            (const GLvoid*)((const char*)vtx_buf + offsetof(ImDrawVert, uv)));
        glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(ImDrawVert),
            (const GLvoid*)((const char*)vtx_buf + offsetof(ImDrawVert, col)));

        for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++) {
            const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];

            if (pcmd->UserCallback) {
                pcmd->UserCallback(cmd_list, pcmd);
                continue;
            }

            ImVec2 clip_min(pcmd->ClipRect.x - clip_off.x, pcmd->ClipRect.y - clip_off.y);
            ImVec2 clip_max(pcmd->ClipRect.z - clip_off.x, pcmd->ClipRect.w - clip_off.y);
            if (clip_max.x <= clip_min.x || clip_max.y <= clip_min.y)
                continue;

            glScissor((GLint)clip_min.x,
                (GLint)(draw_data->DisplaySize.y - clip_max.y),
                (GLint)(clip_max.x - clip_min.x),
                (GLint)(clip_max.y - clip_min.y));

            glBindTexture(GL_TEXTURE_2D, (GLuint)(uintptr_t)pcmd->GetTexID());
            static_assert(sizeof(ImDrawIdx) == 2, "ImGui must use 16-bit indices on GLES1");
            glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount,
                GL_UNSIGNED_SHORT,
                idx_buf + pcmd->IdxOffset);
        }
    }

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);

    glDisable(GL_SCISSOR_TEST);
    glEnable(GL_DEPTH_TEST);

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}
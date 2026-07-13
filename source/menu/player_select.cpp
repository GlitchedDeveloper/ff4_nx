#include "player_select.h"

#include <GLES/gl.h>

#include "../babil.h"
#include "../bridge.h"
#include "../config.h"
#include "../game.h"
#include "../imgui/imgui.h"
#include "../multiplayer.h"
#include "../pad_manager.h"
#include "../stb_image.h"
#include "../util.h"

using namespace pad_manager;
using namespace babil;

namespace player_select {

static GLuint g_FaceTexture = 0;
static int g_FaceWidth      = 0;
static int g_FaceHeight     = 0;
static bool g_TriedLoad     = false;

static int currentTab  = 0;
static bool settingTab = false;

static float g_AnimScale     = 1.0f;
static bool g_Animating      = false;
static float g_AnimElapsedMs = 0.0f;
static int g_PrevIdx         = -1;

static constexpr float ANIM_DURATION_MS = 300.0f;
static constexpr float SCALE_MIN        = 0.5f;

void loadFaceTextures() {
    int size;
    u8* data = bridge::loadFile("MENU_Common.dat", &size);
    debugPrintf("Size: %d\n", size);

    if (memcmp(data, "SSAM", 4) != 0)
        return;

    uint32_t num   = *(uint32_t*)(data + 4);
    uint32_t start = (num * 0x28) + 8;

    const char* target = "face.NCGR";

    int left  = 0;
    int right = (int)num - 1;

    while (left <= right) {
        int mid = left + ((right - left) / 2);

        uint8_t* entry = data + 8 + (mid * 0x28);

        char name[33];
        memcpy(name, entry + 8, 32);
        name[32] = 0;

        int cmp = strcmp(name, target);

        if (cmp == 0) {
            uint32_t offset   = *(uint32_t*)(entry + 0);
            uint32_t fileSize = *(uint32_t*)(entry + 4);

            uint8_t* fileData = data + start + offset;

            debugPrintf(
                "Found %s, size=%u, offset=%u\n",
                name,
                fileSize,
                start + offset);

            int width    = 0;
            int height   = 0;
            int channels = 0;

            unsigned char* pixels = stbi_load_from_memory(
                fileData,
                fileSize,
                &width,
                &height,
                &channels,
                4);

            if (!pixels)
                return;

            g_FaceWidth  = width;
            g_FaceHeight = height;

            glGenTextures(1, &g_FaceTexture);
            glBindTexture(GL_TEXTURE_2D, g_FaceTexture);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RGBA,
                width,
                height,
                0,
                GL_RGBA,
                GL_UNSIGNED_BYTE,
                pixels);

            stbi_image_free(pixels);

            break;
        }

        if (cmp < 0)
            left = mid + 1;
        else
            right = mid - 1;
    }
}

static int getMemberIDForOrder(u8 order) {
    if (order > 5)
        return -1;
    pl::Player::cls* member = pl::PlayerParty::memberForOrder(pl::PlayerParty::playerPartyInstance_, order);
    if (member == pl::PlayerParty::InvalidPlayer) {
        return -1;
    }
    return member->m_PlayerType;
}

static int idx = 0;

void open() {
    if (!g_TriedLoad) {
        loadFaceTextures();
        g_TriedLoad = true;
    }

    idx = 0;
    while (idx < 5 && getMemberIDForOrder(idx) == -1)
        idx++;

    g_PrevIdx       = -1;
    g_Animating     = false;
    g_AnimElapsedMs = 0.0f;
    g_AnimScale     = 1.0f;

    multiplayer::clear();
    game::g_playerSelectOpen = true;
}

void update() {
    s32 kDown = getDown(g_Pads[0]);

    if (idx != g_PrevIdx) {
        g_PrevIdx       = idx;
        g_Animating     = true;
        g_AnimElapsedMs = 0.0f;
        g_AnimScale     = SCALE_MIN;
    }

    if (g_Animating) {
        float deltaTimeMs = 1000.0f / (bridge::framerate > 0 ? bridge::framerate : 1.0f);
        g_AnimElapsedMs += deltaTimeMs;

        float t = std::min(g_AnimElapsedMs / ANIM_DURATION_MS, 1.0f);

        t           = 1.0f - (1.0f - t) * (1.0f - t);
        g_AnimScale = SCALE_MIN + (1.0f - SCALE_MIN) * t;

        if (g_AnimElapsedMs >= ANIM_DURATION_MS) {
            g_Animating = false;
            g_AnimScale = 1.0f;
        }
    }

    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(config::screen_width, config::screen_height), ImGuiCond_Always);
    ImGui::SetNextWindowBgAlpha(0.9f);
    ImGui::SetNextWindowFocus();

    if (ImGui::Begin("FF4NX Player Select Menu", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse)) {
        if (!g_TriedLoad) {
            loadFaceTextures();
            g_TriedLoad = true;
        }

        if (g_FaceTexture != 0) {
            int id = getMemberIDForOrder(idx);

            float u0 = 0.0f;
            float v0 = (float)(id * g_FaceWidth) / g_FaceHeight;
            float u1 = 1.0f;
            float v1 = (float)((id + 1) * g_FaceWidth) / g_FaceHeight;

            float scaledSize  = (float)g_FaceWidth * 2.0f * g_AnimScale;
            ImVec2 windowSize = ImGui::GetWindowSize();

            float imagePosX = (windowSize.x - scaledSize) * 0.5f;
            float imagePosY = (windowSize.y - scaledSize) * 0.5f;

            ImGui::SetCursorPos(ImVec2(imagePosX, imagePosY));

            ImGui::Image(
                (ImTextureID)(intptr_t)g_FaceTexture,
                ImVec2(scaledSize, scaledSize),
                ImVec2(u0, v0),
                ImVec2(u1, v1));

            const char* text = "Press [SELECT] on any controller to assign this character to it";
            ImVec2 textSize  = ImGui::CalcTextSize(text);

            float textPosX = (windowSize.x - textSize.x) * 0.5f;
            float textPosY = windowSize.y - textSize.y - 20.0f;

            ImGui::SetCursorPos(ImVec2(textPosX, textPosY));
            ImGui::Text("%s", text);
        }
    }
    ImGui::End();

    for (s8 player = 0; player < g_ConnectedPads; player++) {
        s32 kDown = getDown(g_Pads[player]);

        if (kDown & FF4Button_Select) {
            int id = getMemberIDForOrder(idx++);

            debugPrintf("id [%d] -> player [%d]\n", id, player);
            multiplayer::g_SymbolToPlayer[id] = player;
            multiplayer::g_PlayerToSymbols[player].push_back(id);
            if (multiplayer::g_PlayerToLastUsedSymbol.find(player) == multiplayer::g_PlayerToLastUsedSymbol.end()) {
                multiplayer::g_PlayerToLastUsedSymbol[player] = id;
            }

            while (idx < 5 && getMemberIDForOrder(idx) == -1)
                idx++;
            if (idx >= 5) {
                lockAllUntilRelease();
                game::g_playerSelectOpen = false;
            }
            break;
        }
    }

    settingTab = false;
}
}
#include "overlay.h"

#include <switch.h>

#include "bridge.h"
#include "imgui/imgui.h"

namespace overlay {

static float fps             = 30.0f;
static float ups             = 30.0f;
static float frameTimes[120] = { };
static int frameTimeIdx      = 0;
static float frameMs;
static float updateTimes[120] = { };
static int updateTimeIdx      = 0;
static float updateMs;

void begin(bool isUpdateFrame) {
    if (isUpdateFrame) {
        if (bridge::g_UpdateCount > 0) {
            float dt = (float)armTicksToNs(bridge::g_ElapsedTicks) / 1000000000.0f;

            float currentFps = 1.0f / dt;
            fps              = (fps * 0.9f) + (currentFps * 0.1f);

            float currentUps = (float)bridge::g_UpdateCount / dt;
            ups              = (ups * 0.9f) + (currentUps * 0.1f);

            frameMs                  = dt * 1000.0f;
            frameTimes[frameTimeIdx] = frameMs;
            frameTimeIdx             = (frameTimeIdx + 1) % 120;

            bridge::g_UpdateCount  = 0;
            bridge::g_ElapsedTicks = 0;
        }

        updateMs                   = ups > 0.0f ? 1000.0f / ups : 0.0f;
        updateTimes[updateTimeIdx] = updateMs;
        updateTimeIdx              = (updateTimeIdx + 1) % 120;
    }

    ImGui::SetNextWindowPos(ImVec2(5, 5), ImGuiCond_Always);
    ImGui::SetNextWindowBgAlpha(0.5f);
    ImGui::Begin("Overlay", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoInputs);

    ImGui::SetWindowFontScale(2.0f / 3.0f);
}

void drawFPS() {
    ImGui::Text("FPS: %.1f", fps);
}

void drawUPS() {
    ImGui::Text("UPS: %.1f", ups);
}

void drawGameSpeedMult() {
    float mult = bridge::getFPSMultiplier();
    if (mult != 1.0f)
        ImGui::Text("Game Speed: %gx", mult);
}

static void drawTimeGraph(const char* label, float currentMs, const float times[], int timesIdx, float targetMs) {
    ImGui::Text("%s: %.1f ms", label, currentMs);

    ImU32 colors[3] = {
        IM_COL32(80, 220, 100, 255),
        IM_COL32(240, 200, 60, 255),
        IM_COL32(240, 80, 80, 255)
    };

    const float maxMs     = targetMs * 2.5f;
    const ImVec2 graphSz  = ImVec2(240, 40);
    const ImVec2 graphPos = ImGui::GetCursorScreenPos();
    ImDrawList* dl        = ImGui::GetWindowDrawList();

    dl->AddRectFilled(graphPos, ImVec2(graphPos.x + graphSz.x, graphPos.y + graphSz.y),
        IM_COL32(0, 0, 0, 150));

    float targetY = graphPos.y + graphSz.y * (1.0f - targetMs / maxMs);
    dl->AddLine(ImVec2(graphPos.x, targetY),
        ImVec2(graphPos.x + graphSz.x, targetY), IM_COL32(255, 255, 255, 50));

    for (int i = 0; i < 119; i++) {
        int i1 = (timesIdx + i) % 120;
        int i2 = (timesIdx + i + 1) % 120;

        float v1 = times[i1];
        float v2 = times[i2];

        float x1 = graphPos.x + (float)i / 119.0f * graphSz.x;
        float x2 = graphPos.x + (float)(i + 1) / 119.0f * graphSz.x;

        auto getY = [&](float v) -> float {
            return graphPos.y + graphSz.y * (1.0f - (v < maxMs ? v / maxMs : 1.0f));
        };
        auto getCi = [&](float v) -> int {
            return v <= targetMs ? 0 : (v <= targetMs * 2.0f ? 1 : 2);
        };

        float ts[4] = { 0.0f, 1.0f };
        int nt      = 2;

        float bvs[2] = { targetMs, targetMs * 2.0f };
        for (int b = 0; b < 2; b++) {
            if ((v1 <= bvs[b]) != (v2 <= bvs[b])) {
                float t = (bvs[b] - v1) / (v2 - v1);
                if (t > 0.0f && t < 1.0f)
                    ts[nt++] = t;
            }
        }

        for (int a = 1; a < nt; a++) {
            float key = ts[a];
            int j     = a - 1;
            while (j >= 0 && ts[j] > key) {
                ts[j + 1] = ts[j];
                j--;
            }
            ts[j + 1] = key;
        }

        for (int s = 0; s < nt - 1; s++) {
            float t0 = ts[s], t1 = ts[s + 1];
            float sx0 = x1 + t0 * (x2 - x1);
            float sy0 = getY(v1 + t0 * (v2 - v1));
            float sx1 = x1 + t1 * (x2 - x1);
            float sy1 = getY(v1 + t1 * (v2 - v1));
            int ci    = getCi(v1 + (t0 + t1) * 0.5f * (v2 - v1));
            dl->AddLine(ImVec2(sx0, sy0), ImVec2(sx1, sy1), colors[ci], 2.0f);
        }
    }

    ImGui::Dummy(graphSz);
}

void drawFrameTimeGraph() {
    float targetUPS = bridge::getTargetUPS();
    if (targetUPS > 60.0f)
        targetUPS = 60.0f;

    float targetMs = 1000.0f / targetUPS;
    drawTimeGraph("Frame", frameMs, frameTimes, frameTimeIdx, targetMs);
}

void drawUpdateTimeGraph() {
    float targetUPS = bridge::getTargetUPS();
    float targetMs  = 1000.0f / targetUPS;

    drawTimeGraph("Update", updateMs, updateTimes, updateTimeIdx, targetMs);
}

void end() {
    ImGui::SetWindowFontScale(1.0f);
    ImGui::End();
}
}
#pragma once

namespace overlay {
void begin(bool isUpdateFrame);
void drawGameSpeedMult();
void drawFPS();
void drawFrameTimeGraph();
void drawUPS();
void drawUpdateTimeGraph();
void end();
}
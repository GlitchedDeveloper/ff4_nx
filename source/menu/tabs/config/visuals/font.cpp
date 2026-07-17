#include "font.h"

#include "../../../../config.h"
#include "../../../elements/option.h"

namespace menu::tabs::visuals {
using namespace elements;

Font::Font()
    : Tab("Font") {
    const char* SuperSamplingOptions[] = { "1x", "2x", "4x", "8x", "16x" };
    const int SuperSamplingValues[]    = { 1, 2, 4, 8, 16 };
    int SuperSamplingState             = 0;
    for (int i = 0; i < 5; i++) {
        if (SuperSamplingValues[i] == config::font_supersampling) {
            SuperSamplingState = i;
        }
    }
    m_Elements.push_back(new Option("Super Sampling", SuperSamplingOptions, std::size(SuperSamplingOptions), SuperSamplingState, [SuperSamplingValues](int value) {
        config::font_supersampling = SuperSamplingValues[value];
        config::mark_for_write();
    }));

    const char* ScalingModeOptions[] = { "Smooth", "Pixelated" };
    m_Elements.push_back(new Option("Scaling Mode", ScalingModeOptions, std::size(ScalingModeOptions), config::font_scaling_mode, [](int value) {
        config::font_scaling_mode = (config::FontScalingMode)value;
        config::mark_for_write();
    }));

    const char* ScaleOptions[] = { "1x", "2x", "4x", "8x", "16x" };
    const int ScaleValues[]    = { 1, 2, 4, 8, 16 };
    int ScaleState             = 0;
    for (int i = 0; i < 5; i++) {
        if (ScaleValues[i] == config::internal_font_scale) {
            ScaleState = i;
        }
    }
    m_Elements.push_back(new Option("Internal Font Scale", ScaleOptions, std::size(ScaleOptions), ScaleState, [ScaleValues](int value) {
        config::internal_font_scale = ScaleValues[value];
        config::mark_for_write();
    }));
};
}
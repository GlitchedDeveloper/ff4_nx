#include "general.h"

#include "../../../config.h"
#include "../../elements/boolean.h"
#include "../../elements/option.h"

namespace menu::tabs {
using namespace elements;

General::General()
    : Tab("General") {
    const char* startupPartOptions[] = { "Default", "DebugPart", "CompanyLogoPart", "MoviePart", "TitlePart", "SoundDebugPart", "SoundPlayerPart" };
    m_Elements.push_back(new Boolean("Skip Launcher", config::skip_launcher, [](bool value) {
        config::skip_launcher = value;
        config::mark_for_write();
    }));
    int startupPart_state = config::game_startup_part + 1;
    if (startupPart_state == 33) {
        startupPart_state = 5;
    }
    if (startupPart_state == 34) {
        startupPart_state = 6;
    }
    m_Elements.push_back(new Option("Game Startup Part", startupPartOptions, std::size(startupPartOptions), startupPart_state, [](int value) {
        if (value == 5)
            config::game_startup_part = 32;
        else if (value == 6)
            config::game_startup_part = 33;
        else
            config::game_startup_part = value - 1;
        config::mark_for_write(); }, OptionFlags_CondenseOptions));
    m_Elements.push_back(new Boolean("Per User Savefiles", config::user_save_separation, [](bool value) {
        config::user_save_separation = value;
        config::mark_for_write(); }, OptionFlags_RequiresRestart));
    const char* joyconHoldStyle[] = { "Vertical", "Horizontal" };
    m_Elements.push_back(new Option("Joycon Hold Style", joyconHoldStyle, std::size(joyconHoldStyle), config::joycon_hold_style, [](int value) {
        config::joycon_hold_style = value;
        config::mark_for_write();
    }));
    const char* languages[] = {
        "Auto",
        "Japanese",
        "English",
        "French",
        "German",
        "Italian",
        "Spanish",
        "Simplified Chinese",
        "Traditional Chinese",
        "Korean",
        "Portuguese",
        "Russian",
        "Thai"
    };
    m_Elements.push_back(new Option("Language", languages, std::size(languages), config::language + 1, [](int value) {
        config::language = value - 1;
        config::mark_for_write(); }, OptionFlags_CondenseOptions));
    m_Elements.push_back(new Option("Audio Language", languages, std::size(languages), config::audio_language + 1, [](int value) {
        config::audio_language = value - 1;
        config::mark_for_write(); }, OptionFlags_CondenseOptions));
    m_Elements.push_back(new Boolean("Show Secret Achievements", config::show_secret_achievements, [](bool value) {
        config::show_secret_achievements = value;
        config::mark_for_write();
    }));
    m_Elements.push_back(new Boolean("Better Stick Movement", config::better_stick_movement, [](bool value) {
        config::better_stick_movement = value;
        config::mark_for_write();
    }));
};
}
#include "config.h"

#include <switch.h>

#include <fstream>
#include <iostream>

#include "bridge.h"
#include "game.h"
#include "stdext.h"
#include "util.h"

namespace config {
int screen_width                       = 0;
int screen_height                      = 0;
char save_filename[256]                = SAVE_FILENAME;
bool user_save_separation              = true;
int joycon_hold_style                  = 0;
bool multiplayer_enabled               = false;
bool skip_launcher                     = false;
bool dfps_battles_enabled              = false;
float dfps_battles_mult                = 1.0f;
int game_startup_part                  = -1;
bool show_fps                          = false;
bool hide_back_button                  = true;
bool hide_vehicle_buttons              = true;
bool remove_newgameplus_limit          = false;
bool fix_titlepart                     = true;
int language                           = -1;
int audio_language                     = -1;
bool show_secret_achievements          = false;
bool show_achievement_popup            = true;
bool log_to_file                       = false;
bool show_frame_graph                  = false;
bool overclock_movie_cpu               = false;
bool overclock_movie_mem               = false;
bool limit_movie_fps                   = true;
bool show_game_speed_mult              = true;
bool guaranteed_encounter              = false;
bool equip_anything                    = false;
bool marked_for_write                  = false;
float world_rumble_intensity           = 0.5f;
float battle_rumble_intensity          = 0.5f;
bool show_ups                          = false;
bool show_update_graph                 = false;
bool SURELY_MAX_DAMAGE                 = false;
bool QUICK_WAIT                        = false;
bool ENEMY_INVINCIBLE                  = false;
bool FRIEND_INVINCIBLE                 = false;
bool RESTART                           = false;
bool SURELY_ESCAPE                     = false;
bool SURELY_CONDITION                  = false;
bool TRANSFIX                          = false;
bool MDEF_INVALIDATION                 = false;
bool QUICK_EVENT                       = false;
bool SURELY_HIT                        = false;
bool SURELY_MISS                       = false;
bool SURELY_CRITICAL                   = false;
bool MONSTER_STOP_ACTION               = false;
bool DAMAGE_OVER_LIMIT                 = false;
bool OPEN_ENEMY_HP                     = false;
bool PHYSICS_RANDOM_MAX                = false;
bool MAGIC_RANDOM_MAX                  = false;
bool QUICK_TURN                        = false;
bool DRAW_CHAR_TOUCH_RECT              = false;
bool MODEL_CHANGE_CHECK                = false;
bool MODEL_CHANGE_SOUND                = false;
bool use_left_stick                    = true;
bool use_right_stick                   = true;
bool reobtainable_limit_break          = false;
bool remove_gil_limit                  = false;
bool swap_in_vehicle                   = false;
bool require_modifier_for_battle_pause = false;
bool augmentless_stat_growth           = false;
bool better_stick_movement             = true;
int font_supersampling                 = 16;
FontScalingMode font_scaling_mode      = FontScalingMode_Smooth;

std::vector<std::string> mod_order;
std::unordered_map<std::string, bool> mods;
std::vector<std::string> font_order;
std::unordered_map<std::string, bool> fonts;

void read_config(const char* file) {
    if (appletGetOperationMode() == AppletOperationMode_Console) {
        screen_width  = 1920;
        screen_height = 1080;
    } else {
        // screen_width  = 1280;
        // screen_height = 720;
        screen_width  = 1920;
        screen_height = 1080;
    }

    std::ifstream in(file);
    if (!in.is_open()) {
        debugPrintf("Failed to open config file: %s\n", file);
        return;
    }

    std::string section;
    std::string line;

    while (std::getline(in, line)) {
        line = stdext::trim(line);

        if (line.empty() || line[0] == ';' || line[0] == '#')
            continue;

        if (line.front() == '[' && line.back() == ']') {
            section = line.substr(1, line.size() - 2);
            continue;
        }

        const auto eq = line.find('=');
        if (eq == std::string::npos)
            continue;

        const std::string key   = stdext::trim(line.substr(0, eq));
        const std::string value = stdext::trim(line.substr(eq + 1));

        if (section == "general") {
            if (key == "skip_launcher")
                skip_launcher = (value == "true");
            else if (key == "game_startup_part")
                game_startup_part = std::stoi(value);
            else if (key == "user_save_separation")
                user_save_separation = (value == "true");
            else if (key == "joycon_hold_style")
                joycon_hold_style = std::stoi(value);
            else if (key == "language")
                language = std::stoi(value);
            else if (key == "audio_language")
                audio_language = std::stoi(value);
            else if (key == "show_secret_achievements")
                show_secret_achievements = (value == "true");
            else if (key == "better_stick_movement")
                better_stick_movement = (value == "true");
        } else if (section == "ui") {
            if (key == "hide_back_button")
                hide_back_button = (value == "true");
            else if (key == "hide_vehicle_buttons")
                hide_vehicle_buttons = (value == "true");
            else if (key == "fix_titlepart")
                fix_titlepart = (value == "true");
        } else if (section == "overlay") {
            if (key == "show_fps")
                show_fps = (value == "true");
            else if (key == "show_achievement_popup")
                show_achievement_popup = (value == "true");
            else if (key == "show_frame_graph")
                show_frame_graph = (value == "true");
            else if (key == "show_game_speed_mult")
                show_game_speed_mult = (value == "true");
            else if (key == "show_ups")
                show_ups = (value == "true");
            else if (key == "show_update_graph")
                show_update_graph = (value == "true");
        } else if (section == "movie") {
            if (key == "overclock_movie_cpu")
                overclock_movie_cpu = (value == "true");
            else if (key == "overclock_movie_mem")
                overclock_movie_mem = (value == "true");
            else if (key == "limit_movie_fps")
                limit_movie_fps = (value == "true");
        } else if (section == "multiplayer") {
            if (key == "multiplayer_enabled")
                multiplayer_enabled = (value == "true");
        } else if (section == "boosts") {
            if (key == "encounters_enabled")
                game::g_encountersEnabled = (value == "true");
            else if (key == "gil_multiplier")
                game::g_gilMultiplier = std::stof(value);
            else if (key == "exp_multiplier")
                game::g_expMultiplier = std::stof(value);
        } else if (section == "cheats") {
            if (key == "save_anywhere")
                game::g_saveAnywhere = (value == "true");
            else if (key == "remove_newgameplus_limit")
                remove_newgameplus_limit = (value == "true");
            else if (key == "guaranteed_encounter")
                guaranteed_encounter = (value == "true");
            else if (key == "equip_anything")
                equip_anything = (value == "true");
            else if (key == "reobtainable_limit_break")
                reobtainable_limit_break = (value == "true");
            else if (key == "augmentless_stat_growth")
                augmentless_stat_growth = (value == "true");
        } else if (section == "debug") {
            if (key == "log_to_file")
                log_to_file = (value == "true");
            else if (key == "SURELY_MAX_DAMAGE")
                SURELY_MAX_DAMAGE = (value == "true");
            else if (key == "QUICK_WAIT")
                QUICK_WAIT = (value == "true");
            else if (key == "ENEMY_INVINCIBLE")
                ENEMY_INVINCIBLE = (value == "true");
            else if (key == "FRIEND_INVINCIBLE")
                FRIEND_INVINCIBLE = (value == "true");
            else if (key == "RESTART")
                RESTART = (value == "true");
            else if (key == "SURELY_ESCAPE")
                SURELY_ESCAPE = (value == "true");
            else if (key == "SURELY_CONDITION")
                SURELY_CONDITION = (value == "true");
            else if (key == "TRANSFIX")
                TRANSFIX = (value == "true");
            else if (key == "MDEF_INVALIDATION")
                MDEF_INVALIDATION = (value == "true");
            else if (key == "QUICK_EVENT")
                QUICK_EVENT = (value == "true");
            else if (key == "SURELY_HIT")
                SURELY_HIT = (value == "true");
            else if (key == "SURELY_MISS")
                SURELY_MISS = (value == "true");
            else if (key == "SURELY_CRITICAL")
                SURELY_CRITICAL = (value == "true");
            else if (key == "MONSTER_STOP_ACTION")
                MONSTER_STOP_ACTION = (value == "true");
            else if (key == "DAMAGE_OVER_LIMIT")
                DAMAGE_OVER_LIMIT = (value == "true");
            else if (key == "OPEN_ENEMY_HP")
                OPEN_ENEMY_HP = (value == "true");
            else if (key == "PHYSICS_RANDOM_MAX")
                PHYSICS_RANDOM_MAX = (value == "true");
            else if (key == "MAGIC_RANDOM_MAX")
                MAGIC_RANDOM_MAX = (value == "true");
            else if (key == "QUICK_TURN")
                QUICK_TURN = (value == "true");
            else if (key == "DRAW_CHAR_TOUCH_RECT")
                DRAW_CHAR_TOUCH_RECT = (value == "true");
            else if (key == "MODEL_CHANGE_CHECK")
                MODEL_CHANGE_CHECK = (value == "true");
            else if (key == "MODEL_CHANGE_SOUND")
                MODEL_CHANGE_SOUND = (value == "true");
        } else if (section == "controller") {
            if (key == "world_rumble_intensity")
                world_rumble_intensity = std::stof(value);
            else if (key == "battle_rumble_intensity")
                battle_rumble_intensity = std::stof(value);
            else if (key == "swap_in_vehicle")
                swap_in_vehicle = (value == "true");
            else if (key == "use_left_stick")
                use_left_stick = (value == "true");
            else if (key == "use_right_stick")
                use_right_stick = (value == "true");
            else if (key == "require_modifier_for_battle_pause")
                require_modifier_for_battle_pause = (value == "true");
        } else if (section == "misc") {
            if (key == "remove_gil_limit")
                remove_gil_limit = (value == "true");
        } else if (section == "mods") {
            mod_order.push_back(key);
            mods[key] = (value == "true");
        } else if (section == "fonts") {
            if (key == "font_supersampling")
                font_supersampling = std::stoi(value);
            else if (key == "font_scaling_mode") {
                if (value == "smooth")
                    font_scaling_mode = FontScalingMode_Smooth;
                else if (value == "pixelated")
                    font_scaling_mode = FontScalingMode_Pixelated;
            } else {
                font_order.push_back(key);
                fonts[key] = (value == "true");
            }
        }
    }
}

void mark_for_write() {
    marked_for_write = true;
    appletLockExit();
}

void write_config() {
    marked_for_write = false;
    std::ofstream out(CONFIG_PATH);
    if (!out.is_open()) {
        debugPrintf("Failed to open config file for writing: %s\n", CONFIG_PATH);
        return;
    }

    out << "[general]\n";
    out << "skip_launcher = " << (skip_launcher ? "true" : "false") << "\n";
    out << "game_startup_part = " << game_startup_part << "\n";
    out << "user_save_separation = " << (user_save_separation ? "true" : "false") << "\n";
    out << "joycon_hold_style = " << joycon_hold_style << "\n";
    out << "language = " << language << "\n";
    out << "audio_language = " << audio_language << "\n";
    out << "show_secret_achievements = " << (show_secret_achievements ? "true" : "false") << "\n";
    out << "better_stick_movement = " << (better_stick_movement ? "true" : "false") << "\n";

    out << "\n[ui]\n";
    out << "hide_back_button = " << (hide_back_button ? "true" : "false") << "\n";
    out << "hide_vehicle_buttons = " << (hide_vehicle_buttons ? "true" : "false") << "\n";
    out << "fix_titlepart = " << (fix_titlepart ? "true" : "false") << "\n";

    out << "\n[overlay]\n";
    out << "show_fps = " << (show_fps ? "true" : "false") << "\n";
    out << "show_achievement_popup = " << (show_achievement_popup ? "true" : "false") << "\n";
    out << "show_frame_graph = " << (show_frame_graph ? "true" : "false") << "\n";
    out << "show_game_speed_mult = " << (show_game_speed_mult ? "true" : "false") << "\n";
    out << "show_ups = " << (show_ups ? "true" : "false") << "\n";
    out << "show_update_graph = " << (show_update_graph ? "true" : "false") << "\n";

    out << "\n[movie]\n";
    out << "overclock_movie_cpu = " << (overclock_movie_cpu ? "true" : "false") << "\n";
    out << "overclock_movie_mem = " << (overclock_movie_mem ? "true" : "false") << "\n";
    out << "limit_movie_fps = " << (limit_movie_fps ? "true" : "false") << "\n";

    out << "\n[multiplayer]\n";
    out << "multiplayer_enabled = " << (multiplayer_enabled ? "true" : "false") << "\n";

    out << "\n[boosts]\n";
    out << "encounters_enabled = " << (game::g_encountersEnabled ? "true" : "false") << "\n";
    out << "gil_multiplier = " << game::g_gilMultiplier << "\n";
    out << "exp_multiplier = " << game::g_expMultiplier << "\n";

    out << "\n[cheats]\n";
    out << "save_anywhere = " << (game::g_saveAnywhere ? "true" : "false") << "\n";
    out << "remove_newgameplus_limit = " << (remove_newgameplus_limit ? "true" : "false") << "\n";
    out << "guaranteed_encounter = " << (guaranteed_encounter ? "true" : "false") << "\n";
    out << "equip_anything = " << (equip_anything ? "true" : "false") << "\n";
    out << "reobtainable_limit_break = " << (reobtainable_limit_break ? "true" : "false") << "\n";
    out << "augmentless_stat_growth = " << (augmentless_stat_growth ? "true" : "false") << "\n";

    out << "\n[debug]\n";
    out << "log_to_file = " << (log_to_file ? "true" : "false") << "\n";
    out << "SURELY_MAX_DAMAGE = " << (SURELY_MAX_DAMAGE ? "true" : "false") << "\n";
    out << "QUICK_WAIT = " << (QUICK_WAIT ? "true" : "false") << "\n";
    out << "ENEMY_INVINCIBLE = " << (ENEMY_INVINCIBLE ? "true" : "false") << "\n";
    out << "FRIEND_INVINCIBLE = " << (FRIEND_INVINCIBLE ? "true" : "false") << "\n";
    out << "RESTART = " << (RESTART ? "true" : "false") << "\n";
    out << "SURELY_ESCAPE = " << (SURELY_ESCAPE ? "true" : "false") << "\n";
    out << "SURELY_CONDITION = " << (SURELY_CONDITION ? "true" : "false") << "\n";
    out << "TRANSFIX = " << (TRANSFIX ? "true" : "false") << "\n";
    out << "MDEF_INVALIDATION = " << (MDEF_INVALIDATION ? "true" : "false") << "\n";
    out << "QUICK_EVENT = " << (QUICK_EVENT ? "true" : "false") << "\n";
    out << "SURELY_HIT = " << (SURELY_HIT ? "true" : "false") << "\n";
    out << "SURELY_MISS = " << (SURELY_MISS ? "true" : "false") << "\n";
    out << "SURELY_CRITICAL = " << (SURELY_CRITICAL ? "true" : "false") << "\n";
    out << "MONSTER_STOP_ACTION = " << (MONSTER_STOP_ACTION ? "true" : "false") << "\n";
    out << "DAMAGE_OVER_LIMIT = " << (DAMAGE_OVER_LIMIT ? "true" : "false") << "\n";
    out << "OPEN_ENEMY_HP = " << (OPEN_ENEMY_HP ? "true" : "false") << "\n";
    out << "PHYSICS_RANDOM_MAX = " << (PHYSICS_RANDOM_MAX ? "true" : "false") << "\n";
    out << "MAGIC_RANDOM_MAX = " << (MAGIC_RANDOM_MAX ? "true" : "false") << "\n";
    out << "QUICK_TURN = " << (QUICK_TURN ? "true" : "false") << "\n";
    out << "DRAW_CHAR_TOUCH_RECT = " << (DRAW_CHAR_TOUCH_RECT ? "true" : "false") << "\n";
    out << "MODEL_CHANGE_CHECK = " << (MODEL_CHANGE_CHECK ? "true" : "false") << "\n";
    out << "MODEL_CHANGE_SOUND = " << (MODEL_CHANGE_SOUND ? "true" : "false") << "\n";

    out << "\n[controller]\n";
    out << "world_rumble_intensity = " << world_rumble_intensity << "\n";
    out << "battle_rumble_intensity = " << battle_rumble_intensity << "\n";
    out << "swap_in_vehicle = " << (swap_in_vehicle ? "true" : "false") << "\n";
    out << "use_left_stick = " << (use_left_stick ? "true" : "false") << "\n";
    out << "use_right_stick = " << (use_right_stick ? "true" : "false") << "\n";
    out << "require_modifier_for_battle_pause = " << (require_modifier_for_battle_pause ? "true" : "false") << "\n";

    out << "\n[misc]\n";
    out << "remove_gil_limit = " << (remove_gil_limit ? "true" : "false") << "\n";

    out << "\n[mods]\n";
    for (bridge::OBBArchive& obb : bridge::OBBs) {
        if (obb.path != OBB_FILE)
            out << obb.path << " = " << (obb.enabled ? "true" : "false") << "\n";
    }

    out << "\n[fonts]\n";
    for (const bridge::FontFile& ff : bridge::fonts)
        out << ff.path << " = " << (ff.enabled ? "true" : "false") << "\n";
    out << "font_supersampling = " << font_supersampling << "\n";
    out << "font_scaling_mode = " << (font_scaling_mode == FontScalingMode_Pixelated ? "pixelated" : "smooth") << "\n";

    out.close();

    if (!user_save_separation) {
        appletUnlockExit();
        return;
    }

    const std::string dest = std::string(config::save_filename) + "/config.ini";

    std::ifstream src(CONFIG_PATH, std::ios::binary);
    std::ofstream dst(dest, std::ios::binary);
    if (!src.is_open() || !dst.is_open()) {

        debugPrintf("Failed to copy config to save folder: %s\n", dest.c_str());
        return;
    }
    dst << src.rdbuf();

    appletUnlockExit();
}
}
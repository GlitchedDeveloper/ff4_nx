#pragma once

#include <switch.h>

#define MEMORY_MB 1024
#define FF4_FOLDER ""
#define SO_NAME FF4_FOLDER "libff4.so"
#define OBB_FILE FF4_FOLDER "data.obb"
#define OBB_MODS FF4_FOLDER "mods/"
#define FONTS_DIR FF4_FOLDER "fonts/"
#define CONFIG_PATH FF4_FOLDER "config.ini"
#define CONTROLS_PATH FF4_FOLDER "controls.ini"
#define LOG_NAME FF4_FOLDER "latest.log"
#define DATA_PATH FF4_FOLDER "data"
#define SAVE_FILENAME FF4_FOLDER "save"
#define OPENING_FILE FF4_FOLDER "opening.mp4"
#define ACHIEVEMENTS_PATH FF4_FOLDER "achievements/"
#define IMGUI_FONT_SIZE 33.0f
#define SCREEN_WIDTH 1920.0f
#define SCREEN_HEIGHT 1080.0f

#define DEBUG_LOG 0

#ifdef __cplusplus

#include <string>
#include <unordered_map>
#include <vector>

namespace config {
extern int screen_width;
extern int screen_height;
extern char save_filename[256];
extern bool user_save_separation;
extern int joycon_hold_style;
extern bool multiplayer_enabled;
extern bool skip_launcher;
extern bool dfps_battles_enabled;
extern float dfps_battles_mult;
extern int game_startup_part;
extern bool show_fps;
extern bool remove_newgameplus_limit;
extern bool clean_title;
extern int language;
extern int audio_language;
extern bool show_secret_achievements;
extern bool show_achievement_popup;
extern bool log_to_file;
extern bool show_frame_graph;
extern bool limit_movie_fps;
extern bool show_game_speed_mult;
extern bool guaranteed_encounter;
extern bool equip_anything;
extern bool marked_for_write;
extern float world_rumble_intensity;
extern float battle_rumble_intensity;
extern bool show_ups;
extern bool show_update_graph;
extern bool SURELY_MAX_DAMAGE;
extern bool QUICK_WAIT;
extern bool ENEMY_INVINCIBLE;
extern bool FRIEND_INVINCIBLE;
extern bool RESTART;
extern bool SURELY_ESCAPE;
extern bool SURELY_CONDITION;
extern bool TRANSFIX;
extern bool MDEF_INVALIDATION;
extern bool QUICK_EVENT;
extern bool SURELY_HIT;
extern bool SURELY_MISS;
extern bool SURELY_CRITICAL;
extern bool MONSTER_STOP_ACTION;
extern bool DAMAGE_OVER_LIMIT;
extern bool OPEN_ENEMY_HP;
extern bool PHYSICS_RANDOM_MAX;
extern bool MAGIC_RANDOM_MAX;
extern bool QUICK_TURN;
extern bool DRAW_CHAR_TOUCH_RECT;
extern bool MODEL_CHANGE_CHECK;
extern bool MODEL_CHANGE_SOUND;
extern std::vector<std::string> mod_order;
extern std::unordered_map<std::string, bool> mods;
extern std::vector<std::string> font_order;
extern std::unordered_map<std::string, bool> fonts;
extern bool use_left_stick;
extern bool use_right_stick;
extern bool reobtainable_limit_break;
extern bool remove_gil_limit;
extern bool swap_in_vehicle;
extern bool require_modifier_for_battle_pause;
extern bool augmentless_stat_growth;
extern bool better_stick_movement;
extern int font_supersampling;
extern enum FontScalingMode {
    FontScalingMode_Smooth    = 0,
    FontScalingMode_Pixelated = 1,
} font_scaling_mode;
extern int internal_font_scale;
extern bool remove_menu_button;
extern bool remove_map_button;
extern bool remove_back_button;
extern bool remove_optimize_button;
extern bool remove_remove_button;
extern bool remove_abilities_button;
extern bool remove_invert_button;
extern bool remove_skip_button;
extern bool remove_disembark_button;
extern bool remove_sort_button;

void read_config(const char* file);
void mark_for_write();
void write_config();
}
#endif

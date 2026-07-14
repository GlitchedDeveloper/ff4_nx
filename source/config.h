#pragma once

#include <switch.h>

#define MEMORY_MB 1024
#define SO_NAME "ff4/libff4.so"
#define OBB_FILE "ff4/data.obb"
#define OBB_MODS "ff4/mods/"
#define FONTS_DIR "ff4/fonts/"
#define CONFIG_PATH "ff4/config.ini"
#define CONTROLS_PATH "ff4/controls.ini"
#define LOG_NAME "ff4/latest.log"
#define DATA_PATH "ff4/data"
#define SAVE_FILENAME "ff4/save"
#define OPENING_FILE "ff4/opening.mp4"
#define ACHIEVEMENTS_PATH "ff4/achievements/"
#define IMGUI_FONT_SIZE 33.0f

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
extern bool hide_back_button;
extern bool hide_vehicle_buttons;
extern bool remove_newgameplus_limit;
extern bool fix_titlepart;
extern int language;
extern int audio_language;
extern bool show_secret_achievements;
extern bool show_achievement_popup;
extern bool log_to_file;
extern bool show_frame_graph;
extern bool overclock_movie_cpu;
extern bool overclock_movie_mem;
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

void read_config(const char* file);
void mark_for_write();
void write_config();
}
#endif

#pragma once

#include "types.h"

namespace achievement {
void init();
void draw();
void unlock(const char* identifier);
void read_achievements();
bool is_unlocked(u8 id);
int get_count();
const char* get_name(u8 id);
const char* get_description(u8 id);
bool is_secret(u8 id);
unsigned int get_texture(u8 id);
float get_progress(u8 id);
void reset_progress_cache();
}
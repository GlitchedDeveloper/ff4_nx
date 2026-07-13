#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

#include "types.h"

namespace bridge {

struct OBBArchive
{
    bool enabled = true;
    std::string path;
    unsigned char* header = nullptr;
    int header_length     = 0;

    unsigned char* m476a(char* str, int* file_length);

    uint8_t isFileExist(char* str);
};

extern std::vector<OBBArchive> OBBs;

extern s32 framerate;
typedef struct
{
    unsigned char* elements;
    int size;
} jni_bytearray;

void applyModOrder();
void buildFileMap();
jni_bytearray* loadFile(char* str);
unsigned char* loadFile(const char* str, int* size);
jni_bytearray* loadSound(char* str);
jni_bytearray* loadRawFile(char* str);
jni_bytearray* getSaveFileName();
jni_bytearray* getSaveDataPath();
uint8_t isSoundFileExist(char* str);
void setFPS(s32 i);
float getFPSMultiplier();
void halfFPSMultiplier();
void doubleFPSMultiplier();
void createSaveFile(size_t size);
void createAchieveFile(size_t size);
extern u64 g_UpdateCount;
extern u64 g_ElapsedTicks;
u64 getTargetUPS();
u64 getCurrentFrame(u64 lastUpdate);
bool isUpdateFrame();
int readHeader(const char* filepath);

typedef struct
{
    int* elements;
    int size;
} jni_intarray;

jni_intarray* loadTexture(jni_bytearray* bArr);
int isDeviceAndroidTV();
jni_intarray* drawFont(char* str, int i, int i2, int i3);

void createEditText(char* str);
char* getEditText();
void initFont();
void reinitFont();

int getCurrentLanguage();

void playMovie();
u8 getMovieState();
void stopMovie();
bool isMoviePlaying();
}

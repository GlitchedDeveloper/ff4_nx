/* jni.cpp -- Fake Java Native Interface
 *
 * Copyright (C) 2026 GlitchedDeveloper, givethesourceplox, Andy Nguyen
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

#include <GLES/egl.h>

#include <cstdarg>
#include <cstdlib>
#include <cstring>

#include "../achievement.h"
#include "../bridge.h"
#include "../config.h"
#include "../pad_manager.h"
#include "../patches.h"
#include "../so_util.h"
#include "../util.h"

using namespace bridge;

namespace patches::jni {

int this_width;
int this_height;
void updateViewportSize(int32_t width, int32_t height, uint8_t portrait) {
    int i3 = width * 3;
    if (i3 >= height * 4) {
        this_height = height;
        this_width  = width;
        if (this_width > (this_height * 21) / 9) {
            this_width = (this_height * 21) / 9;
        }
    } else {
        this_width  = width;
        this_height = i3 / 4;
    }
    int x = (width - this_width) / 2;
    int y = (height - this_height) / 2;
    glViewport(x, y, this_width, this_height);
}

void* FindClass(void) { return (void*)0x41414141; }
void DeleteLocalRef(void* env, void* localRef) { return; }

enum MethodIDs
{
    UNKNOWN = 0,
    GET_CURRENT_FRAME,
    LOAD_FILE,
    LOAD_RAW_FILE,
    GET_LANGUAGE,
    GET_SAVEFILENAME,
    CREATE_SAVEFILE,
    LOAD_TEXTURE,
    IS_DEVICE_ANDROID_TV,
    DRAW_FONT,
    CREATE_EDIT_TEXT,
    GET_EDIT_TEXT,
    GET_RES_WIDTH,
    GET_RES_HEIGHT,
    GET_VIEW_X,
    GET_VIEW_Y,
    GET_VIEW_W,
    GET_VIEW_H,
    UPDATE_VIEWPORT_SIZE,
    SET_FPS,
    IS_OK_ACHIEVEMENT,
    GET_KEY_EVENT,
    LOAD_SOUND,
    GET_SAVE_DATA_PATH,
    GET_DOWNLOAD_STATE,
    IS_SOUND_FILE_EXIST,
    PLAY_MOVIE,
    GET_MOVIE_STATE,
    STOP_MOVIE,
    CREATE_ACHIEVE_FILE,
    UNLOCK_ACHIEVEMENT
} MethodIDs;

typedef struct
{
    const char* name;
    enum MethodIDs id;
} NameToMethodID;

static NameToMethodID name_to_method_ids[] = {
    { "getCurrentFrame", GET_CURRENT_FRAME },
    { "loadFile", LOAD_FILE },
    { "loadRawFile", LOAD_RAW_FILE },
    { "getLanguage", GET_LANGUAGE },
    { "getLanguage2", GET_LANGUAGE },
    { "getSaveFileName", GET_SAVEFILENAME },
    { "createSaveFile", CREATE_SAVEFILE },
    { "loadTexture", LOAD_TEXTURE },
    { "isDeviceAndroidTV", IS_DEVICE_ANDROID_TV },
    { "getDeviceAndroidTV", IS_DEVICE_ANDROID_TV },
    { "drawFont", DRAW_FONT },
    { "createEditText", CREATE_EDIT_TEXT },
    { "getEditText", GET_EDIT_TEXT },
    { "getResWidth", GET_RES_WIDTH },
    { "getResHeight", GET_RES_HEIGHT },
    { "getViewPosX", GET_VIEW_X },
    { "getViewPosY", GET_VIEW_Y },
    { "getViewWidth", GET_VIEW_W },
    { "getViewHeight", GET_VIEW_H },
    { "updateViewportSize", UPDATE_VIEWPORT_SIZE },
    { "setFPS", SET_FPS },
    { "isOKAchievement", IS_OK_ACHIEVEMENT },
    { "getKeyEvent", GET_KEY_EVENT },
    { "loadSound", LOAD_SOUND },
    { "getSaveDataPath", GET_SAVE_DATA_PATH },
    { "getDownloadState", GET_DOWNLOAD_STATE },
    { "isSoundFileExist", IS_SOUND_FILE_EXIST },
    { "playMovie", PLAY_MOVIE },
    { "getMovieState", GET_MOVIE_STATE },
    { "stopMovie", STOP_MOVIE },
    { "getStoragePath", GET_SAVEFILENAME },
    { "createAchieveFile", CREATE_ACHIEVE_FILE },
    { "unlockAchievement", UNLOCK_ACHIEVEMENT },
};

int GetMethodID(void* env, void* jclass, const char* name, const char* sig) {
    for (int i = 0; i < sizeof(name_to_method_ids) / sizeof(NameToMethodID); i++) {
        if (strcmp(name, name_to_method_ids[i].name) == 0)
            return name_to_method_ids[i].id;
    }
    return UNKNOWN;
}

int CallBooleanMethodV(void* env, void* obj, int methodID, uintptr_t* args) {
    switch (methodID) {
        case IS_OK_ACHIEVEMENT:
            return true;
        default:
            return false;
    }
}

float CallFloatMethodV(void* env, void* obj, int methodID, uintptr_t* args) {
    return 0.0f;
}

int CallIntMethodV(void* env, void* obj, int methodID, uintptr_t* args) {
    return 0;
}

void* CallObjectMethodV(void* env, void* obj, int methodID, uintptr_t* args) {
    return NULL;
}

void CallVoidMethodV(void* env, void* obj, int methodID, uintptr_t* args) {
    return;
}

int GetFieldID(void* env, void* clazz, const char* name, const char* sig) {
    return 0;
}

int GetFloatField(void* env, void* obj, int fieldID) { return 0; }

int GetStaticMethodID(void* env, void* jclass, const char* name, const char* sig) {
    for (int i = 0; i < sizeof(name_to_method_ids) / sizeof(NameToMethodID); i++) {
        if (strcmp(name, name_to_method_ids[i].name) == 0)
            return name_to_method_ids[i].id;
    }

    return UNKNOWN;
}

void* CallStaticObjectMethodV(void* env, void* obj, int methodID, uintptr_t* args) {
    switch (methodID) {
        case LOAD_FILE:
            return loadFile((char*)args[0]);
        case LOAD_SOUND:
            return loadSound((char*)args[0]);
        case LOAD_RAW_FILE:
            return loadRawFile((char*)args[0]);
        case GET_SAVEFILENAME:
            return getSaveFileName();
        case LOAD_TEXTURE:
            return loadTexture((jni_bytearray*)args[0]);
        case DRAW_FONT:
            return drawFont((char*)args[0], args[1], args[2], args[3]);
        case GET_EDIT_TEXT:
            return getEditText();
        case GET_SAVE_DATA_PATH:
            return getSaveDataPath();
        default:
            return NULL;
    }
}
void CallStaticVoidMethodV(void* env, void* obj, int methodID, uintptr_t* args) {
    switch (methodID) {
        case CREATE_SAVEFILE:
            createSaveFile((size_t)args[0]);
            break;
        case CREATE_EDIT_TEXT:
            createEditText((char*)args[0]);
            break;
        case UPDATE_VIEWPORT_SIZE:
            updateViewportSize((int32_t)args[0], (int32_t)args[1], (uint8_t)args[2]);
            break;
        case SET_FPS:
            setFPS((int32_t)args[0]);
            break;
        case PLAY_MOVIE:
            playMovie();
            break;
        case STOP_MOVIE:
            stopMovie();
            break;
        case CREATE_ACHIEVE_FILE:
            createAchieveFile((size_t)args[0]);
            break;
        case UNLOCK_ACHIEVEMENT:
            achievement::unlock((char*)args[0]);
            break;
        default:
            return;
    }
}

int CallStaticBooleanMethodV(void* env, void* obj, int methodID, uintptr_t* args) {
    switch (methodID) {
        case IS_OK_ACHIEVEMENT:
            return 1;
        case IS_SOUND_FILE_EXIST:
            return isSoundFileExist((char*)args[0]);
        case IS_DEVICE_ANDROID_TV:
            return isDeviceAndroidTV();
        case GET_MOVIE_STATE:
            return getMovieState();
        default:
            return 0;
    }
}

uint64_t CallStaticLongMethodV(void* env, void* obj, int methodID, uintptr_t* args) {
    switch (methodID) {
        case GET_CURRENT_FRAME:
            return getCurrentFrame((uint64_t)args[0]);
        default:
            return 0;
    }
}

int CallStaticIntMethodV(void* env, void* obj, int methodID, uintptr_t* args) {
    switch (methodID) {
        case GET_LANGUAGE:
            return getCurrentLanguage();
        case GET_VIEW_X:
        case GET_VIEW_Y:
            return 0;
        case GET_VIEW_W:
        case GET_RES_WIDTH:
            return config::screen_width;
        case GET_VIEW_H:
        case GET_RES_HEIGHT:
            return config::screen_height;
        case GET_DOWNLOAD_STATE:
            return 0;
        case GET_KEY_EVENT:
            return pad_manager::getKeyEvent();
        default:
            return 0;
    }
}

float CallStaticFloatMethodV(void* env, void* obj, int methodID, uintptr_t* args) {
    switch (methodID) {
        default:
            return 0.0f;
    }
}

char* NewStringUTF(void* env, char* bytes) { return bytes; }

int GetStringUTFLength(void* env, char* string) {
    return string ? (int)strlen(string) : 0;
}

char* GetStringUTFChars(void* env, char* string, int* isCopy) {
    if (isCopy)
        *isCopy = 0;
    return string;
}

void ReleaseStringUTFChars(void* env, char* string, char* utf) {
    return;
}

int GetArrayLength(void* env, jni_bytearray* obj) {
    return obj ? obj->size : 0;
}

void* GetByteArrayElements(void* env, jni_bytearray* obj, int* isCopy) {
    if (isCopy)
        *isCopy = 0;
    return obj ? obj->elements : NULL;
}

void* NewByteArray(void* env, size_t length) {
    jni_bytearray* result = reinterpret_cast<jni_bytearray*>(malloc(sizeof(jni_bytearray)));
    if (!result)
        return NULL;
    result->elements = reinterpret_cast<unsigned char*>(malloc(length));
    if (!result->elements) {
        free(result);
        return NULL;
    }
    result->size = length;
    return result;
}

void* GetIntArrayElements(void* env, jni_intarray* obj, int* isCopy) {
    if (isCopy)
        *isCopy = 0;
    return obj ? obj->elements : NULL;
}

int ReleaseIntArrayElements(void* env, jni_intarray* obj, void* elems, int mode) {
    return 0;
}

int ReleaseByteArrayElements(void* env, jni_bytearray* obj, void* elems, int mode) {
    return 0;
}

void SetByteArrayRegion(void* env, jni_bytearray* array, size_t start, size_t len, const unsigned char* buf) {
    if (array && array->elements && buf) {
        memcpy((unsigned char*)array->elements + start, buf, len);
    }
}

#define MAX_JNI_ARGS 10

#define PACK_VA_ARGS(args_array, methodID)       \
    va_list args;                                \
    va_start(args, methodID);                    \
    uintptr_t args_array[MAX_JNI_ARGS];          \
    for (int i = 0; i < MAX_JNI_ARGS; i++) {     \
        args_array[i] = va_arg(args, uintptr_t); \
    }                                            \
    va_end(args);

int CallBooleanMethod(void* env, void* obj, int methodID, ...) {
    PACK_VA_ARGS(args_array, methodID)
    return CallBooleanMethodV(env, obj, methodID, args_array);
}

float CallFloatMethod(void* env, void* obj, int methodID, ...) {
    PACK_VA_ARGS(args_array, methodID)
    return CallFloatMethodV(env, obj, methodID, args_array);
}

int CallIntMethod(void* env, void* obj, int methodID, ...) {
    PACK_VA_ARGS(args_array, methodID)
    return CallIntMethodV(env, obj, methodID, args_array);
}

void* CallObjectMethod(void* env, void* obj, int methodID, ...) {
    PACK_VA_ARGS(args_array, methodID)
    return CallObjectMethodV(env, obj, methodID, args_array);
}

void CallVoidMethod(void* env, void* obj, int methodID, ...) {
    PACK_VA_ARGS(args_array, methodID)
    CallVoidMethodV(env, obj, methodID, args_array);
}

void* CallStaticObjectMethod(void* env, void* obj, int methodID, ...) {
    PACK_VA_ARGS(args_array, methodID)
    return CallStaticObjectMethodV(env, obj, methodID, args_array);
}

void CallStaticVoidMethod(void* env, void* obj, int methodID, ...) {
    PACK_VA_ARGS(args_array, methodID)
    CallStaticVoidMethodV(env, obj, methodID, args_array);
}

int CallStaticBooleanMethod(void* env, void* obj, int methodID, ...) {
    PACK_VA_ARGS(args_array, methodID)
    return CallStaticBooleanMethodV(env, obj, methodID, args_array);
}

uint64_t CallStaticLongMethod(void* env, void* obj, int methodID, ...) {
    PACK_VA_ARGS(args_array, methodID)
    return CallStaticLongMethodV(env, obj, methodID, args_array);
}

int CallStaticIntMethod(void* env, void* obj, int methodID, ...) {
    PACK_VA_ARGS(args_array, methodID)
    return CallStaticIntMethodV(env, obj, methodID, args_array);
}

float CallStaticFloatMethod(void* env, void* obj, int methodID, ...) {
    PACK_VA_ARGS(args_array, methodID)
    return CallStaticFloatMethodV(env, obj, methodID, args_array);
}

s8 fake_env[0x1000];
int InitEnv() {
    for (int i = 0; i < (int)(sizeof(fake_env) / sizeof(uintptr_t)); i++)
        ((uintptr_t*)fake_env)[i] = (uintptr_t)ret0;

    *(uintptr_t*)(fake_env + 0x00)  = (uintptr_t)fake_env;
    *(uintptr_t*)(fake_env + 0x30)  = (uintptr_t)FindClass;
    *(uintptr_t*)(fake_env + 0x88)  = (uintptr_t)ret0;
    *(uintptr_t*)(fake_env + 0xB8)  = (uintptr_t)DeleteLocalRef;
    *(uintptr_t*)(fake_env + 0x108) = (uintptr_t)GetMethodID;
    *(uintptr_t*)(fake_env + 0x118) = (uintptr_t)CallObjectMethodV;
    *(uintptr_t*)(fake_env + 0x130) = (uintptr_t)CallBooleanMethodV;
    *(uintptr_t*)(fake_env + 0x190) = (uintptr_t)CallIntMethodV;
    *(uintptr_t*)(fake_env + 0x1C0) = (uintptr_t)CallFloatMethodV;
    *(uintptr_t*)(fake_env + 0x1F0) = (uintptr_t)CallVoidMethodV;
    *(uintptr_t*)(fake_env + 0x2F0) = (uintptr_t)GetFieldID;
    *(uintptr_t*)(fake_env + 0x320) = (uintptr_t)GetFloatField;
    *(uintptr_t*)(fake_env + 0x388) = (uintptr_t)GetStaticMethodID;
    *(uintptr_t*)(fake_env + 0x398) = (uintptr_t)CallStaticObjectMethodV;
    *(uintptr_t*)(fake_env + 0x3A8) = (uintptr_t)CallStaticBooleanMethodV;
    *(uintptr_t*)(fake_env + 0x3B8) = (uintptr_t)CallStaticIntMethodV;
    *(uintptr_t*)(fake_env + 0x3C8) = (uintptr_t)CallStaticFloatMethodV;
    *(uintptr_t*)(fake_env + 0x3D8) = (uintptr_t)CallStaticLongMethodV;
    *(uintptr_t*)(fake_env + 0x3E8) = (uintptr_t)CallStaticVoidMethodV;
    *(uintptr_t*)(fake_env + 0x480) = (uintptr_t)ret0;
    *(uintptr_t*)(fake_env + 0x538) = (uintptr_t)NewStringUTF;
    *(uintptr_t*)(fake_env + 0x540) = (uintptr_t)GetStringUTFLength;
    *(uintptr_t*)(fake_env + 0x548) = (uintptr_t)GetStringUTFChars;
    *(uintptr_t*)(fake_env + 0x550) = (uintptr_t)ReleaseStringUTFChars;
    *(uintptr_t*)(fake_env + 0x558) = (uintptr_t)GetArrayLength;
    *(uintptr_t*)(fake_env + 0x580) = (uintptr_t)NewByteArray;
    *(uintptr_t*)(fake_env + 0x5c0) = (uintptr_t)GetByteArrayElements;
    *(uintptr_t*)(fake_env + 0x5d8) = (uintptr_t)GetIntArrayElements;
    *(uintptr_t*)(fake_env + 0x600) = (uintptr_t)ReleaseByteArrayElements;
    *(uintptr_t*)(fake_env + 0x618) = (uintptr_t)ReleaseIntArrayElements;
    *(uintptr_t*)(fake_env + 0x680) = (uintptr_t)SetByteArrayRegion;

    return 0;
}

void patch() {
    so::hook(so::findAddr_rw("_ZN7_JNIEnv22CallStaticObjectMethodEP7_jclassP10_jmethodIDz"), (uintptr_t)CallStaticObjectMethod);
    so::hook(so::findAddr_rw("_ZN7_JNIEnv23CallStaticBooleanMethodEP7_jclassP10_jmethodIDz"), (uintptr_t)CallStaticBooleanMethod);
    so::hook(so::findAddr_rw("_ZN7_JNIEnv19CallStaticIntMethodEP7_jclassP10_jmethodIDz"), (uintptr_t)CallStaticIntMethod);
    so::hook(so::findAddr_rw("_ZN7_JNIEnv20CallStaticLongMethodEP7_jclassP10_jmethodIDz"), (uintptr_t)CallStaticLongMethod);
    so::hook(so::findAddr_rw("_ZN7_JNIEnv20CallStaticVoidMethodEP7_jclassP10_jmethodIDz"), (uintptr_t)CallStaticVoidMethod);
    InitEnv();
}
}
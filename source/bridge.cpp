#include "bridge.h"

#include <dirent.h>
#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <switch.h>
#include <zlib.h>

#include <queue>

#include "game.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/imgutils.h>
#include <libswresample/swresample.h>
#include <libswscale/swscale.h>
}

#include <GLES/gl.h>

#include "audio/akb.h"
#include "config.h"
#include "stb_image.h"
#include "stb_truetype.h"
#include "util.h"

namespace bridge {
void decodeArray(unsigned char* bArr, int size, uint key) {
    for (int n = 0; n < size; n++) {
        key     = key * 0x41c64e6d + 0x3039;
        bArr[n] = bArr[n] ^ (unsigned char)(key >> 0x18);
    }
}

static int getInt(unsigned char* bArr, int i) {
    return *(unsigned int*)(&bArr[i]);
}

unsigned char* gzipRead(unsigned char* bArr, int* bArr_length) {
    unsigned int readInt = __builtin_bswap32(getInt(bArr, 0));
    unsigned char* bArr2 = reinterpret_cast<unsigned char*>(calloc(readInt, sizeof(unsigned char)));
    unsigned char* bArr3 = &bArr[4];

    z_stream infstream;
    infstream.zalloc = Z_NULL;
    infstream.zfree  = Z_NULL;
    infstream.opaque = Z_NULL;
    // setup "b" as the input and "c" as the compressed output
    infstream.avail_in  = *bArr_length - 4; // size of input
    infstream.next_in   = bArr3; // input char array
    infstream.avail_out = readInt; // size of output
    infstream.next_out  = bArr2; // output char array

    // the actual DE-compression work.
    inflateInit2(&infstream, MAX_WBITS | 16);
    int ret = inflate(&infstream, Z_FULL_FLUSH);
    inflateEnd(&infstream);

    if (ret != Z_STREAM_END) {
        debugPrintf("gzipRead: inflate failed with code %d\n", ret);
        free(bArr2);
        return nullptr;
    }

    *bArr_length = readInt;
    return bArr2;
}

struct FileLocation
{
    int obbIndex;
    int entryOffset;
};

std::unordered_map<std::string, std::vector<FileLocation>> fileMap;

void applyModOrder() {
    if (OBBs.empty())
        return;

    std::vector<bool> consumed(OBBs.size(), false);
    std::vector<OBBArchive> reordered;
    reordered.reserve(OBBs.size());

    for (const std::string& p : config::mod_order) {
        for (size_t i = 0; i < OBBs.size(); i++) {
            if (consumed[i])
                continue;
            if (OBBs[i].path == p) {
                reordered.push_back(std::move(OBBs[i]));
                consumed[i] = true;
                break;
            }
        }
    }
    for (size_t i = 0; i < OBBs.size(); i++) {
        if (consumed[i])
            continue;
        reordered.push_back(std::move(OBBs[i]));
    }

    OBBs = std::move(reordered);
}

void buildFileMap() {
    fileMap.clear();

    for (int obbIdx = 0; obbIdx < (int)OBBs.size(); obbIdx++) {
        OBBArchive& obb = OBBs[obbIdx];
        int count       = getInt(obb.header, 0);
        for (int i = 0; i < count; i++) {
            int i4      = i * 12;
            int namePtr = getInt(obb.header, i4 + 4);

            std::string filename(reinterpret_cast<const char*>(&obb.header[namePtr]));
            fileMap[filename].push_back({ obbIdx, i4 });
        }
    }

    debugPrintf("buildFileMap: indexed %zu files across %zu OBBs\n",
        fileMap.size(), OBBs.size());
}

static unsigned char* loadFromMap(const std::string& path, int* size) {
    auto it = fileMap.find(path);
    if (it == fileMap.end())
        return nullptr;

    for (const FileLocation& loc : it->second) {
        OBBArchive& obb = OBBs[loc.obbIndex];
        if (!obb.enabled)
            continue;

        int i  = loc.entryOffset + 8;
        int a3 = getInt(obb.header, i);
        *size  = getInt(obb.header, i + 4);

        FILE* fp = fopen(obb.path.c_str(), "rb");
        if (!fp)
            continue;

        fseek(fp, a3, SEEK_SET);
        unsigned char* bArr2 = reinterpret_cast<unsigned char*>(malloc(*size));
        for (int i7 = 0; i7 < *size;
            i7 += fread(&bArr2[i7], sizeof(unsigned char), *size - i7, fp)) {
        }
        fclose(fp);

        decodeArray(bArr2, *size, a3 + 419430400u);
        unsigned char* result = gzipRead(bArr2, size);
        free(bArr2);

        if (result != nullptr)
            return result;
    }

    return nullptr;
}

static u8 isFileExistInMap(const std::string& path) {
    auto it = fileMap.find(path);
    if (it == fileMap.end())
        return false;

    for (const FileLocation& loc : it->second) {
        if (OBBs[loc.obbIndex].enabled)
            return true;
    }
    return false;
}

unsigned char* OBBArchive::m476a(char* str, int* file_length) {
    if (!enabled)
        return nullptr;
    int i;

    unsigned char* bArr = header;
    if (bArr != NULL) {
        int a  = getInt(bArr, 0);
        int i2 = 0;
        i      = 0;
        while (a > i2) {
            int i3 = (i2 + a) / 2;
            int i4 = i3 * 12;
            int a2 = getInt(header, i4 + 4);
            int i5 = 0;
            for (int i6 = 0; i6 < strlen(str) && i5 == 0; i6++) {
                i5 = (header[a2 + i6] & 0xFF) - (str[i6] & 0xFF);
            }
            if (i5 == 0) {
                i5 = header[a2 + strlen(str)] & 0xFF;
            }
            if (i5 == 0) {
                i  = i4 + 8;
                a  = i3;
                i2 = a;
            } else if (i5 > 0) {
                a = i3;
            } else {
                i2 = i3 + 1;
            }
        }
    } else {
        i = 0;
    }
    if (i == 0) {
        return NULL;
    }

    FILE* fp = fopen(this->path.c_str(), "rb");
    int a3   = getInt(header, i);
    fseek(fp, a3, SEEK_SET);

    *file_length         = getInt(header, i + 4);
    unsigned char* bArr2 = reinterpret_cast<unsigned char*>(malloc(*file_length));

    for (int i7 = 0; i7 < *file_length;
        i7 += fread(&bArr2[i7], sizeof(unsigned char), *file_length - i7, fp)) {
    }

    fclose(fp);

    decodeArray(bArr2, *file_length, a3 + 419430400u);

    unsigned char* a4 = gzipRead(bArr2, file_length);

    free(bArr2);

    return a4;
}

u8 OBBArchive::isFileExist(char* str) {
    if (!enabled)
        return false;
    int i;
    unsigned char* bArr = header;
    if (bArr != NULL) {
        int a  = getInt(bArr, 0);
        int i2 = 0;
        i      = 0;
        while (a > i2) {
            int i3 = (i2 + a) / 2;
            int i4 = i3 * 12;
            int a2 = getInt(header, i4 + 4);
            int i5 = 0;
            for (int i6 = 0; i6 < strlen(str) && i5 == 0; i6++) {
                i5 = (header[a2 + i6] & 0xFF) - (str[i6] & 0xFF);
            }
            if (i5 == 0) {
                i5 = header[a2 + strlen(str)] & 0xFF;
            }
            if (i5 == 0) {
                i  = i4 + 8;
                a  = i3;
                i2 = a;
            } else if (i5 > 0) {
                a = i3;
            } else {
                i2 = i3 + 1;
            }
        }
    } else {
        i = 0;
    }
    if (i == 0) {
        return false;
    }

    return true;
}

std::vector<OBBArchive> OBBs;

int readHeader(const char* filepath) {
    FILE* fp = fopen(filepath, "rb");
    fseek(fp, 0L, SEEK_END);
    int length = ftell(fp);
    fseek(fp, 0L, SEEK_SET);

    unsigned char bArr[16];

    for (int i = 0; i < 16;
        i += fread(&bArr[i], sizeof(unsigned char), 16 - i, fp)) {
    }

    decodeArray(bArr, 16, 419430400u);

    if (getInt(bArr, 0) != 826495553) {
        debugPrintf("initFileTable: Header Error\n");
        fclose(fp);
        return 0;
    } else if (length != getInt(bArr, 4)) {
        debugPrintf("initFileTable: Size Error\n");
        fclose(fp);
        return 0;
    } else {
        unsigned int a2 = getInt(bArr, 8);
        OBBArchive obb;

        obb.path          = filepath;
        obb.header_length = getInt(bArr, 12);
        obb.header        = reinterpret_cast<unsigned char*>(malloc(obb.header_length));

        fseek(fp, (long)(a2 - 16), SEEK_CUR);

        for (int i = 0; i < obb.header_length;
            i += fread(&obb.header[i], sizeof(unsigned char), obb.header_length - i, fp)) {
        }

        decodeArray(obb.header, obb.header_length, a2 + 419430400u);

        unsigned char* header2 = gzipRead(obb.header, &obb.header_length);

        free(obb.header);

        obb.header = header2;

        fclose(fp);

        auto it = config::mods.find(filepath);
        if (it != config::mods.end())
            obb.enabled = it->second;

        OBBs.push_back(obb);
    }
    return 1;
}

unsigned char* readFile(char* path, int* size) {
    char temp_path[512];
    sprintf(temp_path, DATA_PATH "/%s", path);

    FILE* fp = fopen(temp_path, "rb");
    if (!fp)
        return NULL;

    fseek(fp, 0, SEEK_END);
    *size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    unsigned char* buffer = reinterpret_cast<unsigned char*>(malloc(*size));
    if (!buffer) {
        fclose(fp);
        return NULL;
    }

    fread(buffer, 1, *size, fp);

    fclose(fp);

    return buffer;
}

unsigned char* decodeString(unsigned char* bArr, int* bArr_length) {
    return bArr;
}

const char* lang[] = { "ja", "en", "fr", "de", "it", "es", "zh_CN", "zh_TW", "ko", "pt_BR", "ru", "th" };

unsigned char* loadFile(const char* str, int* size) {
    char temp_path[512];
    unsigned char* data = nullptr;

    if (strncmp(str, "voice/", 6) == 0) {
        if (config::audio_language > -1) {
            const char* lang_code = lang[config::audio_language];
            snprintf(temp_path, sizeof(temp_path), "files/SOUND/VOICE/%s%s", lang_code, &str[8]);
            data = loadFromMap(temp_path, size);
            if (data)
                return data;
        }
        snprintf(temp_path, sizeof(temp_path), "files/SOUND/VOICE/%s", &str[6]);
        data = loadFromMap(temp_path, size);
        if (data)
            return data;
    }

    snprintf(temp_path, sizeof(temp_path), "%s.lproj/%s", lang[getCurrentLanguage()], str);
    data = loadFromMap(temp_path, size);
    if (data)
        return data;

    snprintf(temp_path, sizeof(temp_path), "files/%s", str);
    data = loadFromMap(temp_path, size);
    if (data)
        return data;

    snprintf(temp_path, sizeof(temp_path), "files/SOUND/BGM/%s", str);
    data = loadFromMap(temp_path, size);
    if (data)
        return data;

    snprintf(temp_path, sizeof(temp_path), "files/SOUND/SE/%s", str);
    data = loadFromMap(temp_path, size);
    if (data)
        return data;

    return nullptr;
}

jni_bytearray* loadFile(char* str) {
    debugPrintf("loadFile(%s)\n", str);

    char temp_path[512];
    int file_length;
    unsigned char* a;

    snprintf(temp_path, sizeof(temp_path), "%s.lproj/%s", lang[getCurrentLanguage()], str);
    a = loadFromMap(temp_path, &file_length);
    if (a == nullptr) {
        snprintf(temp_path, sizeof(temp_path), "files/%s", str);
        a = loadFromMap(temp_path, &file_length);
    }

    if (a == nullptr)
        return nullptr;

    jni_bytearray* result = reinterpret_cast<jni_bytearray*>(malloc(sizeof(jni_bytearray)));
    result->elements      = a;
    result->size          = file_length;

    char* substring = strrchr(str, 46);
    substring       = substring == nullptr ? str : substring;

    if (strcmp(substring, ".msd") || str[0] == 'e')
        return result;
    if (str[0] == 'n' && str[1] == 'o' && str[2] == 'a')
        return result;

    unsigned char* b = decodeString(a, &file_length);
    if (b != a)
        free(a);

    result->elements = b;
    result->size     = file_length;
    return result;
}

jni_bytearray* loadRawFile(char* str) {
    int file_length;
    unsigned char* a = loadFromMap(str, &file_length);
    if (a == nullptr)
        return nullptr;

    jni_bytearray* result = reinterpret_cast<jni_bytearray*>(malloc(sizeof(jni_bytearray)));
    result->elements      = a;
    result->size          = file_length;
    return result;
}

jni_bytearray* loadSound(char* str) {
    return nullptr;
}

u8 isSoundFileExist(char* str) {
    char str2[128], path[256];
    if (strlen(str) == 0 || !strstr(str, "voice/")) {
        sprintf(str2, "%s.akb", str);
    } else {
        if (config::audio_language > -1) {
            const char* lang_code = lang[config::audio_language];
            sprintf(str2, "%s%s", lang_code, &str[8]);
        } else {
            sprintf(str2, "%s", &str[6]);
        }
    }

    sprintf(path, "files/SOUND/BGM/%s", str2);
    if (isFileExistInMap(path))
        return true;
    sprintf(path, "files/SOUND/SE/%s", str2);
    if (isFileExistInMap(path))
        return true;
    sprintf(path, "files/SOUND/VOICE/%s", str2);
    if (isFileExistInMap(path))
        return true;

    return false;
}

jni_bytearray* getSaveFileName() {

    const char* buffer    = config::save_filename;
    jni_bytearray* result = reinterpret_cast<jni_bytearray*>(malloc(sizeof(jni_bytearray)));
    result->elements      = reinterpret_cast<unsigned char*>(malloc(strlen(buffer) + 1));
    strcpy((char*)result->elements, buffer);
    result->size = strlen(buffer) + 1;

    return result;
}

jni_bytearray* getSaveDataPath() {

    char save_filename[256];
    sprintf(save_filename, "%s/save.bin", config::save_filename);
    const char* buffer    = save_filename;
    jni_bytearray* result = reinterpret_cast<jni_bytearray*>(malloc(sizeof(jni_bytearray)));
    result->elements      = reinterpret_cast<unsigned char*>(malloc(strlen(buffer) + 1));
    strcpy((char*)result->elements, buffer);
    result->size = strlen(buffer) + 1;

    return result;
}

void createSaveFile(size_t size) {
    char* buffer = reinterpret_cast<char*>(malloc(size));
    char save_filename[256];
    sprintf(save_filename, "%s/save.bin", config::save_filename);
    FILE* fd = fopen(save_filename, "wb");
    fwrite(buffer, sizeof(char), size, fd);
    fclose(fd);
    free(buffer);
}

void createAchieveFile(size_t size) {
    char* buffer = reinterpret_cast<char*>(malloc(size));
    char save_filename[256];
    sprintf(save_filename, "%s/report_achi.bin", config::save_filename);
    FILE* fd = fopen(save_filename, "wb");
    fwrite(buffer, sizeof(char), size, fd);
    fclose(fd);
    free(buffer);
}

float g_FPSMultiplier = 1.0f;
float getFPSMultiplier() {
    return g_FPSMultiplier;
}
void halfFPSMultiplier() {
    g_FPSMultiplier /= 2;
}
void doubleFPSMultiplier() {
    g_FPSMultiplier *= 2;
}

static bool g_overclockActive = false;
static void applyMovieOverclock(bool enable) {
    if (enable == g_overclockActive)
        return;
    static u32 original_cpu_hz, original_mem_hz;
    if (enable) {
        if (hosversionAtLeast(8, 0, 0)) {
            clkrstInitialize();
            pcvInitialize();
            ClkrstSession cpu, mem;
            clkrstOpenSession(&cpu, PcvModuleId_CpuBus, 3);
            clkrstOpenSession(&mem, PcvModuleId_EMC, 3);
            clkrstGetClockRate(&cpu, &original_cpu_hz);
            clkrstGetClockRate(&mem, &original_mem_hz);
            if (config::overclock_movie_cpu)
                clkrstSetClockRate(&cpu, 1224000000);
            if (config::overclock_movie_mem)
                clkrstSetClockRate(&mem, 1600000000);
            clkrstCloseSession(&cpu);
            clkrstCloseSession(&mem);
            g_overclockActive = true;
        }
    } else {
        ClkrstSession cpu, mem;
        clkrstOpenSession(&cpu, PcvModuleId_CpuBus, 3);
        clkrstOpenSession(&mem, PcvModuleId_EMC, 3);
        clkrstSetClockRate(&cpu, original_cpu_hz);
        clkrstSetClockRate(&mem, original_mem_hz);
        clkrstCloseSession(&cpu);
        clkrstCloseSession(&mem);
        pcvExit();
        clkrstExit();
        g_overclockActive = false;
    }
}

jni_intarray* loadTexture(jni_bytearray* bArr) {
    jni_intarray* texture = reinterpret_cast<jni_intarray*>(malloc(sizeof(jni_intarray)));

    int x, y, channels_in_file;
    unsigned char* temp = stbi_load_from_memory(bArr->elements, bArr->size, &x,
        &y, &channels_in_file, 4);

    texture->size        = x * y + 2;
    texture->elements    = reinterpret_cast<int*>(malloc(texture->size * sizeof(int)));
    texture->elements[0] = x;
    texture->elements[1] = y;

    for (int n = 0; n < y; n++) {
        for (int m = 0; m < x; m++) {
            unsigned char* color             = (unsigned char*)&(((u32*)temp)[n * x + m]);
            texture->elements[2 + n * x + m] = RGBA8(color[2], color[1], color[0], color[3]);
        }
    }

    free(temp);

    return texture;
}

int isDeviceAndroidTV() { return 1; }

stbtt_fontinfo* info      = NULL;
unsigned char* fontBuffer = NULL;

static inline u32 utf8_decode_unsafe_2(const char* data) {
    u32 codepoint;
    codepoint = ((data[0] & 0x1F) << 6);
    codepoint |= (data[1] & 0x3F);
    return codepoint;
}

static inline u32 utf8_decode_unsafe_3(const char* data) {
    u32 codepoint;
    codepoint = ((data[0] & 0x0F) << 12);
    codepoint |= (data[1] & 0x3F) << 6;
    codepoint |= (data[2] & 0x3F);
    return codepoint;
}

static inline u32 utf8_decode_unsafe_4(const char* data) {
    u32 codepoint;
    codepoint = ((data[0] & 0x07) << 18);
    codepoint |= (data[1] & 0x3F) << 12;
    codepoint |= (data[2] & 0x3F) << 6;
    codepoint |= (data[3] & 0x3F);
    return codepoint;
}

jni_intarray* drawFont(char* word, int size, int i2, int i3) {
    jni_intarray* texture = reinterpret_cast<jni_intarray*>(malloc(sizeof(jni_intarray)));
    texture->size         = size * size + 5;
    texture->elements     = reinterpret_cast<int*>(malloc(texture->size * sizeof(int)));

    memset(texture->elements, 0, texture->size * sizeof(int));

    const int SS = 4;

    int b_w = size * SS;
    int b_h = size * SS;

    float scale = stbtt_ScaleForPixelHeight(info, size * SS);

    int ascent, descent, lineGap;
    stbtt_GetFontVMetrics(info, &ascent, &descent, &lineGap);

    int i = 0;
    while (word[i]) {
        i++;
        if (i == 4)
            break;
    }

    int codepoint;
    switch (i) {
        case 0:
            codepoint = 32;
            break;
        case 2:
            codepoint = utf8_decode_unsafe_2(word);
            break;
        case 3:
            codepoint = utf8_decode_unsafe_3(word);
            break;
        case 4:
            codepoint = utf8_decode_unsafe_4(word);
            break;
        default:
            codepoint = word[0];
            break;
    }

    int ax;
    int lsb;
    stbtt_GetCodepointHMetrics(info, codepoint, &ax, &lsb);

    if (codepoint == 32) {
        texture->elements[0] = roundf(ax * scale / SS);
        return texture;
    }

    unsigned char* bitmap = reinterpret_cast<unsigned char*>(calloc(b_w * b_h, sizeof(unsigned char)));

    int c_x1, c_y1, c_x2, c_y2;
    stbtt_GetCodepointBitmapBox(info, codepoint, scale, scale, &c_x1, &c_y1, &c_x2, &c_y2);

    int y = roundf(ascent * scale) + c_y1 - (200 * scale);

    int byteOffset = roundf(lsb * scale) + (y * b_w);

    stbtt_MakeCodepointBitmap(info, bitmap + byteOffset, c_x2 - c_x1, c_y2 - c_y1, b_w, scale, scale, codepoint);

    texture->elements[0] = (c_x2 - c_x1 + roundf(lsb * scale)) / SS;
    texture->elements[1] = 0;
    texture->elements[2] = 0;

    for (int oy = 0; oy < size; oy++) {
        for (int ox = 0; ox < size; ox++) {
            int sum = 0;
            for (int sy = 0; sy < SS; sy++) {
                for (int sx = 0; sx < SS; sx++) {
                    int src_x = ox * SS + sx;
                    int src_y = oy * SS + sy;
                    sum += bitmap[src_y * b_w + src_x];
                }
            }
            int avg                               = sum / (SS * SS);
            texture->elements[5 + oy * size + ox] = RGBA8(avg, avg, avg, avg);
        }
    }

    free(bitmap);
    return texture;
}

static SwkbdConfig swkbd;
static bool keyboardOpen = false;
static bool keyboardDone = false;
static char keyboardResult[256];

void createEditText(char* str) {
    swkbdCreate(&swkbd, 0);

    swkbdConfigMakePresetDefault(&swkbd);

    swkbdConfigSetGuideText(&swkbd, "Enter text");

    if (str) {
        swkbdConfigSetInitialText(&swkbd, str);
    }

    keyboardOpen = true;
    keyboardDone = false;

    memset(keyboardResult, 0, sizeof(keyboardResult));
}

char* getEditText() {
    if (!keyboardOpen) {
        return NULL;
    }

    if (!keyboardDone) {
        Result rc = swkbdShow(&swkbd, keyboardResult, sizeof(keyboardResult));

        keyboardDone = true;
        keyboardOpen = false;

        swkbdClose(&swkbd);

        if (R_FAILED(rc)) {
            return NULL;
        }
    }

    return keyboardResult;
}

void initFont() {
    long size;

    if (info != NULL)
        return;

    char font_path[256];
    sprintf(font_path, FONTS_DIR "%s", config::font_filename);

    FILE* fontFile = fopen(font_path, "rb");

    fseek(fontFile, 0, SEEK_END);
    size = ftell(fontFile);
    fseek(fontFile, 0, SEEK_SET);

    fontBuffer = reinterpret_cast<unsigned char*>(malloc(size));

    fread(fontBuffer, size, 1, fontFile);
    fclose(fontFile);

    info = reinterpret_cast<stbtt_fontinfo*>(malloc(sizeof(stbtt_fontinfo)));

    if (!stbtt_InitFont(info, fontBuffer, 0)) {
        debugPrintf("failed\n");
    }
}

void reinitFont() {
    if (info != NULL) {
        free(info);
        info = NULL;
    }

    if (fontBuffer != NULL) {
        free(fontBuffer);
        fontBuffer = NULL;
    }

    initFont();
}

int getCurrentLanguage() {
    if (config::language != -1) {
        return config::language;
    }
    u64 lang;

    if (R_SUCCEEDED(setGetSystemLanguage(&lang))) {
        switch (lang) {
            case SetLanguage_JA:
                return 0;
                break;
            case SetLanguage_ENUS:
            case SetLanguage_ENGB:
                return 1;
                break;
            case SetLanguage_FR:
                return 2;
                break;
            case SetLanguage_DE:
                return 3;
                break;
            case SetLanguage_IT:
                return 4;
                break;
            case SetLanguage_ES:
                return 5;
                break;
            case SetLanguage_ZHHANS:
                return 6;
                break;
            case SetLanguage_ZHHANT:
                return 7;
                break;
            case SetLanguage_KO:
                return 8;
                break;
            case SetLanguage_PT:
                return 9;
                break;
            case SetLanguage_RU:
                return 10;
                break;
            default:
                return 1;
                break;
        }
    }
    return 1;
}

struct MovieState
{
    bool active      = false;
    bool eof_reached = false;

    AVFormatContext* fmt_ctx = nullptr;

    int video_stream_idx      = -1;
    AVCodecContext* video_ctx = nullptr;
    SwsContext* sws_ctx       = nullptr;
    AVFrame* video_frame      = nullptr;
    int video_width           = 0;
    int video_height          = 0;

    int audio_stream_idx       = -1;
    AVCodecContext* audio_ctx  = nullptr;
    SwrContext* swr_ctx        = nullptr;
    AVFrame* audio_frame       = nullptr;
    bool audio_active          = false;
    u8* audio_scratch          = nullptr;
    int audio_scratch_capacity = 0;

    AVPacket* packet = nullptr;

    GLuint texture = 0;

    u64 start_tick_ns     = 0;
    double next_frame_pts = 0.0;

    bool eof_flushed         = false;
    bool decode_thread_alive = false;
};

MovieState g_Movie;

s32 framerate = 30;

void setFPS(s32 i) {
    framerate = i;
    if (config::dfps_battles_enabled && i == 15) {
        framerate = i * config::dfps_battles_mult;
    }
}

u64 g_UpdateCount      = 0;
u64 g_ElapsedTicks     = 0;
static u64 accumulator = 0;
void updateAccumulator() {
    static u64 last_tick = armGetSystemTick();
    u64 tick             = armGetSystemTick();
    accumulator += tick - last_tick;
    last_tick = tick;
}

u64 getTargetUPS() {
    u64 target_ups = framerate * g_FPSMultiplier;
    if (g_Movie.active && config::limit_movie_fps) {
        target_ups = 24;
    }

    game::g_MaxUpdatesPerFrame = 3 * g_FPSMultiplier;
    if (g_Movie.active)
        game::g_MaxUpdatesPerFrame = 3;
    if (game::g_MaxUpdatesPerFrame < 1)
        game::g_MaxUpdatesPerFrame = 1;

    return target_ups;
}

u64 getTargetTicks() {
    u64 target_ups = getTargetUPS();

    return armNsToTicks(1000000000ULL / target_ups);
}

constexpr u64 TICKS_PER_SECOND = 19200000ULL;
u64 getCurrentFrame(u64 lastUpdate) {
    u64 target_ups = getTargetUPS();

    while (true) {
        updateAccumulator();
        u64 updates = (accumulator * target_ups) / TICKS_PER_SECOND;

        if (updates > 0) {
            u64 ticks = (updates * TICKS_PER_SECOND) / target_ups;

            g_ElapsedTicks += ticks;
            accumulator -= ticks;

            if (updates > game::g_MaxUpdatesPerFrame)
                updates = game::g_MaxUpdatesPerFrame;

            g_UpdateCount += updates;
            return lastUpdate + updates;
        }

        svcSleepThread(1000000);
    }
}

bool isUpdateFrame() {
    updateAccumulator();
    return (accumulator * getTargetUPS()) / TICKS_PER_SECOND > 0;
}

struct DecodedFrame
{
    u8* rgba_data = nullptr;
    double pts    = 0.0;
};

#define MOVIE_QUEUE_SIZE 4

static std::queue<DecodedFrame> g_frameQueue;
static pthread_mutex_t g_queueMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t g_queueCond   = PTHREAD_COND_INITIALIZER;
static pthread_t g_decodeThread;
static volatile bool g_decodeThreadStop = false;

double ptsToSeconds(AVRational time_base, s64 pts) {
    if (pts == AV_NOPTS_VALUE)
        return 0.0;
    return pts * av_q2d(time_base);
}

void movieTeardown() {
    applyMovieOverclock(false);

    if (g_Movie.decode_thread_alive) {
        g_decodeThreadStop = true;
        pthread_mutex_lock(&g_queueMutex);
        pthread_cond_signal(&g_queueCond);
        pthread_mutex_unlock(&g_queueMutex);
        pthread_join(g_decodeThread, nullptr);
        g_Movie.decode_thread_alive = false;
        g_decodeThreadStop          = false;
    }

    while (!g_frameQueue.empty()) {
        av_free(g_frameQueue.front().rgba_data);
        g_frameQueue.pop();
    }

    if (g_Movie.audio_active) {
        AKBSystem::ClearRawPCM();
        g_Movie.audio_active = false;
    }
    if (g_Movie.texture) {
        glDeleteTextures(1, &g_Movie.texture);
        g_Movie.texture = 0;
    }
    if (g_Movie.swr_ctx) {
        swr_free(&g_Movie.swr_ctx);
        g_Movie.swr_ctx = nullptr;
    }
    if (g_Movie.sws_ctx) {
        sws_freeContext(g_Movie.sws_ctx);
        g_Movie.sws_ctx = nullptr;
    }
    if (g_Movie.audio_frame) {
        av_frame_free(&g_Movie.audio_frame);
    }
    if (g_Movie.audio_scratch) {
        av_freep(&g_Movie.audio_scratch);
        g_Movie.audio_scratch_capacity = 0;
    }
    if (g_Movie.video_frame) {
        av_frame_free(&g_Movie.video_frame);
    }
    if (g_Movie.packet) {
        av_packet_free(&g_Movie.packet);
    }
    if (g_Movie.audio_ctx) {
        avcodec_free_context(&g_Movie.audio_ctx);
    }
    if (g_Movie.video_ctx) {
        avcodec_free_context(&g_Movie.video_ctx);
    }
    if (g_Movie.fmt_ctx) {
        avformat_close_input(&g_Movie.fmt_ctx);
    }

    g_Movie = MovieState();
}

void queueAudioFrame(AVFrame* frame) {
    if (!g_Movie.swr_ctx)
        return;

    const int device_rate = AKBSystem::GetDeviceRate();

    int out_samples = av_rescale_rnd(
        swr_get_delay(g_Movie.swr_ctx, frame->sample_rate) + frame->nb_samples,
        device_rate, frame->sample_rate, AV_ROUND_UP);

    if (out_samples > g_Movie.audio_scratch_capacity) {
        av_freep(&g_Movie.audio_scratch);
        av_samples_alloc(&g_Movie.audio_scratch, nullptr, 2, out_samples, AV_SAMPLE_FMT_S16, 0);
        g_Movie.audio_scratch_capacity = out_samples;
    }

    int converted = swr_convert(g_Movie.swr_ctx, &g_Movie.audio_scratch, out_samples,
        (const u8**)frame->data, frame->nb_samples);

    if (converted > 0) {
        AKBSystem::PushRawPCM(reinterpret_cast<s16*>(g_Movie.audio_scratch), converted);
        g_Movie.audio_active = true;
    }
}

static bool decodeFrameIntoBuffer(u8*& out_rgba, double& out_pts) {
    while (av_read_frame(g_Movie.fmt_ctx, g_Movie.packet) >= 0) {
        if (g_Movie.packet->stream_index == g_Movie.video_stream_idx) {
            int ret = avcodec_send_packet(g_Movie.video_ctx, g_Movie.packet);
            av_packet_unref(g_Movie.packet);
            if (ret < 0)
                continue;

            ret = avcodec_receive_frame(g_Movie.video_ctx, g_Movie.video_frame);
            if (ret == AVERROR(EAGAIN))
                continue;
            if (ret < 0)
                return false;

            int buf_size        = av_image_get_buffer_size(AV_PIX_FMT_RGBA,
                g_Movie.video_width, g_Movie.video_height, 1);
            u8* buf             = reinterpret_cast<u8*>(av_malloc(buf_size));
            u8* dst[4]          = { buf, nullptr, nullptr, nullptr };
            int dst_linesize[4] = { g_Movie.video_width * 4, 0, 0, 0 };

            sws_scale(g_Movie.sws_ctx, g_Movie.video_frame->data, g_Movie.video_frame->linesize,
                0, g_Movie.video_height, dst, dst_linesize);

            AVRational tb = g_Movie.fmt_ctx->streams[g_Movie.video_stream_idx]->time_base;
            out_rgba      = buf;
            out_pts       = ptsToSeconds(tb, g_Movie.video_frame->best_effort_timestamp);
            return true;
        } else if (g_Movie.packet->stream_index == g_Movie.audio_stream_idx) {
            int ret = avcodec_send_packet(g_Movie.audio_ctx, g_Movie.packet);
            av_packet_unref(g_Movie.packet);
            if (ret < 0)
                continue;

            while (avcodec_receive_frame(g_Movie.audio_ctx, g_Movie.audio_frame) == 0) {
                queueAudioFrame(g_Movie.audio_frame);
            }
            continue;
        } else {
            av_packet_unref(g_Movie.packet);
        }
    }

    if (!g_Movie.eof_flushed) {
        g_Movie.eof_flushed = true;
        avcodec_send_packet(g_Movie.video_ctx, nullptr);
    }

    while (true) {
        int ret = avcodec_receive_frame(g_Movie.video_ctx, g_Movie.video_frame);
        if (ret < 0)
            return false;

        int buf_size        = av_image_get_buffer_size(AV_PIX_FMT_RGBA,
            g_Movie.video_width, g_Movie.video_height, 1);
        u8* buf             = reinterpret_cast<u8*>(av_malloc(buf_size));
        u8* dst[4]          = { buf, nullptr, nullptr, nullptr };
        int dst_linesize[4] = { g_Movie.video_width * 4, 0, 0, 0 };

        sws_scale(g_Movie.sws_ctx, g_Movie.video_frame->data, g_Movie.video_frame->linesize,
            0, g_Movie.video_height, dst, dst_linesize);

        AVRational tb = g_Movie.fmt_ctx->streams[g_Movie.video_stream_idx]->time_base;
        out_rgba      = buf;
        out_pts       = ptsToSeconds(tb, g_Movie.video_frame->best_effort_timestamp);
        return true;
    }
}

static void* decodeThreadFunc(void*) {
    while (!g_decodeThreadStop) {
        u8* rgba_data = nullptr;
        double pts    = 0.0;

        if (!decodeFrameIntoBuffer(rgba_data, pts)) {
            g_Movie.eof_reached = true;
            pthread_cond_signal(&g_queueCond);
            break;
        }

        pthread_mutex_lock(&g_queueMutex);
        while (g_frameQueue.size() >= MOVIE_QUEUE_SIZE && !g_decodeThreadStop) {
            pthread_cond_wait(&g_queueCond, &g_queueMutex);
        }
        if (g_decodeThreadStop) {
            av_free(rgba_data);
            pthread_mutex_unlock(&g_queueMutex);
            break;
        }
        DecodedFrame f;
        f.rgba_data = rgba_data;
        f.pts       = pts;
        g_frameQueue.push(f);
        pthread_cond_signal(&g_queueCond);
        pthread_mutex_unlock(&g_queueMutex);
    }
    return nullptr;
}

void playMovie() {
    if (g_Movie.active)
        stopMovie();

    if (config::overclock_movie_cpu || config::overclock_movie_mem)
        applyMovieOverclock(true);

    if (avformat_open_input(&g_Movie.fmt_ctx, OPENING_FILE, nullptr, nullptr) < 0) {
        debugPrintf("playMovie: failed to open %s\n", OPENING_FILE);
        return;
    }

    if (avformat_find_stream_info(g_Movie.fmt_ctx, nullptr) < 0) {
        debugPrintf("playMovie: failed to find stream info\n");
        movieTeardown();
        return;
    }

    for (unsigned int i = 0; i < g_Movie.fmt_ctx->nb_streams; i++) {
        AVMediaType type = g_Movie.fmt_ctx->streams[i]->codecpar->codec_type;
        if (type == AVMEDIA_TYPE_VIDEO && g_Movie.video_stream_idx < 0) {
            g_Movie.video_stream_idx = i;
        } else if (type == AVMEDIA_TYPE_AUDIO && g_Movie.audio_stream_idx < 0) {
            g_Movie.audio_stream_idx = i;
        }
    }

    if (g_Movie.video_stream_idx < 0) {
        debugPrintf("playMovie: no video stream found\n");
        movieTeardown();
        return;
    }

    AVCodecParameters* vpar = g_Movie.fmt_ctx->streams[g_Movie.video_stream_idx]->codecpar;
    const AVCodec* vcodec   = avcodec_find_decoder(vpar->codec_id);
    if (!vcodec) {
        debugPrintf("playMovie: no decoder for video codec\n");
        movieTeardown();
        return;
    }

    g_Movie.video_ctx = avcodec_alloc_context3(vcodec);
    avcodec_parameters_to_context(g_Movie.video_ctx, vpar);

    g_Movie.video_ctx->thread_count = 3;
    g_Movie.video_ctx->thread_type  = FF_THREAD_SLICE | FF_THREAD_FRAME;

    if (avcodec_open2(g_Movie.video_ctx, vcodec, nullptr) < 0) {
        debugPrintf("playMovie: failed to open video codec\n");
        movieTeardown();
        return;
    }

    g_Movie.video_width  = g_Movie.video_ctx->width;
    g_Movie.video_height = g_Movie.video_ctx->height;

    g_Movie.sws_ctx = sws_getContext(
        g_Movie.video_width, g_Movie.video_height, g_Movie.video_ctx->pix_fmt,
        g_Movie.video_width, g_Movie.video_height, AV_PIX_FMT_RGBA,
        SWS_FAST_BILINEAR, nullptr, nullptr, nullptr);

    g_Movie.video_frame = av_frame_alloc();

    if (g_Movie.audio_stream_idx >= 0) {
        AVCodecParameters* apar = g_Movie.fmt_ctx->streams[g_Movie.audio_stream_idx]->codecpar;
        const AVCodec* acodec   = avcodec_find_decoder(apar->codec_id);
        if (acodec) {
            g_Movie.audio_ctx = avcodec_alloc_context3(acodec);
            avcodec_parameters_to_context(g_Movie.audio_ctx, apar);
            if (avcodec_open2(g_Movie.audio_ctx, acodec, nullptr) == 0) {
                g_Movie.audio_frame = av_frame_alloc();

                AVChannelLayout out_layout = AV_CHANNEL_LAYOUT_STEREO;
                int device_rate            = AKBSystem::GetDeviceRate();

                swr_alloc_set_opts2(&g_Movie.swr_ctx,
                    &out_layout, AV_SAMPLE_FMT_S16, device_rate,
                    &g_Movie.audio_ctx->ch_layout, g_Movie.audio_ctx->sample_fmt, g_Movie.audio_ctx->sample_rate,
                    0, nullptr);
                swr_init(g_Movie.swr_ctx);

                debugPrintf("playMovie: routing movie audio through AKBSystem (device rate=%d, channels=%d)\n",
                    device_rate, AKBSystem::GetDeviceChannels());

                AKBSystem::ClearRawPCM();
                AKBSystem::SetRawPCMVolume(1.0f);
            } else {
                debugPrintf("playMovie: failed to open audio codec\n");
                avcodec_free_context(&g_Movie.audio_ctx);
                g_Movie.audio_stream_idx = -1;
            }
        }
    }

    glGenTextures(1, &g_Movie.texture);
    glBindTexture(GL_TEXTURE_2D, g_Movie.texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, g_Movie.video_width, g_Movie.video_height, 0,
        GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    g_Movie.packet = av_packet_alloc();

    g_Movie.active              = false;
    g_Movie.eof_reached         = false;
    g_Movie.eof_flushed         = false;
    g_Movie.decode_thread_alive = false;
    g_Movie.next_frame_pts      = 0.0;

    u8* first_rgba   = nullptr;
    double first_pts = 0.0;
    if (!decodeFrameIntoBuffer(first_rgba, first_pts)) {
        debugPrintf("playMovie: failed to decode first frame\n");
        g_Movie.eof_reached = true;
        g_Movie.active      = true;
        return;
    }

    glBindTexture(GL_TEXTURE_2D, g_Movie.texture);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, g_Movie.video_width, g_Movie.video_height,
        GL_RGBA, GL_UNSIGNED_BYTE, first_rgba);

    g_Movie.start_tick_ns = armTicksToNs(armGetSystemTick()) - (u64)(first_pts * 1.0e9);

    av_free(first_rgba);
    g_Movie.next_frame_pts = first_pts;
    g_Movie.active         = true;

    g_decodeThreadStop = false;
    if (pthread_create(&g_decodeThread, nullptr, decodeThreadFunc, nullptr) == 0) {
        g_Movie.decode_thread_alive = true;
    } else {
        debugPrintf("playMovie: failed to create decode thread\n");
    }
}

void stopMovie() {
    if (!g_Movie.active)
        return;
    movieTeardown();
}

static void tickAndRenderMovie() {
    if (!g_Movie.active)
        return;

    if (g_Movie.eof_reached) {
        stopMovie();
        return;
    } else {
        double elapsed = (armTicksToNs(armGetSystemTick()) - g_Movie.start_tick_ns) / 1.0e9;

        const int MAX_FRAMES_PER_TICK = 5;
        int frames_advanced           = 0;

        while (elapsed >= g_Movie.next_frame_pts && frames_advanced < MAX_FRAMES_PER_TICK) {
            pthread_mutex_lock(&g_queueMutex);
            if (g_frameQueue.empty()) {
                pthread_mutex_unlock(&g_queueMutex);
                break;
            }
            DecodedFrame frame = g_frameQueue.front();
            g_frameQueue.pop();
            pthread_cond_signal(&g_queueCond);
            pthread_mutex_unlock(&g_queueMutex);

            glBindTexture(GL_TEXTURE_2D, g_Movie.texture);
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, g_Movie.video_width, g_Movie.video_height,
                GL_RGBA, GL_UNSIGNED_BYTE, frame.rgba_data);
            av_free(frame.rgba_data);

            g_Movie.next_frame_pts = frame.pts;
            frames_advanced++;
        }

        if (frames_advanced >= MAX_FRAMES_PER_TICK) {
            g_Movie.start_tick_ns = armTicksToNs(armGetSystemTick()) - (u64)(g_Movie.next_frame_pts * 1.0e9);
        }
    }

    if (!g_Movie.active)
        return;

    int sw = config::screen_width;
    int sh = config::screen_height;

    glViewport(0, 0, sw, sh);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrthof(0.0f, (float)sw, (float)sh, 0.0f, -1.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    glDisable(GL_CULL_FACE);
    glActiveTexture(GL_TEXTURE0);
    glClientActiveTexture(GL_TEXTURE0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, g_Movie.texture);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glDisableClientState(GL_COLOR_ARRAY);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    const GLfloat verts[8] = {
        0.0f,
        0.0f,
        (float)sw,
        0.0f,
        0.0f,
        (float)sh,
        (float)sw,
        (float)sh,
    };
    const GLfloat texcoords[8] = {
        0.0f,
        0.0f,
        1.0f,
        0.0f,
        0.0f,
        1.0f,
        1.0f,
        1.0f,
    };

    glVertexPointer(2, GL_FLOAT, 0, verts);
    glTexCoordPointer(2, GL_FLOAT, 0, texcoords);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisable(GL_TEXTURE_2D);

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
}

u8 getMovieState() {
    if (g_Movie.active)
        tickAndRenderMovie();
    return g_Movie.active ? 1 : 0;
}

bool isMoviePlaying() {
    return g_Movie.active;
}
}

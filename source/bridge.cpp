#include "bridge.h"

#include <dirent.h>
#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <switch.h>
#include <zlib.h>

#include "ff4_3d_nx/movie_player.h"
#include "game.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/imgutils.h>
#include <libswresample/swresample.h>
#include <libswscale/swscale.h>
}

#include <GLES/gl.h>

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
    char str2[128], path[256];
    int file_length;
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
    unsigned char* a = loadFromMap(path, &file_length);
    if (a == NULL) {
        sprintf(path, "files/SOUND/SE/%s", str2);
        a = loadFromMap(path, &file_length);
        if (a == NULL) {
            sprintf(path, "files/SOUND/VOICE/%s", str2);
            a = loadFromMap(path, &file_length);
            if (a == NULL) {
                return NULL;
            }
        }
    }

    jni_bytearray* result = reinterpret_cast<jni_bytearray*>(malloc(sizeof(jni_bytearray)));
    result->elements      = a;
    result->size          = file_length;
    return result;
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

int isDeviceAndroidTV() { return 0; }

std::vector<FontFile> fonts;

static std::vector<FontFile*> g_chain;

void discoverFonts() {
    fonts.clear();
    DIR* dir = opendir(FONTS_DIR);
    if (!dir)
        return;

    struct dirent* ent;
    while ((ent = readdir(dir)) != NULL) {
        const char* dot = strrchr(ent->d_name, '.');
        if (!dot || strcmp(dot, ".ttf") != 0)
            continue;

        FontFile ff;
        ff.path   = std::string(FONTS_DIR) + ent->d_name;
        ff.loaded = false;

        auto it    = config::fonts.find(ff.path);
        ff.enabled = (it != config::fonts.end()) ? it->second : true;

        fonts.push_back(std::move(ff));
    }
    closedir(dir);
}

void applyFontOrder() {
    if (fonts.empty())
        return;

    std::vector<bool> consumed(fonts.size(), false);
    std::vector<FontFile> reordered;
    reordered.reserve(fonts.size());

    for (const std::string& p : config::font_order) {
        for (size_t i = 0; i < fonts.size(); i++) {
            if (consumed[i])
                continue;
            if (fonts[i].path == p) {
                reordered.push_back(std::move(fonts[i]));
                consumed[i] = true;
                break;
            }
        }
    }
    for (size_t i = 0; i < fonts.size(); i++) {
        if (!consumed[i])
            reordered.push_back(std::move(fonts[i]));
    }

    fonts = std::move(reordered);
}

static inline int utf8_decode(const char* word, int i, int* adv) {
    unsigned char c = (unsigned char)word[i];
    int cp;
    if ((c & 0x80) == 0) {
        cp   = c;
        *adv = 1;
    } else if ((c & 0xE0) == 0xC0) {
        cp   = ((c & 0x1F) << 6) | ((unsigned char)word[i + 1] & 0x3F);
        *adv = 2;
    } else if ((c & 0xF0) == 0xE0) {
        cp = ((c & 0x0F) << 12)
            | (((unsigned char)word[i + 1] & 0x3F) << 6)
            | ((unsigned char)word[i + 2] & 0x3F);
        *adv = 3;
    } else {
        cp = ((c & 0x07) << 18)
            | (((unsigned char)word[i + 1] & 0x3F) << 12)
            | (((unsigned char)word[i + 2] & 0x3F) << 6)
            | ((unsigned char)word[i + 3] & 0x3F);
        *adv = 4;
    }
    return cp;
}

static size_t findFontForCodepoint(int cp) {
    for (size_t i = 0; i < g_chain.size(); i++) {
        if (stbtt_FindGlyphIndex(g_chain[i]->info, cp) != 0)
            return i;
    }
    return 0;
}

jni_intarray* drawFont(char* word, int draw_area_size, int font_size, int /*i3*/) {
    jni_intarray* texture = reinterpret_cast<jni_intarray*>(malloc(sizeof(jni_intarray)));

    if (font_size <= 0)
        font_size = draw_area_size;

    int SS = config::font_supersampling;

    if (g_chain.empty()) {
        texture->size        = draw_area_size * draw_area_size + 5;
        texture->elements    = reinterpret_cast<int*>(calloc(texture->size, sizeof(int)));
        texture->elements[0] = 1;
        return texture;
    }

    float em_height_px = (float)(font_size * SS);

    int canvas_h   = draw_area_size * SS;
    float baseline = (canvas_h * 0.5f) + (em_height_px * 0.2f);

    std::vector<float> scales(g_chain.size());
    for (size_t i = 0; i < g_chain.size(); i++)
        scales[i] = stbtt_ScaleForMappingEmToPixels(g_chain[i]->info, (float)(font_size * SS));

    float total_advance_px = 0.0f;
    int prev_cp            = -1;
    size_t prev_font       = 0;
    for (int i = 0; word[i];) {
        int adv;
        int cp            = utf8_decode(word, i, &adv);
        size_t fi         = findFontForCodepoint(cp);
        stbtt_fontinfo* F = g_chain[fi]->info;
        float sc          = scales[fi];
        int ax, lsb;
        stbtt_GetCodepointHMetrics(F, cp, &ax, &lsb);
        if (prev_cp >= 0 && prev_font == fi)
            total_advance_px += stbtt_GetCodepointKernAdvance(F, prev_cp, cp) * sc;
        total_advance_px += ax * sc;
        prev_cp   = cp;
        prev_font = fi;
        i += adv;
    }
    int render_width = (int)((total_advance_px + SS * 0.5f) / SS);
    if (render_width < 1)
        render_width = 1;

    texture->size     = draw_area_size * draw_area_size + 5;
    texture->elements = reinterpret_cast<int*>(malloc(texture->size * sizeof(int)));
    memset(texture->elements, 0, texture->size * sizeof(int));
    texture->elements[0] = render_width;

    if (word[0] == '\0' || (word[0] == ' ' && word[1] == '\0'))
        return texture;

    int b_w               = draw_area_size * SS;
    int b_h               = draw_area_size * SS;
    unsigned char* bitmap = reinterpret_cast<unsigned char*>(calloc(b_w * b_h, sizeof(unsigned char)));

    prev_cp    = -1;
    prev_font  = 0;
    float xpos = 0.0f;
    for (int i = 0; word[i];) {
        int adv;
        int cp            = utf8_decode(word, i, &adv);
        size_t fi         = findFontForCodepoint(cp);
        stbtt_fontinfo* F = g_chain[fi]->info;
        float sc          = scales[fi];

        int ax, lsb;
        stbtt_GetCodepointHMetrics(F, cp, &ax, &lsb);
        if (prev_cp >= 0 && prev_font == fi)
            xpos += stbtt_GetCodepointKernAdvance(F, prev_cp, cp) * sc;

        int x0, y0, x1, y1;
        stbtt_GetCodepointBitmapBox(F, cp, sc, sc, &x0, &y0, &x1, &y1);

        int gx = (int)roundf(xpos + x0);
        int gy = (int)roundf(baseline + y0);
        int gw = x1 - x0;
        int gh = y1 - y0;

        int sx_off = 0, sy_off = 0;
        if (gx < 0) {
            sx_off = -gx;
            gw += gx;
            gx = 0;
        }
        if (gy < 0) {
            sy_off = -gy;
            gh += gy;
            gy = 0;
        }
        if (gx + gw > b_w)
            gw = b_w - gx;
        if (gy + gh > b_h)
            gh = b_h - gy;

        if (gw > 0 && gh > 0) {
            if (sx_off == 0 && sy_off == 0) {
                stbtt_MakeCodepointBitmap(F,
                    bitmap + gy * b_w + gx, gw, gh, b_w,
                    sc, sc, cp);
            } else {
                int full_w          = x1 - x0;
                int full_h          = y1 - y0;
                unsigned char* temp = reinterpret_cast<unsigned char*>(
                    calloc(full_w * full_h, 1));
                stbtt_MakeCodepointBitmap(F, temp, full_w, full_h, full_w,
                    sc, sc, cp);
                for (int row = 0; row < gh; row++) {
                    memcpy(bitmap + (gy + row) * b_w + gx,
                        temp + (sy_off + row) * full_w + sx_off,
                        gw);
                }
                free(temp);
            }
        }

        xpos += ax * sc;
        prev_cp   = cp;
        prev_font = fi;
        i += adv;
    }

    const bool pixelated = (config::font_scaling_mode == config::FontScalingMode_Pixelated);

    if (SS == 1) {
        for (int oy = 0; oy < draw_area_size - 1; oy++) {
            for (int ox = 0; ox < draw_area_size; ox++) {
                int v = bitmap[oy * b_w + ox];
                if (pixelated)
                    v = (v >= 128) ? 255 : 0;
                texture->elements[5 + oy * draw_area_size + ox] = RGBA8(0xFF, 0xFF, 0xFF, v);
            }
        }
    } else {
        const int cs = SS / 2;
        for (int oy = 0; oy < draw_area_size - 1; oy++) {
            for (int ox = 0; ox < draw_area_size; ox++) {
                int v;
                if (pixelated) {
                    int center = bitmap[(oy * SS + cs) * b_w + (ox * SS + cs)];
                    v          = (center >= 128) ? 255 : 0;
                } else {
                    int sum = 0;
                    for (int sy = 0; sy < SS; sy++) {
                        const unsigned char* row = bitmap + (oy * SS + sy) * b_w + (ox * SS);
                        for (int sx = 0; sx < SS; sx++)
                            sum += row[sx];
                    }
                    v = sum / (SS * SS);
                }
                texture->elements[5 + oy * draw_area_size + ox] = RGBA8(0xFF, 0xFF, 0xFF, v);
            }
        }
    }

    int last_row_off = 5 + (draw_area_size - 1) * draw_area_size;
    for (int ox = 0; ox < draw_area_size; ox++)
        texture->elements[last_row_off + ox] = RGBA8(0xFF, 0xFF, 0xFF, 0);

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
    // Free any previously loaded font data.
    for (FontFile& ff : fonts) {
        if (ff.loaded) {
            free(ff.info);
            ff.info = nullptr;
            free(ff.buffer);
            ff.buffer = nullptr;
            ff.loaded = false;
        }
    }
    g_chain.clear();

    for (FontFile& ff : fonts) {
        if (!ff.enabled)
            continue;

        FILE* f = fopen(ff.path.c_str(), "rb");
        if (!f) {
            debugPrintf("initFont: failed to open %s\n", ff.path.c_str());
            continue;
        }
        fseek(f, 0, SEEK_END);
        long sz = ftell(f);
        fseek(f, 0, SEEK_SET);

        ff.buffer = reinterpret_cast<unsigned char*>(malloc(sz));
        if (!ff.buffer) {
            fclose(f);
            continue;
        }
        fread(ff.buffer, 1, sz, f);
        fclose(f);

        ff.info = reinterpret_cast<stbtt_fontinfo*>(malloc(sizeof(stbtt_fontinfo)));
        if (!stbtt_InitFont(ff.info, ff.buffer, 0)) {
            debugPrintf("initFont: stbtt_InitFont failed for %s\n", ff.path.c_str());
            free(ff.info);
            ff.info = nullptr;
            free(ff.buffer);
            ff.buffer = nullptr;
            continue;
        }

        ff.em_scale = stbtt_ScaleForMappingEmToPixels(ff.info, 1.0f);
        ff.loaded   = true;
        g_chain.push_back(&ff);
    }
}

void reinitFont() {
    initFont();
}

float getFontEmScaleCorrection(const char* path) {
    FILE* f = fopen(path, "rb");
    if (!f)
        return 1.0f;

    fseek(f, 0, SEEK_END);
    long sz = ftell(f);
    fseek(f, 0, SEEK_SET);

    unsigned char* buf = reinterpret_cast<unsigned char*>(malloc(sz));
    if (!buf) {
        fclose(f);
        return 1.0f;
    }
    fread(buf, 1, sz, f);
    fclose(f);

    stbtt_fontinfo info;
    float correction = 1.0f;
    if (stbtt_InitFont(&info, buf, 0)) {
        float sPix = stbtt_ScaleForPixelHeight(&info, 1.0f);
        float sEm  = stbtt_ScaleForMappingEmToPixels(&info, 1.0f);
        if (sPix > 0.0f && sEm > 0.0f)
            correction = sEm / sPix;
    }
    free(buf);
    return correction;
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
    if (movie_is_playing() && config::limit_movie_fps) {
        target_ups = 24;
    }

    game::g_MaxUpdatesPerFrame = 3 * g_FPSMultiplier;
    if (movie_is_playing())
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

void playMovie() {
    movie_play(OPENING_FILE, 1);
    return;
}

void stopMovie() {
    movie_stop();
    return;
}

u8 getMovieState() {
    bool is_playing = movie_is_playing();
    if (is_playing) {
        movie_render();
    }
    return is_playing;
}
}

/* imports.cpp -- .so import resolution
 *
 * Copyright (C) 2026 GlitchedDeveloper, fgsfds, Andy Nguyen
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

#include "imports.h"

#include <GLES/egl.h>
#include <assert.h>
#include <ctype.h>
#include <fcntl.h>
#include <malloc.h>
#include <math.h>
#include <pthread.h>
#include <sched.h>
#include <semaphore.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <switch.h>
#include <sys/reent.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <wchar.h>

#include "config.h"
#include "ff4_3d_nx/opensles.h"
#include "util.h"
#include "wrappers/aasset.h"
#include "wrappers/pthread.h"

int __android_log_print(int prio, const char* tag, const char* fmt, ...) {
#ifdef DEBUG_LOG
    va_list list;
    static char string[0x1000];

    va_start(list, fmt);
    debugPrintf("__android_log_print: tag=%s, fmt=%s\n", tag, fmt);
    vsnprintf(string, sizeof(string), fmt, list);
    va_end(list);

    debugPrintf("%s: %s\n", tag, string);
#endif
    return 0;
}

extern uintptr_t __cxa_atexit;
extern "C" int __cxa_guard_acquire(int64_t*);
extern "C" void __cxa_guard_release(int64_t*);
extern "C" void __cxa_pure_virtual(void);

static void __stack_chk_fail_nop(void) { /* disabled for Switch port */ }

static int g_free_guard_logs;
static int g_strlen_null_logs;
static int g_memcmp_guard_logs;

static int free_wrapper_looks_like_path(const void* ptr, char preview[65]) {
    const unsigned char* s = (const unsigned char*)ptr;
    int i;
    int sep                                    = 0;
    int alpha                                  = 0;
    int digits                                 = 0;
    int dots                                   = 0;
    int underscores                            = 0;
    int printable                              = 0;
    static const char* const resource_tokens[] = {
        "Act", "Anim", "Props", "Door", "IDLE", "ESC", "Hud", "UI", ".img", ".act", ".wad", ".txd"
    };

    if (preview)
        preview[0] = '\0';

    if (!ptr)
        return 0;

    for (i = 0; i < 64; i++) {
        unsigned char c = s[i];
        if (preview)
            preview[i] = (char)c;

        if (c == '\0')
            break;

        if (c < 0x20 || c > 0x7e) {
            if (preview)
                preview[i] = '\0';
            return 0;
        }

        printable++;

        if (c == '/' || c == '\\')
            sep = 1;
        else if (isalpha(c))
            alpha = 1;
        else if (isdigit(c))
            digits = 1;
        else if (c == '.')
            dots = 1;
        else if (c == '_')
            underscores = 1;
    }

    if (preview)
        preview[i < 64 ? i : 64] = '\0';

    if (printable < 4 || !alpha)
        return 0;

    if (preview && (strncmp(preview, "/Act/", 5) == 0 || strncmp(preview, "Act/", 4) == 0 || strncmp(preview, "Anim/", 5) == 0 || strncmp(preview, "/Anim/", 6) == 0 || strncmp(preview, "Props/", 6) == 0 || strncmp(preview, "/Props/", 7) == 0 || strncmp(preview, "ESC", 3) == 0))
        return 1;

    if (preview && (preview[0] == '/' || strchr(preview, '\\') != NULL))
        return 1;

    if (preview && printable >= 8 && (dots || underscores || digits)) {
        unsigned i;
        for (i = 0; i < sizeof(resource_tokens) / sizeof(resource_tokens[0]); i++) {
            if (strstr(preview, resource_tokens[i]) != NULL)
                return 1;
        }
    }

    return 0;
}

void free_wrapper(void* ptr) {
    uintptr_t p = (uintptr_t)ptr;
    char preview[65];

    if (!ptr)
        return;

    // Switch user-space pointers stay below 48 bits. If upper bits are set,
    // the caller handed us garbage, often an inline ASCII path blob rather than
    // a real heap pointer. Skip the free so we don't explode in _free_r.
    if ((p >> 48) != 0) {
        if (g_free_guard_logs < 32) {
            char text[9];
            memcpy(text, &p, 8);
            text[8] = '\0';
            debugPrintf("free_wrapper: ignoring bogus ptr=%p text=\"%s\"\n", ptr, text);
            g_free_guard_logs++;
        }
        return;
    }

    // Resource streaming occasionally hands delete/free a pointer to a
    // filename buffer instead of heap memory. Treat obvious path strings as
    // non-owning references and ignore the free to keep the worker thread alive.
    if (free_wrapper_looks_like_path(ptr, preview)) {
        if (g_free_guard_logs < 32) {
            debugPrintf("free_wrapper: ignoring path-like ptr=%p text=\"%s\"\n", ptr, preview);
            g_free_guard_logs++;
        }
        return;
    }

    free(ptr);
}

void* realloc_wrapper(void* ptr, size_t size) {
    uintptr_t p = (uintptr_t)ptr;
    char preview[65];

    if (!ptr)
        return realloc(NULL, size);

    if (size == 0) {
        free_wrapper(ptr);
        return NULL;
    }

    if ((p >> 48) != 0) {
        void* fresh = calloc(1, size);
        if (g_free_guard_logs < 32) {
            char text[9];
            memcpy(text, &p, 8);
            text[8] = '\0';
            debugPrintf("realloc_wrapper: replaced bogus ptr=%p text=\"%s\" size=%zu -> %p\n",
                ptr, text, size, fresh);
            g_free_guard_logs++;
        }
        return fresh;
    }

    if (free_wrapper_looks_like_path(ptr, preview)) {
        size_t copy_len = strnlen((const char*)ptr, 64);
        void* fresh     = calloc(1, size);
        if (fresh && copy_len > 0) {
            if (copy_len >= size)
                copy_len = size - 1;
            memcpy(fresh, ptr, copy_len);
            ((char*)fresh)[copy_len] = '\0';
        }
        if (g_free_guard_logs < 32) {
            debugPrintf("realloc_wrapper: detached path-like ptr=%p text=\"%s\" size=%zu -> %p\n",
                ptr, preview, size, fresh);
            g_free_guard_logs++;
        }
        return fresh;
    }

    return realloc(ptr, size);
}

static unsigned long long ndk_rel_caller(const void* caller) {
    uintptr_t base = (uintptr_t)so::text_virtbase;
    uintptr_t addr = (uintptr_t)caller;
    if (!base || addr < base)
        return 0ull;
    return (unsigned long long)(addr - base);
}

static int g_strcmp_guard_logs;
static int strcmp_wrapper(const char* a, const char* b) {
    if (a == b)
        return 0;

    if (!a || !b) {
        void* caller           = __builtin_return_address(0);
        unsigned long long rel = ndk_rel_caller(caller);

        if (g_strcmp_guard_logs < 64) {
            debugPrintf("strcmp_wrapper: caller=%p rel=0x%llx a=%p b=%p -> guard\n",
                caller, rel, a, b);
            g_strcmp_guard_logs++;
        }

        if (!a && !b)
            return 0;
        return a ? 1 : -1;
    }

    return strcmp(a, b);
}

static int strncmp_wrapper(const char* a, const char* b, size_t n) {
    if (n == 0 || a == b)
        return 0;

    if (!a || !b) {
        void* caller           = __builtin_return_address(0);
        unsigned long long rel = ndk_rel_caller(caller);

        if (g_strcmp_guard_logs < 64) {
            debugPrintf("strncmp_wrapper: caller=%p rel=0x%llx a=%p b=%p n=%zu -> guard\n",
                caller, rel, a, b, n);
            g_strcmp_guard_logs++;
        }

        if (!a && !b)
            return 0;
        return a ? 1 : -1;
    }

    return strncmp(a, b, n);
}

static size_t strlen_wrapper(const char* s) {
    if (!s) {
        void* caller           = __builtin_return_address(0);
        unsigned long long rel = ndk_rel_caller(caller);
        if (g_strlen_null_logs < 64) {
            debugPrintf("strlen_wrapper: caller=%p rel=0x%llx s=NULL -> 0\n",
                caller, rel);
            g_strlen_null_logs++;
        }
        return 0;
    }

    return strlen(s);
}

static int memcmp_wrapper(const void* a, const void* b, size_t n) {
    if (n == 0 || a == b)
        return 0;

    if (!a || !b) {
        void* caller           = __builtin_return_address(0);
        unsigned long long rel = ndk_rel_caller(caller);

        if (g_memcmp_guard_logs < 64) {
            debugPrintf("memcmp_wrapper: caller=%p rel=0x%llx a=%p b=%p n=%zu -> guard\n",
                caller, rel, a, b, n);
            g_memcmp_guard_logs++;
        }

        if (!a && !b)
            return 0;
        return a ? 1 : -1;
    }

    return memcmp(a, b, n);
}

static size_t __strlen_chk_wrapper(const char* s, size_t maxlen) {
    (void)maxlen;
    return strlen_wrapper(s);
}

int __vsnprintf_chk_wrapper(char* s, size_t maxlen, int flag, size_t slen, const char* fmt, va_list ap) {
    return vsnprintf(s, maxlen, fmt, ap);
}

int __vsprintf_chk_wrapper(char* s, int flag, size_t slen, const char* fmt, va_list ap) {
    return vsprintf(s, fmt, ap);
}

static double cos_wrapper(double x) {
    return std::cos(x);
}

static double sin_wrapper(double x) {
    return std::sin(x);
}

static uint64_t __stack_chk_guard_fake = 0x4242424242424242;

void glLightfvHook(GLenum light, GLenum pname, const GLfloat* params) {
    if (pname == GL_AMBIENT || pname == GL_DIFFUSE || pname == GL_POSITION) {
        static const GLfloat zero[] = { 0.0f, 0.0f, 0.0f, 1.0f };
        glLightfv(light, pname, zero);
        return;
    }
    glLightfv(light, pname, params);
}

namespace imports {
FILE* stderr_fake;

so::DynLibFunction dynlib_functions[] = {
    { "__android_log_print", (uintptr_t)__android_log_print },

    { "__cxa_atexit", (uintptr_t)&__cxa_atexit },
    { "__cxa_finalize", (uintptr_t)&ret0 },
    { "__cxa_guard_acquire", (uintptr_t)&__cxa_guard_acquire },
    { "__cxa_guard_release", (uintptr_t)&__cxa_guard_release },
    { "__cxa_pure_virtual", (uintptr_t)&__cxa_pure_virtual },

    { "__errno", (uintptr_t)&__errno },

    { "__memcpy_chk", (uintptr_t)&memcpy },
    { "__stack_chk_fail", (uintptr_t)__stack_chk_fail_nop },
    { "__stack_chk_guard", (uintptr_t)&__stack_chk_guard_fake },
    { "__strcat_chk", (uintptr_t)&strcat },
    { "__strrchr_chk", (uintptr_t)&strrchr },
    { "__strcpy_chk", (uintptr_t)&strcpy },
    { "__strlen_chk", (uintptr_t)&__strlen_chk_wrapper },
    { "__strncpy_chk", (uintptr_t)&strncpy },
    { "__strncpy_chk2", (uintptr_t)&strncpy },
    { "__vsnprintf_chk", (uintptr_t)&__vsnprintf_chk_wrapper },
    { "__vsprintf_chk", (uintptr_t)&__vsprintf_chk_wrapper },

    { "AAsset_close", (uintptr_t)&AAsset_wrapper::close },
    { "AAsset_getLength", (uintptr_t)&AAsset_wrapper::getLength },
    { "AAsset_read", (uintptr_t)&AAsset_wrapper::read },
    { "AAsset_seek", (uintptr_t)&AAsset_wrapper::seek },
    { "AAssetManager_fromJava", (uintptr_t)&AAssetManager_wrapper::fromJava },
    { "AAssetManager_open", (uintptr_t)&AAssetManager_wrapper::open },

    { "atan2f", (uintptr_t)&atan2f },
    { "atanf", (uintptr_t)&atanf },
    { "cos", (uintptr_t)&cos_wrapper },
    { "cosf", (uintptr_t)&cosf },
    { "sin", (uintptr_t)&sin_wrapper },
    { "sinf", (uintptr_t)&sinf },
    { "tanf", (uintptr_t)&tanf },
    { "sqrtf", (uintptr_t)&sqrtf },

    { "atoi", (uintptr_t)&atoi },
    { "atof", (uintptr_t)&atof },
    { "toupper", (uintptr_t)&toupper },

    { "calloc", (uintptr_t)&calloc },
    { "free", (uintptr_t)&free_wrapper },
    { "malloc", (uintptr_t)&malloc },
    { "realloc", (uintptr_t)&realloc_wrapper },

    { "gettimeofday", (uintptr_t)&gettimeofday },
    { "time", (uintptr_t)&time },
    { "gmtime", (uintptr_t)&gmtime },

    { "abort", (uintptr_t)&abort },

    { "fopen", (uintptr_t)&fopen },
    { "fclose", (uintptr_t)&fclose },
    { "fputc", (uintptr_t)&fputc },
    { "fread", (uintptr_t)&fread },
    { "fseek", (uintptr_t)&fseek },
    { "ftell", (uintptr_t)&ftell },
    { "fwrite", (uintptr_t)&fwrite },

    { "glAlphaFunc", (uintptr_t)&glAlphaFunc },
    { "glBindTexture", (uintptr_t)&glBindTexture },
    { "glBlendFunc", (uintptr_t)&glBlendFunc },
    { "glClear", (uintptr_t)&glClear },
    { "glClearColor", (uintptr_t)&glClearColor },
    { "glColor4ub", (uintptr_t)&glColor4ub },
    { "glColorPointer", (uintptr_t)&glColorPointer },
    { "glCullFace", (uintptr_t)&glCullFace },
    { "glDeleteTextures", (uintptr_t)&glDeleteTextures },
    { "glDepthFunc", (uintptr_t)&glDepthFunc },
    { "glDepthMask", (uintptr_t)&glDepthMask },
    { "glDisable", (uintptr_t)&glDisable },
    { "glDisableClientState", (uintptr_t)&glDisableClientState },
    { "glDrawArrays", (uintptr_t)&glDrawArrays },
    { "glEnable", (uintptr_t)&glEnable },
    { "glEnableClientState", (uintptr_t)&glEnableClientState },
    { "glFogf", (uintptr_t)&glFogf },
    { "glFogfv", (uintptr_t)&glFogfv },
    { "glGenTextures", (uintptr_t)&glGenTextures },
    { "glGetError", (uintptr_t)&glGetError },
    { "glLightfv", (uintptr_t)&glLightfvHook },
    { "glLoadIdentity", (uintptr_t)&glLoadIdentity },
    { "glLoadMatrixf", (uintptr_t)&glLoadMatrixf },
    { "glMaterialfv", (uintptr_t)&glMaterialfv },
    { "glMatrixMode", (uintptr_t)&glMatrixMode },
    { "glMultMatrixf", (uintptr_t)&glMultMatrixf },
    { "glNormalPointer", (uintptr_t)&glNormalPointer },
    { "glOrthof", (uintptr_t)&glOrthof },
    { "glPopMatrix", (uintptr_t)&glPopMatrix },
    { "glPushMatrix", (uintptr_t)&glPushMatrix },
    { "glScissor", (uintptr_t)&glScissor },
    { "glTexCoordPointer", (uintptr_t)&glTexCoordPointer },
    { "glTexImage2D", (uintptr_t)&glTexImage2D },
    { "glTexParameteri", (uintptr_t)&glTexParameteri },
    { "glTexSubImage2D", (uintptr_t)&glTexSubImage2D },
    { "glTranslatef", (uintptr_t)&glTranslatef },
    { "glVertexPointer", (uintptr_t)&glVertexPointer },
    { "glViewport", (uintptr_t)&glViewport },

    { "slCreateEngine", (uintptr_t)&slCreateEngine },
#define SL_IID(n) { "SL_IID_" #n, (uintptr_t)&SL_IID_##n }
    SL_IID(3DCOMMIT),
    SL_IID(3DDOPPLER),
    SL_IID(3DGROUPING),
    SL_IID(3DLOCATION),
    SL_IID(3DMACROSCOPIC),
    SL_IID(3DSOURCE),
    SL_IID(ANDROIDCONFIGURATION),
    SL_IID(ANDROIDEFFECT),
    SL_IID(ANDROIDEFFECTCAPABILITIES),
    SL_IID(ANDROIDEFFECTSEND),
    SL_IID(ANDROIDSIMPLEBUFFERQUEUE),
    SL_IID(AUDIODECODERCAPABILITIES),
    SL_IID(AUDIOENCODER),
    SL_IID(AUDIOENCODERCAPABILITIES),
    SL_IID(AUDIOIODEVICECAPABILITIES),
    SL_IID(BASSBOOST),
    SL_IID(BUFFERQUEUE),
    SL_IID(DEVICEVOLUME),
    SL_IID(DYNAMICINTERFACEMANAGEMENT),
    SL_IID(DYNAMICSOURCE),
    SL_IID(EFFECTSEND),
    SL_IID(ENGINE),
    SL_IID(ENGINECAPABILITIES),
    SL_IID(ENVIRONMENTALREVERB),
    SL_IID(EQUALIZER),
    SL_IID(LED),
    SL_IID(METADATAEXTRACTION),
    SL_IID(METADATATRAVERSAL),
    SL_IID(MIDIMESSAGE),
    SL_IID(MIDIMUTESOLO),
    SL_IID(MIDITEMPO),
    SL_IID(MIDITIME),
    SL_IID(MUTESOLO),
    SL_IID(NULL),
    SL_IID(OBJECT),
    SL_IID(OUTPUTMIX),
    SL_IID(PITCH),
    SL_IID(PLAY),
    SL_IID(PLAYBACKRATE),
    SL_IID(PREFETCHSTATUS),
    SL_IID(PRESETREVERB),
    SL_IID(RATEPITCH),
    SL_IID(RECORD),
    SL_IID(SEEK),
    SL_IID(THREADSYNC),
    SL_IID(VIBRA),
    SL_IID(VIRTUALIZER),
    SL_IID(VISUALIZATION),
    SL_IID(VOLUME),
#undef SL_IID

    { "memcmp", (uintptr_t)&memcmp_wrapper },
    { "memcpy", (uintptr_t)&memcpy },
    { "memmove", (uintptr_t)&memmove },
    { "memset", (uintptr_t)&memset },
    { "memchr", (uintptr_t)&memchr },

    { "sprintf", (uintptr_t)&sprintf },
    { "vsnprintf", (uintptr_t)&vsnprintf },
    { "vsprintf", (uintptr_t)&vsprintf },

    { "_Znwm", (uintptr_t)&malloc },
    { "_Znam", (uintptr_t)&malloc },
    { "_ZdlPv", (uintptr_t)&free_wrapper },
    { "_ZdaPv", (uintptr_t)&free_wrapper },

    { "printf", (uintptr_t)&debugPrintf },

    { "pthread_cond_broadcast", (uintptr_t)&pthread_wrapper::cond::broadcast },
    { "pthread_cond_destroy", (uintptr_t)&pthread_wrapper::cond::destroy },
    { "pthread_cond_init", (uintptr_t)&pthread_wrapper::cond::init },
    { "pthread_cond_wait", (uintptr_t)&pthread_wrapper::cond::wait },

    { "pthread_create", (uintptr_t)&pthread_wrapper::create },
    { "pthread_join", (uintptr_t)&pthread_join },

    { "pthread_mutexattr_init", (uintptr_t)&pthread_wrapper::mutexattr::init },
    { "pthread_mutexattr_settype", (uintptr_t)&pthread_wrapper::mutexattr::settype },
    { "pthread_mutexattr_destroy", (uintptr_t)&pthread_wrapper::mutexattr::destroy },

    { "pthread_mutex_destroy", (uintptr_t)&pthread_wrapper::mutex::destroy },
    { "pthread_mutex_init", (uintptr_t)&pthread_wrapper::mutex::init },
    { "pthread_mutex_lock", (uintptr_t)&pthread_wrapper::mutex::lock },
    { "pthread_mutex_unlock", (uintptr_t)&pthread_wrapper::mutex::unlock },

    { "puts", (uintptr_t)&puts },

    { "srand", (uintptr_t)&srand },
    { "rand", (uintptr_t)&rand },

    { "strlen", (uintptr_t)&strlen_wrapper },
    { "strrchr", (uintptr_t)&strrchr },
    { "strcmp", (uintptr_t)&strcmp_wrapper },
    { "strcpy", (uintptr_t)&strcpy },
    { "strncmp", (uintptr_t)&strncmp_wrapper },
    { "strtol", (uintptr_t)&strtol },
    { "strstr", (uintptr_t)&strstr },
    { "strncpy", (uintptr_t)&strncpy },
    { "strcat", (uintptr_t)&strcat },
    { "strchr", (uintptr_t)&strchr },
    { "strncasecmp", (uintptr_t)&strncasecmp },
    { "stpcpy", (uintptr_t)&stpcpy },

    { "__strchr_chk", (uintptr_t)&strchr },
    { "__strncat_chk", (uintptr_t)&strncat },

    { "usleep", (uintptr_t)&usleep },

    { "qsort", (uintptr_t)&qsort },
};

size_t dynlib_numfunctions = sizeof(dynlib_functions) / sizeof(*dynlib_functions);

void update() {
}
}
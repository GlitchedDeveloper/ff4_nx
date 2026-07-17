#include <SDL2/SDL.h>
#include <switch.h>
#include <sys/stat.h>

#include "babil.h"
#include "config.h"
#include "error.h"
#include "game.h"
#include "imports.h"
#include "pad_manager.h"
#include "patches.h"
#include "so_util.h"
#include "switch/result.h"
#include "switch/services/acc.h"
#include "util.h"

static void* heap_so_base   = NULL;
static size_t heap_so_limit = 0;

extern "C" void __libnx_initheap(void) {
    void* addr;
    size_t size = 0, fake_heap_size = 0;
    size_t mem_available = 0, mem_used = 0;

    if (envHasHeapOverride()) {
        addr = envGetHeapOverrideAddr();
        size = envGetHeapOverrideSize();
    } else {
        svcGetInfo(&mem_available, InfoType_TotalMemorySize, CUR_PROCESS_HANDLE, 0);
        svcGetInfo(&mem_used, InfoType_UsedMemorySize, CUR_PROCESS_HANDLE, 0);
        if (mem_available > mem_used + 0x200000)
            size = (mem_available - mem_used - 0x200000) & ~0x1FFFFF;
        if (size == 0)
            size = 0x2000000 * 16;
        Result rc = svcSetHeapSize(&addr, size);
        if (R_FAILED(rc))
            diagAbortWithResult(MAKERESULT(Module_Libnx, LibnxError_HeapAllocFailed));
    }

    extern char* fake_heap_start;
    extern char* fake_heap_end;
    fake_heap_size  = umin(size, MEMORY_MB * 1024 * 1024);
    fake_heap_start = (char*)addr;
    fake_heap_end   = (char*)addr + fake_heap_size;

    heap_so_base  = (char*)addr + fake_heap_size;
    heap_so_base  = (void*)ALIGN_MEM((uintptr_t)heap_so_base, 0x1000);
    heap_so_limit = (char*)addr + size - (char*)heap_so_base;
}

static void check_syscalls() {
    if (!envIsSyscallHinted(0x77))
        fatal_error("svcMapProcessCodeMemory is unavailable.");
    if (!envIsSyscallHinted(0x78))
        fatal_error("svcUnmapProcessCodeMemory is unavailable.");
    if (!envIsSyscallHinted(0x73))
        fatal_error("svcSetProcessMemoryPermission is unavailable.");
    if (envGetOwnProcessHandle() == INVALID_HANDLE)
        fatal_error("Own process handle is unavailable.\nTitle Takeover required:\n1. Install a game in Ryujinx\n2. Hold R while launching it\n3. Select bully_nx.nro from Hbmenu");
}

AccountUid getUid() {
    Result rc;
    rc = accountInitialize(AccountServiceType_Application);

    AccountUid uid = { 0, 0 };
    rc             = accountGetPreselectedUser(&uid);
    if (R_FAILED(rc)) {
        PselUserSelectionSettings settings;
        memset(&settings, 0, sizeof(settings));
        rc = pselShowUserSelector(&uid, &settings);
        if (R_FAILED(rc)) {
            debugPrintf("pselShowUserSelector failed: 0x%x\n", rc);
            return { 0, 0 };
        }
    }

    return uid;
}

int main() {
    Result rc;
    rc = setInitialize();
    if (R_FAILED(rc))
        fatal_error("setInitialize failed.");

    config::read_config(CONFIG_PATH);
    pad_manager::read_controls(CONTROLS_PATH);

    mkdir(SAVE_FILENAME, 0777);
    mkdir(OBB_MODS, 0777);
    mkdir(FONTS_DIR, 0777);
    mkdir(ACHIEVEMENTS_PATH, 0777);

    if (config::user_save_separation) {
        rc = accountInitialize(AccountServiceType_Application);
        if (R_FAILED(rc)) {
            debugPrintf("accountInitialize failed: 0x%x\n", rc);
            setExit();
            return 0;
        }
        AccountUid uid = getUid();
        if (uid.uid[0] == 0 && uid.uid[1] == 0) {
            setExit();
            accountExit();
            return 0;
        } else {
            sprintf(config::save_filename, SAVE_FILENAME "/%lX-%lX", uid.uid[1], uid.uid[0]);
            mkdir(config::save_filename, 0777);
        }

        char config_path[256];
        sprintf(config_path, "%s/config.ini", config::save_filename);
        config::read_config(config_path);

        char controls_path[256];
        sprintf(controls_path, "%s/controls.ini", config::save_filename);
        pad_manager::read_controls(controls_path);
    }

    rc = appletInitialize();
    if (R_FAILED(rc))
        fatal_error("appletInitialize failed.");

    SDL_SetMainReady();
    if (SDL_Init(SDL_INIT_AUDIO) < 0)
        debugPrintf("SDL_Init(audio) failed: %s\n", SDL_GetError());

    check_syscalls();

    int res = so::load(SO_NAME, heap_so_base, heap_so_limit);
    if (res < 0)
        fatal_error("Could not load %s. code: %d", SO_NAME, res);

    so::relocate();
    so::resolve(imports::dynlib_functions, imports::dynlib_numfunctions, 1);

    babil::init();
    patches::game::patch();
    patches::jni::patch();
    game::init();

    so::finalize();
    so::flushCaches();
    so::executeInitArray();

    game::start();

    if (config::marked_for_write) {
        config::write_config();
    }

    setExit();
    return 0;
}
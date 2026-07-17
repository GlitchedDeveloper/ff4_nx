/* util.cpp -- misc utility functions
 *
 * Copyright (C) 2026 GlitchedDeveloper, givethesourceplox, fgsfds, Andy Nguyen
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

#include "util.h"

#include <netinet/in.h>
#include <pthread.h>
#include <stdarg.h>
#include <stdio.h>
#include <switch.h>
#include <unistd.h>

#include "config.h"
#include "so_util.h"

extern const uint8_t __code_start[];

static pthread_mutex_t s_log_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_t s_main_thread;
static int s_main_thread_set = 0;
static int s_compat_delay_ms = 2;
static FILE* s_logFile       = nullptr;

void debugPrintf_setMainThread(void) {
    s_main_thread     = pthread_self();
    s_main_thread_set = 1;
}

void debugPrintf_setCompatDelayMs(int ms) {
    if (ms < 0)
        ms = 0;
    if (ms > 20)
        ms = 20;
    s_compat_delay_ms = ms;
}

#ifdef DEBUG_LOG

static int s_nxlinkSock = -1;

static void initNxLink(void) {
    if (R_FAILED(socketInitializeDefault()))
        return;
    s_nxlinkSock = nxlinkStdio();
    if (s_nxlinkSock < 0)
        socketExit();
}

static void deinitNxLink(void) {
    if (s_nxlinkSock >= 0) {
        close(s_nxlinkSock);
        socketExit();
        s_nxlinkSock = -1;
    }
}

extern "C" void userAppInit(void) {
    initNxLink();
}

extern "C" void userAppExit(void) {
    deinitNxLink();
}

#endif

void debugBacktrace(void) {
    if (__nxlink_host.s_addr != 0) {
        const uintptr_t MAIN_APP_BASE = reinterpret_cast<uintptr_t>(&__code_start);

        const uintptr_t LIBFF4_BASE = reinterpret_cast<uintptr_t>(so::text_virtbase);
        const size_t LIBFF4_SIZE    = so::text_size;

        void** fp;
        __asm__ __volatile__("mov %0, x29" : "=r"(fp));

        int depth = 0;
        debugPrintf("--- Backtrace ---\n");

        while (fp && depth < 12) {
            void* next_fp = fp[0];
            void* lr      = fp[1];

            if (!lr)
                break;

            uintptr_t pc = reinterpret_cast<uintptr_t>(lr);

            if (pc >= LIBFF4_BASE && pc < (LIBFF4_BASE + LIBFF4_SIZE)) {
                uintptr_t relative_offset = pc - LIBFF4_BASE;
                uintptr_t ghidra_address  = relative_offset + 0x100000;
                debugPrintf("  Frame [%d]: 0x%lX -> [libff4.so] Ghidra: 0x%lX (Rel: +0x%lX)\n",
                    depth, pc, ghidra_address, relative_offset);
            } else if (pc >= MAIN_APP_BASE) {
                uintptr_t relative_offset = pc - MAIN_APP_BASE;
                debugPrintf("  Frame [%d]: 0x%lX -> [Main App]  Rel: +0x%lX\n",
                    depth, pc, relative_offset);
            } else {
                debugPrintf("  Frame [%d]: 0x%lX -> [Unknown/Sys]\n", depth, pc);
            }

            if (next_fp <= fp)
                break;

            fp = reinterpret_cast<void**>(next_fp);
            depth++;
        }
    }
}

int debugPrintf(const char* text, ...) {
#ifdef DEBUG_LOG
    va_list list;
    const char* tag = "?";
    if (s_main_thread_set)
        tag = pthread_equal(pthread_self(), s_main_thread) ? "M" : "G";

    pthread_mutex_lock(&s_log_mutex);

    if (config::log_to_file) {
        if (!s_logFile)
            s_logFile = fopen(LOG_NAME, "a");
        if (s_logFile) {
            fprintf(s_logFile, "[%s] ", tag);
            va_start(list, text);
            vfprintf(s_logFile, text, list);
            va_end(list);
            fflush(s_logFile);
        }
    }

    printf("[%s] ", tag);
    va_start(list, text);
    vprintf(text, list);
    va_end(list);

    if (!config::log_to_file && s_compat_delay_ms > 0)
        svcSleepThread((int64_t)s_compat_delay_ms * 1000000LL);

    pthread_mutex_unlock(&s_log_mutex);
#endif
    return 0;
}

static uint8_t s_tls_block[0x1000] __attribute__((aligned(16)));
void tls_setup_guard(void) {
    *(uint64_t*)(s_tls_block + 0x28) = 0x0123456789ABCDEFull;
    armSetTlsRw(s_tls_block);
}

void retvoid(void) { return; }

int ret0(void) { return 0; }

int ret1(void) { return 1; }

int retm1(void) { return -1; }

/* util.h -- misc utility functions
 *
 * Copyright (C) 2026 GlitchedDeveloper, givethesourceplox, fgsfds, Andy Nguyen
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

int debugPrintf(const char* text, ...);
void debugPrintf_setMainThread(void);
void debugPrintf_setCompatDelayMs(int ms);

void debugBacktrace(void);

void tls_setup_guard(void);

void retvoid(void);
int ret0(void);
int ret1(void);
int retm1(void);

static inline void* armGetTlsRw(void) {
    void* ret;
    __asm__("mrs %x[data], s3_3_c13_c0_2" : [data] "=r"(ret));
    return ret;
}

static inline void armSetTlsRw(void* addr) {
    __asm__("msr s3_3_c13_c0_2, %0" : : "r"(addr));
}

static inline uint64_t umin(uint64_t a, uint64_t b) {
    return (a < b) ? a : b;
}

#ifdef __cplusplus
}
#endif
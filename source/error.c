/* error.c -- error handler
 *
 * Copyright (C) 2021 fgsfds, Andy Nguyen
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

#include "error.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <switch.h>

#include "util.h"

extern void release_egl(void);

void fatal_error(const char* fmt, ...) {
    release_egl();

    PadState pad;
    padConfigureInput(1, HidNpadStyleSet_NpadStandard);
    padInitializeDefault(&pad);

    consoleInit(NULL);

    va_list list;
    va_start(list, fmt);
    vprintf(fmt, list);
    va_end(list);

    printf("\n\nPress A to exit.");

    consoleUpdate(NULL);

    while (appletMainLoop()) {
        padUpdate(&pad);
        const u64 keys = padGetButtonsDown(&pad);
        if (keys & HidNpadButton_A)
            break;
    }

    consoleExit(NULL);
    exit(1);
}

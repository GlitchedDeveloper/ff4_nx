/* so_util.h -- utils to load and hook .so modules
 *
 * Copyright (C) 2026 GlitchedDeveloper, givethesourceplox, Andy Nguyen, fgsfds
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

#pragma once

#include <cstddef>
#include <cstdint>

#include "types.h"

#define GET_FUNCTION(name, symbol) name = reinterpret_cast<decltype(name)>(so::findAddr_rx(symbol))
#define GET_VARIABLE(name, symbol) GET_FUNCTION(name, symbol)

namespace so {
#define ALIGN_MEM(x, align) (((x) + ((align) - 1)) & ~((align) - 1))

struct DynLibFunction
{
    const char* symbol;
    uintptr_t func;
};

extern void *text_base, *data_base;
extern void* text_virtbase;
extern size_t text_size, data_size;

extern void *trampoline_base, *trampoline_virtbase;
extern size_t trampoline_size;
extern uintptr_t trampoline_cursor;

struct CodeBuf
{
    uintptr_t rw;
    uintptr_t rx;
    int count = 0;

    void w32(uint32_t v);
    void w64(uint64_t v);
    void wb(int64_t target);
    void wjump(uint64_t target);
    void copy(uint32_t instruction, uint64_t orig_rx);
};

bool addr_is_in_rx(uintptr_t addr);
uintptr_t addr_rx_to_rw(uintptr_t addr);
bool addr_is_in_rw(uintptr_t addr);
uintptr_t addr_rw_to_rx(uintptr_t addr);

void hook(uintptr_t addr, uintptr_t dst, uintptr_t* trampoline_out = nullptr);

void flushCaches(void);
void freeTemp(void);
int load(const char* filename, void* base, size_t max_size);
int relocate(void);
int resolve(DynLibFunction* funcs, int num_funcs, int taint_missing_imports);
void executeInitArray(void);
uintptr_t findAddr_rw(const char* symbol);
uintptr_t findAddr_rx(const char* symbol);
uintptr_t findRelAddr(const char* symbol);
DynLibFunction* findImport(DynLibFunction* funcs, int num_funcs, const char* name);
void finalize(void);
int unload(void);
}
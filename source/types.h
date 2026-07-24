#pragma once

#include <cstdint>
using s8   = int8_t;
using s16  = int16_t;
using s32  = int32_t;
using s64  = int64_t;
using sptr = intptr_t;

using u8   = uint8_t;
using u16  = uint16_t;
using u32  = uint32_t;
using u64  = uint64_t;
using uptr = uintptr_t;

using wchar = char16_t;

using c8  = char;
using c16 = char16_t;
using c32 = char32_t;

#ifndef BIT
#define BIT(n) (1U << (n))
#endif
#pragma once

#include <cstddef>

extern "C" {
char* b64decode(const void* data, const size_t len);
}
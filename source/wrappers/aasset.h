#pragma once

#include <stddef.h>

namespace AAsset_wrapper {
int read(void* asset_ptr, void* buf, size_t count);
long seek(void* asset_ptr, long offset, int whence);
long getLength(void* asset_ptr);
void close(void* asset_ptr);
}

namespace AAssetManager_wrapper {
void* open(void* mgr, const char* filename, int mode);
void* fromJava(void* env, void* assetManager);
}
#include "aasset.h"

#include <stdio.h>
#include <stdlib.h>

#include "../util.h"

typedef struct
{
    FILE* fp;
    long size;
} FakeAAsset;

static int fake_asset_manager_storage;

namespace AAsset_wrapper {
void close(void* asset_ptr) {
    FakeAAsset* asset = (FakeAAsset*)asset_ptr;
    if (!asset)
        return;
    debugPrintf("AAsset_close: closing asset\n");
    if (asset->fp)
        fclose(asset->fp);
    free(asset);
}

int read(void* asset_ptr, void* buf, size_t count) {
    FakeAAsset* asset = (FakeAAsset*)asset_ptr;
    if (!asset || !asset->fp)
        return -1;
    return (int)fread(buf, 1, count, asset->fp);
}

long seek(void* asset_ptr, long offset, int whence) {
    FakeAAsset* asset = (FakeAAsset*)asset_ptr;
    if (!asset || !asset->fp)
        return -1;
    if (fseek(asset->fp, offset, whence) != 0)
        return -1;
    return ftell(asset->fp);
}

long getLength(void* asset_ptr) {
    FakeAAsset* asset = (FakeAAsset*)asset_ptr;
    if (!asset)
        return 0;
    return asset->size;
}
}

namespace AAssetManager_wrapper {
void* open(void* mgr, const char* filename, int mode) {
    char path[512];
    snprintf(path, sizeof(path), "assets/%s", filename);
    debugPrintf("AAssetManager_open: opening %s\n", path);

    FILE* fp = fopen(path, "rb");
    if (!fp) {
        debugPrintf("AAssetManager_open: FAILED to open %s\n", path);
        return NULL;
    }

    FakeAAsset* asset = reinterpret_cast<FakeAAsset*>(calloc(1, sizeof(FakeAAsset)));
    asset->fp         = fp;

    fseek(fp, 0, SEEK_END);
    asset->size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    debugPrintf("AAssetManager_open: opened %s (size=%ld)\n", path, asset->size);
    return asset;
}

void* fromJava(void* env, void* assetManager) {
    debugPrintf("AAssetManager_fromJava: returning fake manager\n");
    return &fake_asset_manager_storage;
}
}
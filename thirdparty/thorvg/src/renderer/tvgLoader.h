#ifndef _TVG_LOADER_H_
#define _TVG_LOADER_H_

#include "tvgLoadModule.h"

struct LoaderMgr
{
    static bool init();
    static bool term();
    static LoadModule* loader(const string& path, bool* invalid);
    static LoadModule* loader(const char* data, uint32_t size, const string& mimeType, bool copy);
    static LoadModule* loader(const uint32_t* data, uint32_t w, uint32_t h, bool copy);
    static LoadModule* loader(const char* name, const char* data, uint32_t size, const string& mimeType, bool copy);
    static LoadModule* loader(const char* key);
    static bool retrieve(const string& path);
    static bool retrieve(LoadModule* loader);
};

#endif //_TVG_LOADER_H_

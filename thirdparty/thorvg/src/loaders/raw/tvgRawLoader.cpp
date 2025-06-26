#include <fstream>
#include <string.h>
#include "tvgLoader.h"
#include "tvgRawLoader.h"

/************************************************************************/
/* Internal Class Implementation                                        */
/************************************************************************/


/************************************************************************/
/* External Class Implementation                                        */
/************************************************************************/

RawLoader::RawLoader() : ImageLoader(FileType::Raw)
{
}


RawLoader::~RawLoader()
{
    if (copy) free(surface.buf32);
}


bool RawLoader::open(const uint32_t* data, uint32_t w, uint32_t h, bool copy)
{
    if (!LoadModule::read()) return true;

    if (!data || w == 0 || h == 0) return false;

    this->w = (float)w;
    this->h = (float)h;
    this->copy = copy;

    if (copy) {
        surface.buf32 = (uint32_t*)malloc(sizeof(uint32_t) * w * h);
        if (!surface.buf32) return false;
        memcpy((void*)surface.buf32, data, sizeof(uint32_t) * w * h);
    }
    else surface.buf32 = const_cast<uint32_t*>(data);

    //setup the surface
    surface.stride = w;
    surface.w = w;
    surface.h = h;
    surface.cs = ColorSpace::ARGB8888;
    surface.channelSize = sizeof(uint32_t);
    surface.premultiplied = true;

    return true;
}


bool RawLoader::read()
{
    LoadModule::read();

    return true;
}

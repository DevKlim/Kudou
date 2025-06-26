#ifndef _TVG_JPG_LOADER_H_
#define _TVG_JPG_LOADER_H_

#include "tvgLoader.h"

using tjhandle = void*;

//TODO: Use Task?
class JpgLoader : public ImageLoader
{
public:
    JpgLoader();
    ~JpgLoader();

    bool open(const string& path) override;
    bool open(const char* data, uint32_t size, bool copy) override;
    bool read() override;

private:
    void clear();

    tjhandle jpegDecompressor;
    unsigned char* data = nullptr;
    unsigned long size = 0;
    bool freeData = false;
};

#endif //_TVG_JPG_LOADER_H_

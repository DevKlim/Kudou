#ifndef _TVG_PNG_LOADER_H_
#define _TVG_PNG_LOADER_H_

#include <png.h>
#include "tvgLoader.h"

class PngLoader : public ImageLoader
{
public:
    PngLoader();
    ~PngLoader();

    bool open(const string& path) override;
    bool open(const char* data, uint32_t size, bool copy) override;
    bool read() override;

private:
    void clear();

    png_imagep image = nullptr;
};

#endif //_TVG_PNG_LOADER_H_

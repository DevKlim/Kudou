#ifndef _TVG_WEBP_LOADER_H_
#define _TVG_WEBP_LOADER_H_

#include "tvgLoader.h"
#include "tvgTaskScheduler.h"

class WebpLoader : public ImageLoader, public Task
{
public:
    WebpLoader();
    ~WebpLoader();

    bool open(const string& path) override;
    bool open(const char* data, uint32_t size, bool copy) override;
    bool read() override;

    RenderSurface* bitmap() override;

private:
    void run(unsigned tid) override;

    unsigned char* data = nullptr;
    unsigned long size = 0;
    bool freeData = false;
};

#endif //_TVG_WEBP_LOADER_H_

#include <memory.h>
#include <webp/decode.h>

#include "tvgWebpLoader.h"


/************************************************************************/
/* Internal Class Implementation                                        */
/************************************************************************/


void WebpLoader::run(unsigned tid)
{
    //TODO: acquire the current colorspace format & pre-multiplied alpha image.
    surface.buf8 = WebPDecodeBGRA(data, size, nullptr, nullptr);
    surface.stride = (uint32_t)w;
    surface.w = (uint32_t)w;
    surface.h = (uint32_t)h;
    surface.channelSize = sizeof(uint32_t);
    surface.cs = ColorSpace::ARGB8888;
    surface.premultiplied = false;

}


/************************************************************************/
/* External Class Implementation                                        */
/************************************************************************/

WebpLoader::WebpLoader() : ImageLoader(FileType::Webp)
{
}


WebpLoader::~WebpLoader()
{
    done();

    if (freeData) free(data);
    data = nullptr;
    size = 0;
    freeData = false;
    WebPFree(surface.buf8);
}


bool WebpLoader::open(const string& path)
{
#ifdef THORVG_FILE_IO_SUPPORT
    auto webpFile = fopen(path.c_str(), "rb");
    if (!webpFile) return false;

    auto ret = false;

    //determine size
    if (fseek(webpFile, 0, SEEK_END) < 0) goto finalize;
    if (((size = ftell(webpFile)) < 1)) goto finalize;
    if (fseek(webpFile, 0, SEEK_SET)) goto finalize;

    data = (unsigned char *) malloc(size);
    if (!data) goto finalize;

    freeData = true;

    if (fread(data, size, 1, webpFile) < 1) goto finalize;

    int width, height;
    if (!WebPGetInfo(data, size, &width, &height)) goto finalize;

    w = static_cast<float>(width);
    h = static_cast<float>(height);

    ret = true;

finalize:
    fclose(webpFile);
    return ret;
#else
    return false;
#endif
}


bool WebpLoader::open(const char* data, uint32_t size, bool copy)
{
    if (copy) {
        this->data = (unsigned char *) malloc(size);
        if (!this->data) return false;
        memcpy((unsigned char *)this->data, data, size);
        freeData = true;
    } else {
        this->data = (unsigned char *) data;
        freeData = false;
    }

    int width, height;
    if (!WebPGetInfo(this->data, size, &width, &height)) return false;

    w = static_cast<float>(width);
    h = static_cast<float>(height);
    surface.cs = ColorSpace::ARGB8888;
    this->size = size;
    return true;
}


bool WebpLoader::read()
{
    if (!LoadModule::read()) return true;

    if (!data || w == 0 || h == 0) return false;

    TaskScheduler::request(this);

    return true;
}


RenderSurface* WebpLoader::bitmap()
{
    this->done();

    return ImageLoader::bitmap();
}

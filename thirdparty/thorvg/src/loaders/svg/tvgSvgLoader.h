#ifndef _TVG_SVG_LOADER_H_
#define _TVG_SVG_LOADER_H_

#include "tvgTaskScheduler.h"
#include "tvgSvgLoaderCommon.h"

class SvgLoader : public ImageLoader, public Task
{
public:
    string filePath;
    string svgPath = "";
    char* content = nullptr;
    uint32_t size = 0;

    SvgLoaderData loaderData;
    Scene* root = nullptr;

    bool copy = false;

    SvgLoader();
    ~SvgLoader();

    bool open(const string& path) override;
    bool open(const char* data, uint32_t size, bool copy) override;
    bool resize(Paint* paint, float w, float h) override;
    bool read() override;
    bool close() override;

    Paint* paint() override;

private:
    SvgViewFlag viewFlag = SvgViewFlag::None;
    AspectRatioAlign align = AspectRatioAlign::XMidYMid;
    AspectRatioMeetOrSlice meetOrSlice = AspectRatioMeetOrSlice::Meet;
    float vx = 0;
    float vy = 0;
    float vw = 0;
    float vh = 0;

    bool header();
    void clear(bool all = true);
    void run(unsigned tid) override;
};


#endif //_TVG_SVG_LOADER_H_

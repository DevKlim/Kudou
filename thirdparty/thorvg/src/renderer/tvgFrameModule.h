#ifndef _TVG_FRAME_MODULE_H_
#define _TVG_FRAME_MODULE_H_

#include "tvgLoadModule.h"

namespace tvg
{

class FrameModule: public ImageLoader
{
public:
    float segmentBegin = 0.0f;
    float segmentEnd = 1.0f;

    FrameModule(FileType type) : ImageLoader(type) {}
    virtual ~FrameModule() {}

    virtual bool frame(float no) = 0;       //set the current frame number
    virtual float totalFrame() = 0;         //return the total frame count
    virtual float curFrame() = 0;           //return the current frame number
    virtual float duration() = 0;           //return the animation duration in seconds

    void segment(float* begin, float* end)
    {
        if (begin) *begin = segmentBegin;
        if (end) *end = segmentEnd;
    }

    void segment(float begin, float end)
    {
        segmentBegin = begin;
        segmentEnd = end;
    }

    virtual bool animatable() override { return true; }
};

}

#endif //_TVG_FRAME_MODULE_H_

#ifndef _TVG_ANIMATION_H_
#define _TVG_ANIMATION_H_

#include "tvgCommon.h"
#include "tvgPaint.h"
#include "tvgPicture.h"

struct Animation::Impl
{
    Picture* picture = nullptr;

    Impl()
    {
        picture = Picture::gen().release();
        PP(picture)->ref();
    }

    ~Impl()
    {
        if (PP(picture)->unref() == 0) {
            delete(picture);
        }
    }
};

#endif //_TVG_ANIMATION_H_

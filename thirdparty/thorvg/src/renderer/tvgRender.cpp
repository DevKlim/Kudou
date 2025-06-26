#include "tvgMath.h"
#include "tvgRender.h"

/************************************************************************/
/* Internal Class Implementation                                        */
/************************************************************************/


/************************************************************************/
/* External Class Implementation                                        */
/************************************************************************/

uint32_t RenderMethod::ref()
{
    ScopedLock lock(key);
    return (++refCnt);
}


uint32_t RenderMethod::unref()
{
    ScopedLock lock(key);
    return (--refCnt);
}


void RenderRegion::intersect(const RenderRegion& rhs)
{
    auto x1 = x + w;
    auto y1 = y + h;
    auto x2 = rhs.x + rhs.w;
    auto y2 = rhs.y + rhs.h;

    x = (x > rhs.x) ? x : rhs.x;
    y = (y > rhs.y) ? y : rhs.y;
    w = ((x1 < x2) ? x1 : x2) - x;
    h = ((y1 < y2) ? y1 : y2) - y;

    if (w < 0) w = 0;
    if (h < 0) h = 0;
}


void RenderRegion::add(const RenderRegion& rhs)
{
    if (rhs.x < x) {
        w += (x - rhs.x);
        x = rhs.x;
    }
    if (rhs.y < y) {
        h += (y - rhs.y);
        y = rhs.y;
    }
    if (rhs.x + rhs.w > x + w) w = (rhs.x + rhs.w) - x;
    if (rhs.y + rhs.h > y + h) h = (rhs.y + rhs.h) - y;
}

#include "tvgMath.h"
#include "tvgSwCommon.h"

/************************************************************************/
/* Internal Class Implementation                                        */
/************************************************************************/

static inline bool _onlyShifted(const Matrix& m)
{
    if (tvg::equal(m.e11, 1.0f) && tvg::equal(m.e22, 1.0f) && tvg::zero(m.e12) && tvg::zero(m.e21)) return true;
    return false;
}


static bool _genOutline(SwImage* image, const Matrix& transform, SwMpool* mpool, unsigned tid)
{
    image->outline = mpoolReqOutline(mpool, tid);
    auto outline = image->outline;

    outline->pts.reserve(5);
    outline->types.reserve(5);
    outline->cntrs.reserve(1);
    outline->closed.reserve(1);

    Point to[4];
    auto w = static_cast<float>(image->w);
    auto h = static_cast<float>(image->h);
    to[0] = {0, 0};
    to[1] = {w, 0};
    to[2] = {w, h};
    to[3] = {0, h};

    for (int i = 0; i < 4; i++) {
        outline->pts.push(mathTransform(&to[i], transform));
        outline->types.push(SW_CURVE_TYPE_POINT);
    }

    outline->pts.push(outline->pts[0]);
    outline->types.push(SW_CURVE_TYPE_POINT);
    outline->cntrs.push(outline->pts.count - 1);
    outline->closed.push(true);

    image->outline = outline;

    return true;
}


/************************************************************************/
/* External Class Implementation                                        */
/************************************************************************/

bool imagePrepare(SwImage* image, const Matrix& transform, const SwBBox& clipRegion, SwBBox& renderRegion, SwMpool* mpool, unsigned tid)
{
    image->direct = _onlyShifted(transform);

    //Fast track: Non-transformed image but just shifted.
    if (image->direct) {
        image->ox = -static_cast<int32_t>(nearbyint(transform.e13));
        image->oy = -static_cast<int32_t>(nearbyint(transform.e23));
    //Figure out the scale factor by transform matrix
    } else {
        auto scaleX = sqrtf((transform.e11 * transform.e11) + (transform.e21 * transform.e21));
        auto scaleY = sqrtf((transform.e22 * transform.e22) + (transform.e12 * transform.e12));
        image->scale = (fabsf(scaleX - scaleY) > 0.01f) ? 1.0f : scaleX;

        if (tvg::zero(transform.e12) && tvg::zero(transform.e21)) image->scaled = true;
        else image->scaled = false;
    }

    if (!_genOutline(image, transform, mpool, tid)) return false;
    return mathUpdateOutlineBBox(image->outline, clipRegion, renderRegion, image->direct);
}


bool imageGenRle(SwImage* image, const SwBBox& renderRegion, bool antiAlias)
{
    if ((image->rle = rleRender(image->rle, image->outline, renderRegion, antiAlias))) return true;

    return false;
}


void imageDelOutline(SwImage* image, SwMpool* mpool, uint32_t tid)
{
    mpoolRetOutline(mpool, tid);
    image->outline = nullptr;
}


void imageReset(SwImage* image)
{
    rleReset(image->rle);
}


void imageFree(SwImage* image)
{
    rleFree(image->rle);
}

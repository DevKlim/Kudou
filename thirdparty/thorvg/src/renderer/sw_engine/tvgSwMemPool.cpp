#include "tvgSwCommon.h"


/************************************************************************/
/* Internal Class Implementation                                        */
/************************************************************************/


/************************************************************************/
/* External Class Implementation                                        */
/************************************************************************/

SwOutline* mpoolReqOutline(SwMpool* mpool, unsigned idx)
{
    return &mpool->outline[idx];
}


void mpoolRetOutline(SwMpool* mpool, unsigned idx)
{
    mpool->outline[idx].pts.clear();
    mpool->outline[idx].cntrs.clear();
    mpool->outline[idx].types.clear();
    mpool->outline[idx].closed.clear();
}


SwOutline* mpoolReqStrokeOutline(SwMpool* mpool, unsigned idx)
{
    return &mpool->strokeOutline[idx];
}


void mpoolRetStrokeOutline(SwMpool* mpool, unsigned idx)
{
    mpool->strokeOutline[idx].pts.clear();
    mpool->strokeOutline[idx].cntrs.clear();
    mpool->strokeOutline[idx].types.clear();
    mpool->strokeOutline[idx].closed.clear();
}


SwOutline* mpoolReqDashOutline(SwMpool* mpool, unsigned idx)
{
    return &mpool->dashOutline[idx];
}


void mpoolRetDashOutline(SwMpool* mpool, unsigned idx)
{
    mpool->dashOutline[idx].pts.clear();
    mpool->dashOutline[idx].cntrs.clear();
    mpool->dashOutline[idx].types.clear();
    mpool->dashOutline[idx].closed.clear();
}


SwMpool* mpoolInit(uint32_t threads)
{
    auto allocSize = threads + 1;

    auto mpool = static_cast<SwMpool*>(calloc(1, sizeof(SwMpool)));
    mpool->outline = static_cast<SwOutline*>(calloc(1, sizeof(SwOutline) * allocSize));
    mpool->strokeOutline = static_cast<SwOutline*>(calloc(1, sizeof(SwOutline) * allocSize));
    mpool->dashOutline = static_cast<SwOutline*>(calloc(1, sizeof(SwOutline) * allocSize));
    mpool->allocSize = allocSize;

    return mpool;
}


bool mpoolClear(SwMpool* mpool)
{
    for (unsigned i = 0; i < mpool->allocSize; ++i) {
        mpool->outline[i].pts.reset();
        mpool->outline[i].cntrs.reset();
        mpool->outline[i].types.reset();
        mpool->outline[i].closed.reset();

        mpool->strokeOutline[i].pts.reset();
        mpool->strokeOutline[i].cntrs.reset();
        mpool->strokeOutline[i].types.reset();
        mpool->strokeOutline[i].closed.reset();

        mpool->dashOutline[i].pts.reset();
        mpool->dashOutline[i].cntrs.reset();
        mpool->dashOutline[i].types.reset();
        mpool->dashOutline[i].closed.reset();
    }

    return true;
}


bool mpoolTerm(SwMpool* mpool)
{
    if (!mpool) return false;

    mpoolClear(mpool);

    free(mpool->outline);
    free(mpool->strokeOutline);
    free(mpool->dashOutline);
    free(mpool);

    return true;
}

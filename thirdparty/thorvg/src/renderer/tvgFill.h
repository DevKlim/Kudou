#ifndef _TVG_FILL_H_
#define _TVG_FILL_H_

#include <cstdlib>
#include <cstring>
#include "tvgCommon.h"

template<typename T>
struct DuplicateMethod
{
    virtual ~DuplicateMethod() {}
    virtual T* duplicate() = 0;
};

template<class T>
struct FillDup : DuplicateMethod<Fill>
{
    T* inst = nullptr;

    FillDup(T* _inst) : inst(_inst) {}
    ~FillDup() {}

    Fill* duplicate() override
    {
        return inst->duplicate();
    }
};

struct Fill::Impl
{
    ColorStop* colorStops = nullptr;
    Matrix* transform = nullptr;
    uint32_t cnt = 0;
    FillSpread spread;
    DuplicateMethod<Fill>* dup = nullptr;

    ~Impl()
    {
        delete(dup);
        free(colorStops);
        free(transform);
    }

    void method(DuplicateMethod<Fill>* dup)
    {
        this->dup = dup;
    }

    Fill* duplicate()
    {
        auto ret = dup->duplicate();
        if (!ret) return nullptr;

        ret->pImpl->cnt = cnt;
        ret->pImpl->spread = spread;
        ret->pImpl->colorStops = static_cast<ColorStop*>(malloc(sizeof(ColorStop) * cnt));
        if (cnt > 0) memcpy(ret->pImpl->colorStops, colorStops, sizeof(ColorStop) * cnt);
        if (transform) {
            ret->pImpl->transform = static_cast<Matrix*>(malloc(sizeof(Matrix)));
            *ret->pImpl->transform = *transform;
        }
        return ret;
    }
};


struct RadialGradient::Impl
{
    float cx = 0.0f, cy = 0.0f;
    float fx = 0.0f, fy = 0.0f;
    float r = 0.0f, fr = 0.0f;

    Fill* duplicate();
    Result radial(float cx, float cy, float r, float fx, float fy, float fr);
};


struct LinearGradient::Impl
{
    float x1 = 0.0f;
    float y1 = 0.0f;
    float x2 = 0.0f;
    float y2 = 0.0f;

    Fill* duplicate();
};


#endif  //_TVG_FILL_H_

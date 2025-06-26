#ifndef _TVG_CANVAS_H_
#define _TVG_CANVAS_H_

#include "tvgPaint.h"


enum Status : uint8_t {Synced = 0, Updating, Drawing, Damaged};

struct Canvas::Impl
{
    list<Paint*> paints;
    RenderMethod* renderer;
    RenderRegion vport = {0, 0, INT32_MAX, INT32_MAX};
    Status status = Status::Synced;

    Impl(RenderMethod* pRenderer) : renderer(pRenderer)
    {
        renderer->ref();
    }

    ~Impl()
    {
        //make it sure any deferred jobs
        renderer->sync();
        renderer->clear();

        clearPaints();

        if (renderer->unref() == 0) delete(renderer);
    }

    void clearPaints()
    {
        for (auto paint : paints) {
            if (P(paint)->unref() == 0) delete(paint);
        }
        paints.clear();
    }

    Result push(unique_ptr<Paint> paint)
    {
        //You cannot push paints during rendering.
        if (status == Status::Drawing) return Result::InsufficientCondition;

        auto p = paint.release();
        if (!p) return Result::MemoryCorruption;
        PP(p)->ref();
        paints.push_back(p);

        return update(p, true);
    }

    Result clear(bool free)
    {
        //Clear render target before drawing
        if (!renderer->clear()) return Result::InsufficientCondition;

        //Free paints
        if (free) clearPaints();

        status = Status::Synced;

        return Result::Success;
    }

    Result update(Paint* paint, bool force)
    {
        if (paints.empty() || status == Status::Drawing) return Result::InsufficientCondition;

        Array<RenderData> clips;
        auto flag = RenderUpdateFlag::None;
        if (status == Status::Damaged || force) flag = RenderUpdateFlag::All;

        auto m = Matrix{1, 0, 0, 0, 1, 0, 0, 0, 1};

        if (paint) {
            paint->pImpl->update(renderer, m, clips, 255, flag);
        } else {
            for (auto paint : paints) {
                paint->pImpl->update(renderer, m, clips, 255, flag);
            }
        }
        status = Status::Updating;
        return Result::Success;
    }

    Result draw()
    {
        if (status == Status::Damaged) update(nullptr, false);
        if (status == Status::Drawing || paints.empty() || !renderer->preRender()) return Result::InsufficientCondition;

        bool rendered = false;
        for (auto paint : paints) {
            if (paint->pImpl->render(renderer)) rendered = true;
        }

        if (!rendered || !renderer->postRender()) return Result::InsufficientCondition;

        status = Status::Drawing;
        return Result::Success;
    }

    Result sync()
    {
        if (status == Status::Synced || status == Status::Damaged) return Result::InsufficientCondition;

        if (renderer->sync()) {
            status = Status::Synced;
            return Result::Success;
        }

        return Result::Unknown;
    }

    Result viewport(int32_t x, int32_t y, int32_t w, int32_t h)
    {
        if (status != Status::Damaged && status != Status::Synced) return Result::InsufficientCondition;

        RenderRegion val = {x, y, w, h};
        //intersect if the target buffer is already set.
        auto surface = renderer->mainSurface();
        if (surface && surface->w > 0 && surface->h > 0) {
            val.intersect({0, 0, (int32_t)surface->w, (int32_t)surface->h});
        }
        if (vport == val) return Result::Success;
        renderer->viewport(val);
        vport = val;
        status = Status::Damaged;
        return Result::Success;
    }
};

#endif /* _TVG_CANVAS_H_ */

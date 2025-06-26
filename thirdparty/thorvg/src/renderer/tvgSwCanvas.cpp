#include "tvgCanvas.h"
#include "tvgLoadModule.h"

#ifdef THORVG_SW_RASTER_SUPPORT
    #include "tvgSwRenderer.h"
#else
    class SwRenderer : public RenderMethod
    {
        //Non Supported. Dummy Class */
    };
#endif

/************************************************************************/
/* Internal Class Implementation                                        */
/************************************************************************/

struct SwCanvas::Impl
{
};


/************************************************************************/
/* External Class Implementation                                        */
/************************************************************************/

#ifdef THORVG_SW_RASTER_SUPPORT
SwCanvas::SwCanvas() : Canvas(SwRenderer::gen()), pImpl(nullptr)
#else
SwCanvas::SwCanvas() : Canvas(nullptr), pImpl(nullptr)
#endif
{
}


SwCanvas::~SwCanvas()
{
    delete(pImpl);
}


Result SwCanvas::mempool(MempoolPolicy policy) noexcept
{
#ifdef THORVG_SW_RASTER_SUPPORT
    //We know renderer type, avoid dynamic_cast for performance.
    auto renderer = static_cast<SwRenderer*>(Canvas::pImpl->renderer);
    if (!renderer) return Result::MemoryCorruption;

    //It can't change the policy during the running.
    if (!Canvas::pImpl->paints.empty()) return Result::InsufficientCondition;

    if (policy == MempoolPolicy::Individual) renderer->mempool(false);
    else renderer->mempool(true);

    return Result::Success;
#endif
    return Result::NonSupport;
}


Result SwCanvas::target(uint32_t* buffer, uint32_t stride, uint32_t w, uint32_t h, Colorspace cs) noexcept
{
#ifdef THORVG_SW_RASTER_SUPPORT
    if (Canvas::pImpl->status != Status::Damaged && Canvas::pImpl->status != Status::Synced) {
        return Result::InsufficientCondition;
    }

    //We know renderer type, avoid dynamic_cast for performance.
    auto renderer = static_cast<SwRenderer*>(Canvas::pImpl->renderer);
    if (!renderer) return Result::MemoryCorruption;

    if (!renderer->target(buffer, stride, w, h, static_cast<ColorSpace>(cs))) return Result::InvalidArguments;
    Canvas::pImpl->vport = {0, 0, (int32_t)w, (int32_t)h};
    renderer->viewport(Canvas::pImpl->vport);

    //FIXME: The value must be associated with an individual canvas instance.
    ImageLoader::cs = static_cast<ColorSpace>(cs);

    //Paints must be updated again with this new target.
    Canvas::pImpl->status = Status::Damaged;

    return Result::Success;
#endif
    return Result::NonSupport;
}


unique_ptr<SwCanvas> SwCanvas::gen() noexcept
{
#ifdef THORVG_SW_RASTER_SUPPORT
    if (SwRenderer::init() <= 0) return nullptr;
    return unique_ptr<SwCanvas>(new SwCanvas);
#endif
    return nullptr;
}

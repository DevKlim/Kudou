#include "tvgCanvas.h"

#ifdef THORVG_WG_RASTER_SUPPORT
    #include "tvgWgRenderer.h"
#endif

/************************************************************************/
/* Internal Class Implementation                                        */
/************************************************************************/

struct WgCanvas::Impl
{
};


/************************************************************************/
/* External Class Implementation                                        */
/************************************************************************/

#ifdef THORVG_WG_RASTER_SUPPORT
WgCanvas::WgCanvas() : Canvas(WgRenderer::gen()), pImpl(nullptr)
#else
WgCanvas::WgCanvas() : Canvas(nullptr), pImpl(nullptr)
#endif
{
}


WgCanvas::~WgCanvas()
{
#ifdef THORVG_WG_RASTER_SUPPORT
    auto renderer = static_cast<WgRenderer*>(Canvas::pImpl->renderer);
    renderer->target(nullptr, 0, 0);
#endif
}


Result WgCanvas::target(void* instance, void* surface, uint32_t w, uint32_t h, void* device) noexcept
{
#ifdef THORVG_WG_RASTER_SUPPORT
    if (Canvas::pImpl->status != Status::Damaged && Canvas::pImpl->status != Status::Synced) {
        return Result::InsufficientCondition;
    }

    if (!instance || !surface || (w == 0) || (h == 0)) return Result::InvalidArguments;

    //We know renderer type, avoid dynamic_cast for performance.
    auto renderer = static_cast<WgRenderer*>(Canvas::pImpl->renderer);
    if (!renderer) return Result::MemoryCorruption;

    if (!renderer->target((WGPUInstance)instance, (WGPUSurface)surface, w, h, (WGPUDevice)device)) return Result::Unknown;
    Canvas::pImpl->vport = {0, 0, (int32_t)w, (int32_t)h};
    renderer->viewport(Canvas::pImpl->vport);

    //Paints must be updated again with this new target.
    Canvas::pImpl->status = Status::Damaged;

    return Result::Success;
#endif
    return Result::NonSupport;
}


unique_ptr<WgCanvas> WgCanvas::gen() noexcept
{
#ifdef THORVG_WG_RASTER_SUPPORT
    return unique_ptr<WgCanvas>(new WgCanvas);
#endif
    return nullptr;
}

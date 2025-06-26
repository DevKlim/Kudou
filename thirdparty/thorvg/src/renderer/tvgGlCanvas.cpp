#include "tvgCanvas.h"

#ifdef THORVG_GL_RASTER_SUPPORT
    #include "tvgGlRenderer.h"
#else
    class GlRenderer : public RenderMethod
    {
        //Non Supported. Dummy Class */
    };
#endif

/************************************************************************/
/* Internal Class Implementation                                        */
/************************************************************************/

struct GlCanvas::Impl
{
};


/************************************************************************/
/* External Class Implementation                                        */
/************************************************************************/

#ifdef THORVG_GL_RASTER_SUPPORT
GlCanvas::GlCanvas() : Canvas(GlRenderer::gen()), pImpl(nullptr)
#else
GlCanvas::GlCanvas() : Canvas(nullptr), pImpl(nullptr)
#endif
{
}


GlCanvas::~GlCanvas()
{
    delete(pImpl);
}


Result GlCanvas::target(int32_t id, uint32_t w, uint32_t h) noexcept
{
#ifdef THORVG_GL_RASTER_SUPPORT
    if (Canvas::pImpl->status != Status::Damaged && Canvas::pImpl->status != Status::Synced) {
        return Result::InsufficientCondition;
    }

    //We know renderer type, avoid dynamic_cast for performance.
    auto renderer = static_cast<GlRenderer*>(Canvas::pImpl->renderer);
    if (!renderer) return Result::MemoryCorruption;

    if (!renderer->target(id, w, h)) return Result::Unknown;
    Canvas::pImpl->vport = {0, 0, (int32_t)w, (int32_t)h};
    renderer->viewport(Canvas::pImpl->vport);

    //Paints must be updated again with this new target.
    Canvas::pImpl->status = Status::Damaged;

    return Result::Success;
#endif
    return Result::NonSupport;
}


unique_ptr<GlCanvas> GlCanvas::gen() noexcept
{
#ifdef THORVG_GL_RASTER_SUPPORT
    if (GlRenderer::init() <= 0) return nullptr;
    return unique_ptr<GlCanvas>(new GlCanvas);
#endif
    return nullptr;
}

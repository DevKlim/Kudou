#include "tvgCanvas.h"

/************************************************************************/
/* External Class Implementation                                        */
/************************************************************************/

Canvas::Canvas(RenderMethod *pRenderer):pImpl(new Impl(pRenderer))
{
}


Canvas::~Canvas()
{
    delete(pImpl);
}


Result Canvas::reserve(TVG_UNUSED uint32_t n) noexcept
{
    return Result::NonSupport;
}


list<Paint*>& Canvas::paints() noexcept
{
    return pImpl->paints;
}


Result Canvas::push(unique_ptr<Paint> paint) noexcept
{
    return pImpl->push(std::move(paint));
}


Result Canvas::clear(bool free) noexcept
{
    return pImpl->clear(free);
}


Result Canvas::draw() noexcept
{
    TVGLOG("RENDERER", "Draw S. -------------------------------- Canvas(%p)", this);
    auto ret = pImpl->draw();
    TVGLOG("RENDERER", "Draw E. -------------------------------- Canvas(%p)", this);

    return ret;
}


Result Canvas::update(Paint* paint) noexcept
{
    TVGLOG("RENDERER", "Update S. ------------------------------ Canvas(%p)", this);
    auto ret = pImpl->update(paint, false);
    TVGLOG("RENDERER", "Update E. ------------------------------ Canvas(%p)", this);

    return ret;
}


Result Canvas::viewport(int32_t x, int32_t y, int32_t w, int32_t h) noexcept
{
    return pImpl->viewport(x, y, w, h);
}


Result Canvas::sync() noexcept
{
    return pImpl->sync();
}

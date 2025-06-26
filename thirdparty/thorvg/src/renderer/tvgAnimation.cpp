#include "tvgFrameModule.h"
#include "tvgAnimation.h"

/************************************************************************/
/* Internal Class Implementation                                        */
/************************************************************************/

/************************************************************************/
/* External Class Implementation                                        */
/************************************************************************/

Animation::~Animation()
{
    delete(pImpl);
}


Animation::Animation() : pImpl(new Impl)
{
}


Result Animation::frame(float no) noexcept
{
    auto loader = pImpl->picture->pImpl->loader;

    if (!loader) return Result::InsufficientCondition;
    if (!loader->animatable()) return Result::NonSupport;

    if (static_cast<FrameModule*>(loader)->frame(no)) return Result::Success;
    return Result::InsufficientCondition;
}


Picture* Animation::picture() const noexcept
{
    return pImpl->picture;
}


float Animation::curFrame() const noexcept
{
    auto loader = pImpl->picture->pImpl->loader;

    if (!loader) return 0;
    if (!loader->animatable()) return 0;

    return static_cast<FrameModule*>(loader)->curFrame();
}


float Animation::totalFrame() const noexcept
{
    auto loader = pImpl->picture->pImpl->loader;

    if (!loader) return 0;
    if (!loader->animatable()) return 0;

    return static_cast<FrameModule*>(loader)->totalFrame();
}


float Animation::duration() const noexcept
{
    auto loader = pImpl->picture->pImpl->loader;

    if (!loader) return 0;
    if (!loader->animatable()) return 0;

    return static_cast<FrameModule*>(loader)->duration();
}


Result Animation::segment(float begin, float end) noexcept
{
    if (begin < 0.0f || end > 1.0f || begin > end) return Result::InvalidArguments;

    auto loader = pImpl->picture->pImpl->loader;
    if (!loader) return Result::InsufficientCondition;
    if (!loader->animatable()) return Result::NonSupport;

    static_cast<FrameModule*>(loader)->segment(begin, end);

    return Result::Success;
}


Result Animation::segment(float *begin, float *end) noexcept
{
    auto loader = pImpl->picture->pImpl->loader;
    if (!loader) return Result::InsufficientCondition;
    if (!loader->animatable()) return Result::NonSupport;
    if (!begin && !end) return Result::InvalidArguments;

    static_cast<FrameModule*>(loader)->segment(begin, end);

    return Result::Success;
}


unique_ptr<Animation> Animation::gen() noexcept
{
    return unique_ptr<Animation>(new Animation);
}

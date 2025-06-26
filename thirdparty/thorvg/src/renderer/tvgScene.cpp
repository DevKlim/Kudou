#include <cstdarg>
#include "tvgScene.h"

/************************************************************************/
/* Internal Class Implementation                                        */
/************************************************************************/

Result Scene::Impl::resetEffects()
{
    if (effects) {
        for (auto e = effects->begin(); e < effects->end(); ++e) {
            delete(*e);
        }
        delete(effects);
        effects = nullptr;
    }
    return Result::Success;
}


/************************************************************************/
/* External Class Implementation                                        */
/************************************************************************/

Scene::Scene() : pImpl(new Impl(this))
{
}


Scene::~Scene()
{
    delete(pImpl);
}


unique_ptr<Scene> Scene::gen() noexcept
{
    return unique_ptr<Scene>(new Scene);
}


TVG_DEPRECATED uint32_t Scene::identifier() noexcept
{
    return (uint32_t) Type::Scene;
}


Type Scene::type() const noexcept
{
    return Type::Scene;
}


Result Scene::push(unique_ptr<Paint> paint) noexcept
{
    auto p = paint.release();
    if (!p) return Result::MemoryCorruption;
    P(p)->ref();

    //Relocated the paint to the current scene space
    P(p)->renderFlag |= RenderUpdateFlag::Transform;

    pImpl->paints.push_back(p);

    return Result::Success;
}


Result Scene::reserve(TVG_UNUSED uint32_t size) noexcept
{
    return Result::NonSupport;
}


Result Scene::clear(bool free) noexcept
{
    pImpl->clear(free);

    return Result::Success;
}


list<Paint*>& Scene::paints() noexcept
{
    return pImpl->paints;
}


Result Scene::push(SceneEffect effect, ...) noexcept
{
    if (effect == SceneEffect::ClearAll) return pImpl->resetEffects();

    if (!pImpl->effects) pImpl->effects = new Array<RenderEffect*>;

    va_list args;
    va_start(args, effect);

    RenderEffect* re = nullptr;

    switch (effect) {
        case SceneEffect::GaussianBlur: {
            re = RenderEffectGaussianBlur::gen(args);
            break;
        }
        case SceneEffect::DropShadow: {
            re = RenderEffectDropShadow::gen(args);
            break;
        }
        case SceneEffect::Fill: {
            re = RenderEffectFill::gen(args);
            break;
        }
        case SceneEffect::Tint: {
            re = RenderEffectTint::gen(args);
            break;
        }
        case SceneEffect::Tritone: {
            re = RenderEffectTritone::gen(args);
            break;
        }
        default: break;
    }

    if (!re) return Result::InvalidArguments;

    pImpl->effects->push(re);

    return Result::Success;
}

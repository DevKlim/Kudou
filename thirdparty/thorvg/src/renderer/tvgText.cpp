#include "tvgText.h"


/************************************************************************/
/* Internal Class Implementation                                        */
/************************************************************************/



/************************************************************************/
/* External Class Implementation                                        */
/************************************************************************/


Text::Text() : pImpl(new Impl(this))
{
}


Text::~Text()
{
    delete(pImpl);
}


Result Text::text(const char* text) noexcept
{
    return pImpl->text(text);
}


Result Text::font(const char* name, float size, const char* style) noexcept
{
    return pImpl->font(name, size, style);
}


Result Text::load(const std::string& path) noexcept
{
#ifdef THORVG_FILE_IO_SUPPORT
    bool invalid; //invalid path
    auto loader = LoaderMgr::loader(path, &invalid);
    if (loader) {
        if (loader->sharing > 0) --loader->sharing;   //font loading doesn't mean sharing.
        return Result::Success;
    } else {
        if (invalid) return Result::InvalidArguments;
        else return Result::NonSupport;
    }
    return Result::Success;
#else
    TVGLOG("RENDERER", "FILE IO is disabled!");
    return Result::NonSupport;
#endif
}


Result Text::load(const char* name, const char* data, uint32_t size, const string& mimeType, bool copy) noexcept
{
    if (!name || (size == 0 && data)) return Result::InvalidArguments;

    //unload font
    if (!data) {
        if (LoaderMgr::retrieve(name)) return Result::Success;
        return Result::InsufficientCondition;
    }

    if (!LoaderMgr::loader(name, data, size, mimeType, copy)) return Result::NonSupport;
    return Result::Success;
}


Result Text::unload(const std::string& path) noexcept
{
#ifdef THORVG_FILE_IO_SUPPORT
    if (LoaderMgr::retrieve(path)) return Result::Success;
    return Result::InsufficientCondition;
#else
    TVGLOG("RENDERER", "FILE IO is disabled!");
    return Result::NonSupport;
#endif
}


Result Text::fill(uint8_t r, uint8_t g, uint8_t b) noexcept
{
    return pImpl->shape->fill(r, g, b);
}


Result Text::fill(unique_ptr<Fill> f) noexcept
{
    return pImpl->shape->fill(std::move(f));
}


unique_ptr<Text> Text::gen() noexcept
{
    return unique_ptr<Text>(new Text);
}


Type Text::type() const noexcept
{
    return Type::Text;
}

#include "tvgIteratorAccessor.h"
#include "tvgCompressor.h"

/************************************************************************/
/* Internal Class Implementation                                        */
/************************************************************************/

static bool accessChildren(Iterator* it, function<bool(const Paint* paint, void* data)> func, void* data)
{
    while (auto child = it->next()) {
        //Access the child
        if (!func(child, data)) return false;

        //Access the children of the child
        if (auto it2 = IteratorAccessor::iterator(child)) {
            if (!accessChildren(it2, func, data)) {
                delete(it2);
                return false;
            }
            delete(it2);
        }
    }
    return true;
}


/************************************************************************/
/* External Class Implementation                                        */
/************************************************************************/

TVG_DEPRECATED unique_ptr<Picture> Accessor::set(unique_ptr<Picture> picture, function<bool(const Paint* paint)> func) noexcept
{
    auto backward = [](const tvg::Paint* paint, void* data) -> bool
    {
        auto func = reinterpret_cast<function<bool(const Paint* paint)>*>(data);
        if (!(*func)(paint)) return false;
        return true;
    };

    set(picture.get(), backward, reinterpret_cast<void*>(&func));
    return picture;
}


Result Accessor::set(Paint* paint, function<bool(const Paint* paint, void* data)> func, void* data) noexcept
{
    if (!paint || !func) return Result::InvalidArguments;

    //Use the Preorder Tree-Searc

    //Root
    if (!func(paint, data)) return Result::Success;

    //Children
    if (auto it = IteratorAccessor::iterator(paint)) {
        accessChildren(it, func, data);
        delete(it);
    }
    return Result::Success;
}


uint32_t Accessor::id(const char* name) noexcept
{
    return djb2Encode(name);
}


Accessor::~Accessor()
{

}


Accessor::Accessor() : pImpl(nullptr)
{

}


unique_ptr<Accessor> Accessor::gen() noexcept
{
    return unique_ptr<Accessor>(new Accessor);
}

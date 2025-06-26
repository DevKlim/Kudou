#ifndef _TVG_SAVE_MODULE_H_
#define _TVG_SAVE_MODULE_H_

#include "tvgIteratorAccessor.h"

namespace tvg
{

class SaveModule
{
public:
    virtual ~SaveModule() {}

    virtual bool save(Paint* paint, const string& path, bool compress) = 0;
    virtual bool save(Animation* animation, Paint* bg, const string& path, uint32_t quality, uint32_t fps) = 0;
    virtual bool close() = 0;
};

}

#endif //_TVG_SAVE_MODULE_H_

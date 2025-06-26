#ifndef _TVG_ITERATOR_ACCESSOR_H_
#define _TVG_ITERATOR_ACCESSOR_H_

#include "tvgPaint.h"

namespace tvg
{

class IteratorAccessor
{
public:
    //Utility Method: Iterator Accessor
    static Iterator* iterator(const Paint* paint)
    {
        return paint->pImpl->iterator();
    }
};

}

#endif //_TVG_ITERATOR_ACCESSOR_H_

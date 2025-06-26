#ifndef _TVG_LOCK_H_
#define _TVG_LOCK_H_

#ifdef THORVG_THREAD_SUPPORT

#include <mutex>
#include "tvgTaskScheduler.h"

namespace tvg {

    struct Key
    {
        std::mutex mtx;
    };

    struct ScopedLock
    {
        Key* key = nullptr;

        ScopedLock(Key& k)
        {
            if (TaskScheduler::threads() > 0) {
                k.mtx.lock();
                key = &k;
            }
        }

        ~ScopedLock()
        {
            if (TaskScheduler::threads() > 0) {
                key->mtx.unlock();
            }
        }
    };

}

#else //THORVG_THREAD_SUPPORT

namespace tvg {

    struct Key {};

    struct ScopedLock
    {
        ScopedLock(Key& key) {}
    };

}

#endif //THORVG_THREAD_SUPPORT

#endif //_TVG_LOCK_H_


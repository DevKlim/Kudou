#pragma once
#ifndef VHACD_MUTEX_H
#define VHACD_MUTEX_H

#if defined(WIN32)

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x400
#endif
#include <windows.h>
#pragma comment(lib, "winmm.lib")
#endif

#if defined(__linux__)
//#include <sys/time.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>
#define __stdcall
#endif

#if defined(__APPLE__) || defined(__linux__)
#include <pthread.h>
#endif

#if defined(__APPLE__) || !defined(__GLIBC__)
#define PTHREAD_MUTEX_RECURSIVE_NP PTHREAD_MUTEX_RECURSIVE
#endif

#define VHACD_DEBUG

//#define VHACD_NDEBUG
#ifdef VHACD_NDEBUG
#define VHACD_VERIFY(x) (x)
#else
#define VHACD_VERIFY(x) assert((x))
#endif

namespace VHACD {
class Mutex {
public:
    Mutex(void)
    {
#if defined(WIN32) || defined(_XBOX)
        InitializeCriticalSection(&m_mutex);
#elif defined(__APPLE__) || defined(__linux__)
        pthread_mutexattr_t mutexAttr; // Mutex Attribute
        VHACD_VERIFY(pthread_mutexattr_init(&mutexAttr) == 0);
        VHACD_VERIFY(pthread_mutexattr_settype(&mutexAttr, PTHREAD_MUTEX_RECURSIVE_NP) == 0);
        VHACD_VERIFY(pthread_mutex_init(&m_mutex, &mutexAttr) == 0);
        VHACD_VERIFY(pthread_mutexattr_destroy(&mutexAttr) == 0);
#endif
    }
    ~Mutex(void)
    {
#if defined(WIN32) || defined(_XBOX)
        DeleteCriticalSection(&m_mutex);
#elif defined(__APPLE__) || defined(__linux__)
        VHACD_VERIFY(pthread_mutex_destroy(&m_mutex) == 0);
#endif
    }
    void Lock(void)
    {
#if defined(WIN32) || defined(_XBOX)
        EnterCriticalSection(&m_mutex);
#elif defined(__APPLE__) || defined(__linux__)
        VHACD_VERIFY(pthread_mutex_lock(&m_mutex) == 0);
#endif
    }
    bool TryLock(void)
    {
#if defined(WIN32) || defined(_XBOX)
        bool bRet = false;
        //assert(("TryEnterCriticalSection seems to not work on XP???", 0));
        bRet = TryEnterCriticalSection(&m_mutex) ? true : false;
        return bRet;
#elif defined(__APPLE__) || defined(__linux__)
        int32_t result = pthread_mutex_trylock(&m_mutex);
        return (result == 0);
#endif
    }

    void Unlock(void)
    {
#if defined(WIN32) || defined(_XBOX)
        LeaveCriticalSection(&m_mutex);
#elif defined(__APPLE__) || defined(__linux__)
        VHACD_VERIFY(pthread_mutex_unlock(&m_mutex) == 0);
#endif
    }

private:
#if defined(WIN32) || defined(_XBOX)
    CRITICAL_SECTION m_mutex;
#elif defined(__APPLE__) || defined(__linux__)
    pthread_mutex_t m_mutex;
#endif
};
}
#endif // VHACD_MUTEX_H

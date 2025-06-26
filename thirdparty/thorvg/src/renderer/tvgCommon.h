#ifndef _TVG_COMMON_H_
#define _TVG_COMMON_H_

#include "config.h"
#include "thorvg.h"

using namespace std;
using namespace tvg;

//for MSVC Compat
#ifdef _MSC_VER
    #define TVG_UNUSED
    #define strncasecmp _strnicmp
    #define strcasecmp _stricmp
#else
    #define TVG_UNUSED __attribute__ ((__unused__))
#endif

// Portable 'fallthrough' attribute
#if __has_cpp_attribute(fallthrough)
    #ifdef _MSC_VER
        #define TVG_FALLTHROUGH [[fallthrough]];
    #else
        #define TVG_FALLTHROUGH __attribute__ ((fallthrough));
    #endif
#else
    #define TVG_FALLTHROUGH
#endif

#if defined(_MSC_VER) && defined(__clang__)
    #define strncpy strncpy_s
    #define strdup _strdup
#endif

enum class FileType { Png = 0, Jpg, Webp, Tvg, Svg, Lottie, Ttf, Raw, Gif, Unknown };

using Size = Point;

#ifdef THORVG_LOG_ENABLED
    constexpr auto ErrorColor = "\033[31m";  //red
    constexpr auto ErrorBgColor = "\033[41m";//bg red
    constexpr auto LogColor = "\033[32m";    //green
    constexpr auto LogBgColor = "\033[42m";  //bg green
    constexpr auto GreyColor = "\033[90m";   //grey
    constexpr auto ResetColors = "\033[0m";  //default
    #define TVGERR(tag, fmt, ...) fprintf(stderr, "%s[E]%s %s" tag "%s (%s %d): %s" fmt "\n", ErrorBgColor, ResetColors, ErrorColor, GreyColor, __FILE__, __LINE__, ResetColors, ##__VA_ARGS__)
    #define TVGLOG(tag, fmt, ...) fprintf(stdout, "%s[L]%s %s" tag "%s (%s %d): %s" fmt "\n", LogBgColor, ResetColors, LogColor, GreyColor, __FILE__, __LINE__, ResetColors, ##__VA_ARGS__)
#else
    #define TVGERR(...) do {} while(0)
    #define TVGLOG(...) do {} while(0)
#endif

uint16_t THORVG_VERSION_NUMBER();


#define P(A) ((A)->pImpl)              //Access to pimpl.
#define PP(A) (((Paint*)(A))->pImpl)   //Access to pimpl.


//for debugging
#if 0
#include <sys/time.h>
static inline double THORVG_TIMESTAMP()
{
   struct timeval tv;
   gettimeofday(&tv, NULL);
   return (tv.tv_sec + tv.tv_usec / 1000000.0);
}
#endif

#endif //_TVG_COMMON_H_

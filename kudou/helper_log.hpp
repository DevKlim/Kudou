
#ifndef GDCEF_HELPER_LOG_HPP
#define GDCEF_HELPER_LOG_HPP

#include "core/io/marshalls.h"
#include "core/string/ustring.h"

#include <sstream>

// ****************************************************************************
// Logging macros using Godot's logging system with stringstream
// ****************************************************************************

#define PRINT_ERROR(x)                                   \
    {                                                    \
        std::stringstream ss;                            \
        ss << "[gdCEF][ERROR][" << __func__ << "]" << x; \
        ERR_PRINT(String(ss.str().c_str()));      \
    }

#define GDCEF_DEBUG(x)                                    \
    {                                                     \
        std::stringstream ss;                             \
        ss << "[gdCEF][GDCef::" << __func__ << "] " << x; \
        WARN_PRINT(String(ss.str().c_str()));      \
    }

#define GDCEF_ERROR(x)                                    \
    {                                                     \
        std::stringstream ss;                             \
        ss << "[gdCEF][GDCef::" << __func__ << "] " << x; \
        ERR_PRINT(String(ss.str().c_str()));       \
    }

#define GDCEF_WARNING(x)                                  \
    {                                                     \
        std::stringstream ss;                             \
        ss << "[gdCEF][GDCef::" << __func__ << "] " << x; \
        WARN_PRINT(String(ss.str().c_str()));      \
    }

#define BROWSER_DEBUG(txt)                                              \
    {                                                                   \
        std::stringstream ss;                                           \
        String name = get_name();                                \
        ss << "[gdCEF][GDBrowserView::" << __func__ << "][id: " << m_id \
           << ", name: " << name.utf8().get_data() << "] " << txt;      \
        WARN_PRINT(String(ss.str().c_str()));                    \
    }

#define BROWSER_ERROR(txt)                                              \
    {                                                                   \
        std::stringstream ss;                                           \
        String name = get_name();                                \
        ss << "[gdCEF][GDBrowserView::" << __func__ << "][id: " << m_id \
           << ", name: " << name.utf8().get_data() << "] " << txt;      \
        ERR_PRINT(String(ss.str().c_str()));                     \
        m_error << ss.str();                                            \
    }

#endif // GDCEF_HELPER_LOG_HPP


#ifndef GDCEF_HELPER_CONFIG_HPP
#define GDCEF_HELPER_CONFIG_HPP

#include "gdcef.hpp"
#include "helper_files.hpp"

#include "core/os/os.h"

template <class T>
inline T getConfig(const Dictionary& config,
                   const char* property,
                   T const& default_value)
{
    if ((property != nullptr) && config.has(property))
        return config[property];
    return default_value;
}

template <>
inline std::string getConfig<std::string>(const Dictionary& config,
                                          const char* property,
                                          std::string const& default_value)
{
    if ((property != nullptr) && config.has(property))
    {
        String str = config[property];
        return str.utf8().get_data();
    }
    return default_value;
}

template <>
inline fs::path getConfig<fs::path>(const Dictionary& config,
                                    const char* property,
                                    fs::path const& default_value)
{
    if ((property != nullptr) && config.has(property))
    {
        String str = config[property];
        return str.utf8().get_data();
    }
    return default_value;
}

template <>
inline cef_state_t getConfig<cef_state_t>(const Dictionary& config,
                                          const char* property,
                                          cef_state_t const& default_value)
{
    if ((property != nullptr) && config.has(property))
        return config[property] ? STATE_ENABLED : STATE_DISABLED;
    return default_value;
}

#endif // GDCEF_HELPER_CONFIG_HPP
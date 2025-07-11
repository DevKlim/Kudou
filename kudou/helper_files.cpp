
#include "helper_files.hpp"
#include "helper_log.hpp"
#include <fstream>
#include <iostream>

#if defined(_WIN32)
#    include <Windows.h>
#else
#    include <unistd.h>
#endif

bool are_valid_files(fs::path const& folder,
                     std::vector<std::string> const& files)
{
    bool failure = false;

    for (auto const& it : files)
    {
        fs::path f = {folder / it};
        if (!fs::exists(f))
        {
            PRINT_ERROR(
                "CEF artifact " << f << " is missing and is needed for CEF");
            failure = true;
        }
    }

    return !failure;
}

std::string executable_name()
{
#if defined(_WIN32)
#    pragma comment(lib, "kernel32.lib")
    char buf[MAX_PATH];
    GetModuleFileNameA(nullptr, buf, MAX_PATH);
    return buf;

#else

    char path[1024];
    if (readlink("/proc/self/exe", path, 1024) == -1)
        return {};
    return path;

#endif
}

fs::path real_path()
{
#if defined(_WIN32)

    return fs::canonical({fs::current_path() / executable_name()})
        .parent_path();

#else

    fs::path p(executable_name());
    return p.parent_path();

#endif
}

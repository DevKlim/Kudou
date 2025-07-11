
#ifndef GDCEF_HELPER_FILES_HPP
#define GDCEF_HELPER_FILES_HPP

#include "helper_log.hpp"
#include "core/config/project_settings.h"
#include <vector>

#if __has_include(<filesystem>)
#    include <filesystem>
namespace fs = std::filesystem;
#elif __has_include(<experimental/filesystem>)
#    include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#else
#    error "Missing the <filesystem> header."
#endif

#define GLOBALIZE_PATH(path)                \
    ProjectSettings::get_singleton() \
        ->globalize_path(path)              \
        .utf8()                             \
        .get_data()

std::string executable_name();

bool are_valid_files(std::filesystem::path const& folder,
                     std::vector<std::string> const& files);

fs::path real_path();

String convert_godot_url(String const& url);

#endif // GDCEF_HELPER_FILES_HPP
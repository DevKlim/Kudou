#pragma once

#include "core/string/ustring.h"

namespace GodotSharpDirs {

String get_res_metadata_dir();
String get_res_temp_assemblies_dir();

String get_api_assemblies_dir();

String get_mono_user_dir();

#ifdef TOOLS_ENABLED
String get_build_logs_dir();
String get_data_editor_tools_dir();
#endif

} // namespace GodotSharpDirs

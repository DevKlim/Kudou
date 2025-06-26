#pragma once

class String;

namespace TestUtils {

String get_data_path(const String &p_file);
String get_executable_dir();
String get_temp_path(const String &p_suffix);
} // namespace TestUtils

#pragma once

#include "core/object/class_db.h"

class ShaderIncludeDB : public Object {
	GDCLASS(ShaderIncludeDB, Object)

private:
	static HashMap<String, String> built_in_includes;

protected:
	static void _bind_methods();

public:
	static void register_built_in_include_file(const String &p_filename, const String &p_shader_code);
	static PackedStringArray list_built_in_include_files();
	static bool has_built_in_include_file(const String &p_filename);
	static String get_built_in_include_file(const String &p_filename);
	static String parse_include_files(const String &p_code);
};

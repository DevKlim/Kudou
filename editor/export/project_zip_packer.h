#pragma once

#include "core/io/zip_io.h"
#include "core/variant/variant.h"

class ProjectZIPPacker {
	static void _zip_file(const String &p_path, const String &p_base_path, zipFile p_zip);
	static void _zip_recursive(const String &p_path, const String &p_base_path, zipFile p_zip);

public:
	static String get_project_zip_safe_name();
	static void pack_project_zip(const String &p_path);
};

#pragma once

#include "core/extension/gdextension.h"

#ifdef TOOLS_ENABLED

class GDExtensionAPIDump {
public:
	static Dictionary generate_extension_api(bool p_include_docs = false);
	static void generate_extension_json_file(const String &p_path, bool p_include_docs = false);
	static Error validate_extension_json_file(const String &p_path);
};
#endif

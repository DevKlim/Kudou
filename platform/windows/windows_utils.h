#pragma once

#ifdef WINDOWS_ENABLED

#include "core/string/ustring.h"
#include "core/templates/hash_map.h"

class WindowsUtils {
	static HashMap<String, Vector<String>> temp_pdbs;

public:
	static Error copy_and_rename_pdb(const String &p_dll_path);
	static void remove_temp_pdbs(const String &p_dll_path);
};

#endif // WINDOWS_ENABLED

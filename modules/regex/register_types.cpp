#include "register_types.h"

#include "regex.h"

#include "core/object/class_db.h"

void initialize_regex_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	GDREGISTER_CLASS(RegExMatch);
	GDREGISTER_CLASS(RegEx);
}

void uninitialize_regex_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
}

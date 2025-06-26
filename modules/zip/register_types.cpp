#include "register_types.h"

#include "zip_packer.h"
#include "zip_reader.h"

#include "core/object/class_db.h"

void initialize_zip_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	GDREGISTER_CLASS(ZIPPacker);
	GDREGISTER_CLASS(ZIPReader);
}

void uninitialize_zip_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
}

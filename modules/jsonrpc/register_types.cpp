#include "register_types.h"

#include "jsonrpc.h"

#include "core/object/class_db.h"

void initialize_jsonrpc_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	GDREGISTER_CLASS(JSONRPC);
}

void uninitialize_jsonrpc_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
}

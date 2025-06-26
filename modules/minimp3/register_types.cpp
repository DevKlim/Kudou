#include "register_types.h"

#include "audio_stream_mp3.h"

#ifdef TOOLS_ENABLED
#include "core/config/engine.h"
#include "editor/editor_node.h"
#include "resource_importer_mp3.h"

static void _editor_init() {
	Ref<ResourceImporterMP3> mp3_import;
	mp3_import.instantiate();
	ResourceFormatImporter::get_singleton()->add_importer(mp3_import);
}
#endif

void initialize_minimp3_module(ModuleInitializationLevel p_level) {
	if (p_level == MODULE_INITIALIZATION_LEVEL_SCENE) {
		GDREGISTER_CLASS(AudioStreamMP3);
	}

#ifdef TOOLS_ENABLED
	if (p_level == MODULE_INITIALIZATION_LEVEL_EDITOR) {
		GDREGISTER_CLASS(ResourceImporterMP3);

		EditorNode::add_init_callback(_editor_init);
	}
#endif
}

void uninitialize_minimp3_module(ModuleInitializationLevel p_level) {
}

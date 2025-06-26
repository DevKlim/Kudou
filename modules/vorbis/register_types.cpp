#include "register_types.h"

#include "audio_stream_ogg_vorbis.h"

#ifdef TOOLS_ENABLED
#include "editor/editor_node.h"
#include "resource_importer_ogg_vorbis.h"

static void _editor_init() {
	Ref<ResourceImporterOggVorbis> ogg_vorbis_importer;
	ogg_vorbis_importer.instantiate();
	ResourceFormatImporter::get_singleton()->add_importer(ogg_vorbis_importer);
}
#endif

void initialize_vorbis_module(ModuleInitializationLevel p_level) {
	if (p_level == MODULE_INITIALIZATION_LEVEL_SCENE) {
		GDREGISTER_CLASS(AudioStreamOggVorbis);
		GDREGISTER_CLASS(AudioStreamPlaybackOggVorbis);
	}

#ifdef TOOLS_ENABLED
	if (p_level == MODULE_INITIALIZATION_LEVEL_EDITOR) {
		GDREGISTER_CLASS(ResourceImporterOggVorbis);

		EditorNode::add_init_callback(_editor_init);
	}
#endif
}

void uninitialize_vorbis_module(ModuleInitializationLevel p_level) {
}

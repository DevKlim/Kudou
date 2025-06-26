#include "register_types.h"

#include "audio_stream_interactive.h"
#include "audio_stream_playlist.h"
#include "audio_stream_synchronized.h"
#include "core/object/class_db.h"

#ifdef TOOLS_ENABLED
#include "editor/audio_stream_interactive_editor_plugin.h"
#endif

void initialize_interactive_music_module(ModuleInitializationLevel p_level) {
	if (p_level == MODULE_INITIALIZATION_LEVEL_SCENE) {
		GDREGISTER_CLASS(AudioStreamPlaylist);
		GDREGISTER_ABSTRACT_CLASS(AudioStreamPlaybackPlaylist);
		GDREGISTER_CLASS(AudioStreamInteractive);
		GDREGISTER_ABSTRACT_CLASS(AudioStreamPlaybackInteractive);
		GDREGISTER_CLASS(AudioStreamSynchronized);
		GDREGISTER_ABSTRACT_CLASS(AudioStreamPlaybackSynchronized);
	}
#ifdef TOOLS_ENABLED
	if (p_level == MODULE_INITIALIZATION_LEVEL_EDITOR) {
		EditorPlugins::add_by_type<AudioStreamInteractiveEditorPlugin>();
	}
#endif
}

void uninitialize_interactive_music_module(ModuleInitializationLevel p_level) {
	// Nothing to do here.
}

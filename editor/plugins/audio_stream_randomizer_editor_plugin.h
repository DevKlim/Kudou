#pragma once

#include "editor/plugins/editor_plugin.h"

class AudioStreamRandomizerEditorPlugin : public EditorPlugin {
	GDCLASS(AudioStreamRandomizerEditorPlugin, EditorPlugin);

private:
	void _move_stream_array_element(Object *p_undo_redo, Object *p_edited, const String &p_array_prefix, int p_from_index, int p_to_pos);

public:
	virtual String get_plugin_name() const override { return "AudioStreamRandomizer"; }
	bool has_main_screen() const override { return false; }
	virtual void edit(Object *p_object) override;
	virtual bool handles(Object *p_object) const override;
	virtual void make_visible(bool p_visible) override;

	AudioStreamRandomizerEditorPlugin();
};

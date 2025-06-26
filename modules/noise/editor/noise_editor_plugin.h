#pragma once

#include "editor/plugins/editor_plugin.h"

class NoiseEditorPlugin : public EditorPlugin {
	GDCLASS(NoiseEditorPlugin, EditorPlugin)

public:
	String get_plugin_name() const override;

	NoiseEditorPlugin();
};

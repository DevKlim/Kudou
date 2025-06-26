#pragma once

#include "core/io/zip_io.h"
#include "editor/plugins/editor_plugin.h"

class WebToolsEditorPlugin : public EditorPlugin {
	GDCLASS(WebToolsEditorPlugin, EditorPlugin);

private:
	void _download_zip();

public:
	static void initialize();

	WebToolsEditorPlugin();
};

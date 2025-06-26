#pragma once

#include "editor/plugins/editor_plugin.h"
#include "editor/plugins/texture_editor_plugin.h"

class EditorInspectorPluginSubViewportPreview : public EditorInspectorPluginTexture {
	GDCLASS(EditorInspectorPluginSubViewportPreview, EditorInspectorPluginTexture);

public:
	virtual bool can_handle(Object *p_object) override;
	virtual void parse_begin(Object *p_object) override;
};

class SubViewportPreviewEditorPlugin : public EditorPlugin {
	GDCLASS(SubViewportPreviewEditorPlugin, EditorPlugin);

public:
	virtual String get_plugin_name() const override { return "SubViewportPreview"; }

	SubViewportPreviewEditorPlugin();
};

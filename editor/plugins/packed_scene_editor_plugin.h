#pragma once

#include "editor/editor_inspector.h"
#include "editor/plugins/editor_plugin.h"
#include "scene/gui/box_container.h"

class PackedSceneEditor : public VBoxContainer {
	GDCLASS(PackedSceneEditor, VBoxContainer);

	Ref<PackedScene> packed_scene;
	Button *open_scene_button;

	void _on_open_scene_pressed();

protected:
	void _notification(int p_what);

public:
	PackedSceneEditor(Ref<PackedScene> &p_packed_scene);
};

class EditorInspectorPluginPackedScene : public EditorInspectorPlugin {
	GDCLASS(EditorInspectorPluginPackedScene, EditorInspectorPlugin);

public:
	virtual bool can_handle(Object *p_object) override;
	virtual void parse_begin(Object *p_object) override;
};

class PackedSceneEditorPlugin : public EditorPlugin {
	GDCLASS(PackedSceneEditorPlugin, EditorPlugin);

public:
	PackedSceneEditorPlugin();
};

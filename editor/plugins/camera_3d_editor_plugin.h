#pragma once

#include "editor/plugins/editor_plugin.h"
#include "editor/plugins/texture_editor_plugin.h"

class Camera3D;
class SubViewport;

class Camera3DEditor : public Control {
	GDCLASS(Camera3DEditor, Control);

	Panel *panel = nullptr;
	Button *preview = nullptr;
	Node *node = nullptr;

	void _pressed();

protected:
	void _node_removed(Node *p_node);

public:
	void edit(Node *p_camera);
	Camera3DEditor();
};

class Camera3DPreview : public TexturePreview {
	GDCLASS(Camera3DPreview, TexturePreview);

	Camera3D *camera = nullptr;
	SubViewport *sub_viewport = nullptr;

	void _update_sub_viewport_size();

public:
	Camera3DPreview(Camera3D *p_camera);
};

class EditorInspectorPluginCamera3DPreview : public EditorInspectorPluginTexture {
	GDCLASS(EditorInspectorPluginCamera3DPreview, EditorInspectorPluginTexture);

public:
	virtual bool can_handle(Object *p_object) override;
	virtual void parse_begin(Object *p_object) override;
};

class Camera3DEditorPlugin : public EditorPlugin {
	GDCLASS(Camera3DEditorPlugin, EditorPlugin);

public:
	virtual String get_plugin_name() const override { return "Camera3D"; }
	bool has_main_screen() const override { return false; }
	virtual void edit(Object *p_object) override;
	virtual bool handles(Object *p_object) const override;
	virtual void make_visible(bool p_visible) override;

	Camera3DEditorPlugin();
};

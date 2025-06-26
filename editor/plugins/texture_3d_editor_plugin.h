#pragma once

#include "editor/editor_inspector.h"
#include "editor/plugins/editor_plugin.h"
#include "scene/gui/spin_box.h"
#include "scene/resources/shader.h"
#include "scene/resources/texture.h"

class ColorChannelSelector;

class Texture3DEditor : public Control {
	GDCLASS(Texture3DEditor, Control);

	struct ThemeCache {
		Color outline_color;
	} theme_cache;

	SpinBox *layer = nullptr;
	Label *info = nullptr;
	Ref<Texture3D> texture;

	Ref<Shader> shader;
	Ref<ShaderMaterial> material;

	Control *texture_rect = nullptr;

	ColorChannelSelector *channel_selector = nullptr;

	bool setting = false;

	void _draw_outline();

	void _make_shaders();

	void _layer_changed(double) {
		if (!setting) {
			_update_material(false);
		}
	}

	void _texture_changed();

	void _texture_rect_update_area();
	void _texture_rect_draw();

	void _update_material(bool p_texture_changed);
	void _update_gui();

	void on_selected_channels_changed();

protected:
	void _notification(int p_what);

public:
	void edit(Ref<Texture3D> p_texture);

	Texture3DEditor();
	~Texture3DEditor();
};

class EditorInspectorPlugin3DTexture : public EditorInspectorPlugin {
	GDCLASS(EditorInspectorPlugin3DTexture, EditorInspectorPlugin);

public:
	virtual bool can_handle(Object *p_object) override;
	virtual void parse_begin(Object *p_object) override;
};

class Texture3DEditorPlugin : public EditorPlugin {
	GDCLASS(Texture3DEditorPlugin, EditorPlugin);

public:
	virtual String get_plugin_name() const override { return "Texture3D"; }

	Texture3DEditorPlugin();
};

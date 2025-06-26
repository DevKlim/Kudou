#pragma once

#include "editor/editor_inspector.h"
#include "editor/plugins/editor_plugin.h"
#include "scene/gui/spin_box.h"
#include "scene/resources/shader.h"
#include "scene/resources/texture.h"

class ColorChannelSelector;

class TextureLayeredEditor : public Control {
	GDCLASS(TextureLayeredEditor, Control);

	struct ThemeCache {
		Color outline_color;
	} theme_cache;

	SpinBox *layer = nullptr;
	Label *info = nullptr;
	Ref<TextureLayered> texture;

	Ref<Shader> shaders[3];
	Ref<ShaderMaterial> materials[3];

	float x_rot = 0;
	float y_rot = 0;
	Control *texture_rect = nullptr;

	bool setting = false;

	ColorChannelSelector *channel_selector = nullptr;

	void _draw_outline();

	void _make_shaders();
	void _update_material(bool p_texture_changed);

	void _layer_changed(double) {
		if (!setting) {
			_update_material(false);
		}
	}

	void _texture_changed();

	void _texture_rect_update_area();
	void _texture_rect_draw();

	void _update_gui();

	void on_selected_channels_changed();

protected:
	void _notification(int p_what);
	virtual void gui_input(const Ref<InputEvent> &p_event) override;

public:
	void edit(Ref<TextureLayered> p_texture);

	TextureLayeredEditor();
};

class EditorInspectorPluginLayeredTexture : public EditorInspectorPlugin {
	GDCLASS(EditorInspectorPluginLayeredTexture, EditorInspectorPlugin);

public:
	virtual bool can_handle(Object *p_object) override;
	virtual void parse_begin(Object *p_object) override;
};

class TextureLayeredEditorPlugin : public EditorPlugin {
	GDCLASS(TextureLayeredEditorPlugin, EditorPlugin);

public:
	virtual String get_plugin_name() const override { return "TextureLayered"; }

	TextureLayeredEditorPlugin();
};

#pragma once

#include "editor/editor_inspector.h"
#include "editor/plugins/editor_plugin.h"
#include "scene/gui/margin_container.h"
#include "scene/resources/texture.h"

class AspectRatioContainer;
class ColorRect;
class TextureRect;
class ShaderMaterial;
class ColorChannelSelector;

class TexturePreview : public MarginContainer {
	GDCLASS(TexturePreview, MarginContainer);

private:
	struct ThemeCache {
		Color outline_color;
	} theme_cache;

	TextureRect *texture_display = nullptr;

	MarginContainer *margin_container = nullptr;
	Control *outline_overlay = nullptr;
	AspectRatioContainer *centering_container = nullptr;
	ColorRect *bg_rect = nullptr;
	TextureRect *checkerboard = nullptr;
	Label *metadata_label = nullptr;
	Ref<ShaderMaterial> material;

	ColorChannelSelector *channel_selector = nullptr;

	void _draw_outline();
	void _update_metadata_label_text();

protected:
	void _notification(int p_what);
	void _update_texture_display_ratio();

	void on_selected_channels_changed();

public:
	TextureRect *get_texture_display();
	TexturePreview(Ref<Texture2D> p_texture, bool p_show_metadata);
};

class EditorInspectorPluginTexture : public EditorInspectorPlugin {
	GDCLASS(EditorInspectorPluginTexture, EditorInspectorPlugin);

public:
	virtual bool can_handle(Object *p_object) override;
	virtual void parse_begin(Object *p_object) override;
};

class TextureEditorPlugin : public EditorPlugin {
	GDCLASS(TextureEditorPlugin, EditorPlugin);

public:
	virtual String get_plugin_name() const override { return "Texture2D"; }

	TextureEditorPlugin();
};

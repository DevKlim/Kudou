#pragma once

#include "scene/gui/box_container.h"
#include "scene/resources/theme.h"

class Button;
class ColorPickerButton;
class ColorRect;
class MarginContainer;
class ScrollContainer;

class ThemeEditorPreview : public VBoxContainer {
	GDCLASS(ThemeEditorPreview, VBoxContainer);

	ScrollContainer *preview_container = nullptr;
	MarginContainer *preview_root = nullptr;
	ColorRect *preview_bg = nullptr;
	MarginContainer *preview_overlay = nullptr;
	Control *picker_overlay = nullptr;
	Control *hovered_control = nullptr;

	struct ThemeCache {
		Ref<StyleBox> preview_picker_overlay;
		Color preview_picker_overlay_color;
		Ref<StyleBox> preview_picker_label;
		Ref<Font> preview_picker_font;
		int font_size = 16;
	} theme_cache;

	double time_left = 0;

	void _propagate_redraw(Control *p_at);
	void _refresh_interval();
	void _preview_visibility_changed();

	void _picker_button_cbk();
	Control *_find_hovered_control(Control *p_parent, Vector2 p_mouse_position);

	void _draw_picker_overlay();
	void _gui_input_picker_overlay(const Ref<InputEvent> &p_event);
	void _reset_picker_overlay();

protected:
	HBoxContainer *preview_toolbar = nullptr;
	MarginContainer *preview_content = nullptr;
	Button *picker_button = nullptr;

	void add_preview_overlay(Control *p_overlay);

	void _notification(int p_what);
	static void _bind_methods();

public:
	void set_preview_theme(const Ref<Theme> &p_theme);

	ThemeEditorPreview();
};

class DefaultThemeEditorPreview : public ThemeEditorPreview {
	GDCLASS(DefaultThemeEditorPreview, ThemeEditorPreview);

	ColorPickerButton *test_color_picker_button = nullptr;

protected:
	void _notification(int p_what);

public:
	DefaultThemeEditorPreview();
};

class SceneThemeEditorPreview : public ThemeEditorPreview {
	GDCLASS(SceneThemeEditorPreview, ThemeEditorPreview);

	Ref<PackedScene> loaded_scene;

	Button *reload_scene_button = nullptr;

	void _reload_scene();

protected:
	void _notification(int p_what);
	static void _bind_methods();

public:
	bool set_preview_scene(const String &p_path);
	String get_preview_scene_path() const;

	SceneThemeEditorPreview();
};

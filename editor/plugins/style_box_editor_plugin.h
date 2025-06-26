#pragma once

#include "editor/editor_inspector.h"
#include "editor/plugins/editor_plugin.h"
#include "scene/gui/texture_rect.h"

class Button;
class StyleBox;

class StyleBoxPreview : public TextureRect {
	GDCLASS(StyleBoxPreview, TextureRect);

	Button *grid_preview = nullptr;
	Ref<StyleBox> stylebox;

	void _sb_changed();
	void _redraw();
	static bool grid_preview_enabled;
	void _grid_preview_toggled(bool p_active);

protected:
	void _notification(int p_what);

public:
	void edit(const Ref<StyleBox> &p_stylebox);

	StyleBoxPreview();
};

class EditorInspectorPluginStyleBox : public EditorInspectorPlugin {
	GDCLASS(EditorInspectorPluginStyleBox, EditorInspectorPlugin);

public:
	virtual bool can_handle(Object *p_object) override;
	virtual void parse_begin(Object *p_object) override;
};

class StyleBoxEditorPlugin : public EditorPlugin {
	GDCLASS(StyleBoxEditorPlugin, EditorPlugin);

public:
	virtual String get_plugin_name() const override { return "StyleBox"; }

	StyleBoxEditorPlugin();
};

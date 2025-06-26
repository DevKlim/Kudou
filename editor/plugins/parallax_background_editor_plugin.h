#pragma once

#include "editor/plugins/editor_plugin.h"

class HBoxContainer;
class MenuButton;
class ParallaxBackground;

class ParallaxBackgroundEditorPlugin : public EditorPlugin {
	GDCLASS(ParallaxBackgroundEditorPlugin, EditorPlugin);

	enum {
		MENU_CONVERT_TO_PARALLAX_2D,
	};

	ParallaxBackground *parallax_background = nullptr;
	HBoxContainer *toolbar = nullptr;
	MenuButton *menu = nullptr;

	void _menu_callback(int p_idx);
	void convert_to_parallax2d();

protected:
	void _notification(int p_what);

public:
	virtual String get_plugin_name() const override { return "ParallaxBackground"; }
	bool has_main_screen() const override { return false; }
	virtual void edit(Object *p_object) override;
	virtual bool handles(Object *p_object) const override;
	virtual void make_visible(bool p_visible) override;

	ParallaxBackgroundEditorPlugin();
};

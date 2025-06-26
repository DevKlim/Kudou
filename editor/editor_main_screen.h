#pragma once

#include "scene/gui/panel_container.h"

class Button;
class ConfigFile;
class EditorPlugin;
class HBoxContainer;
class VBoxContainer;

class EditorMainScreen : public PanelContainer {
	GDCLASS(EditorMainScreen, PanelContainer);

public:
	enum EditorTable {
		EDITOR_2D = 0,
		EDITOR_3D,
		EDITOR_SCRIPT,
		EDITOR_GAME,
		EDITOR_ASSETLIB,
	};

private:
	VBoxContainer *main_screen_vbox = nullptr;
	EditorPlugin *selected_plugin = nullptr;

	HBoxContainer *button_hb = nullptr;
	Vector<Button *> buttons;
	Vector<EditorPlugin *> editor_table;
	HashMap<String, EditorPlugin *> main_editor_plugins;

	int _get_current_main_editor() const;

protected:
	void _notification(int p_what);

public:
	void set_button_container(HBoxContainer *p_button_hb);

	void save_layout_to_config(Ref<ConfigFile> p_config_file, const String &p_section) const;
	void load_layout_from_config(Ref<ConfigFile> p_config_file, const String &p_section);

	void set_button_enabled(int p_index, bool p_enabled);
	bool is_button_enabled(int p_index) const;

	void select_next();
	void select_prev();
	void select_by_name(const String &p_name);
	void select(int p_index);
	int get_selected_index() const;
	int get_plugin_index(EditorPlugin *p_editor) const;
	EditorPlugin *get_selected_plugin() const;
	EditorPlugin *get_plugin_by_name(const String &p_plugin_name) const;
	bool can_auto_switch_screens() const;

	VBoxContainer *get_control() const;

	void add_main_plugin(EditorPlugin *p_editor);
	void remove_main_plugin(EditorPlugin *p_editor);

	EditorMainScreen();
};

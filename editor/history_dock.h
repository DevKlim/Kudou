#pragma once

#include "scene/gui/box_container.h"

class CheckBox;
class ConfigFile;
class ItemList;
class EditorUndoRedoManager;

class HistoryDock : public VBoxContainer {
	GDCLASS(HistoryDock, VBoxContainer);

	EditorUndoRedoManager *ur_manager;
	ItemList *action_list = nullptr;

	CheckBox *current_scene_checkbox = nullptr;
	CheckBox *global_history_checkbox = nullptr;

	bool need_refresh = true;
	int current_version = 0;

	void on_history_changed();
	void refresh_history();
	void on_version_changed();
	void refresh_version();

	void _save_layout_to_config(Ref<ConfigFile> p_layout, const String &p_section) const;
	void _load_layout_from_config(Ref<ConfigFile> p_layout, const String &p_section);

protected:
	void _notification(int p_notification);
	static void _bind_methods();

public:
	void seek_history(int p_index);

	HistoryDock();
};

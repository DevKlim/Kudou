#pragma once

#include "groups_editor.h"

class ConfigFile;
class ConnectionsDock;

class NodeDock : public VBoxContainer {
	GDCLASS(NodeDock, VBoxContainer);

	Button *connections_button = nullptr;
	Button *groups_button = nullptr;

	ConnectionsDock *connections = nullptr;
	GroupsEditor *groups = nullptr;

	HBoxContainer *mode_hb = nullptr;

	Label *select_a_node = nullptr;

	void _save_layout_to_config(Ref<ConfigFile> p_layout, const String &p_section) const;
	void _load_layout_from_config(Ref<ConfigFile> p_layout, const String &p_section);

private:
	inline static NodeDock *singleton = nullptr;

public:
	static NodeDock *get_singleton() { return singleton; }

protected:
	void _notification(int p_what);
	static void _bind_methods();

public:
	void set_node(Node *p_node);

	void show_groups();
	void show_connections();

	void update_lists();

	NodeDock();
	~NodeDock();
};

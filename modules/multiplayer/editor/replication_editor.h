#pragma once

#include "../scene_replication_config.h"

#include "editor/plugins/editor_plugin.h"
#include "scene/gui/box_container.h"

class ConfirmationDialog;
class MultiplayerSynchronizer;
class AcceptDialog;
class LineEdit;
class Tree;
class TreeItem;
class PropertySelector;
class SceneTreeDialog;

class ReplicationEditor : public VBoxContainer {
	GDCLASS(ReplicationEditor, VBoxContainer);

private:
	MultiplayerSynchronizer *current = nullptr;

	ConfirmationDialog *delete_dialog = nullptr;
	Button *add_pick_button = nullptr;
	Button *add_from_path_button = nullptr;
	LineEdit *np_line_edit = nullptr;

	Label *drop_label = nullptr;

	Ref<SceneReplicationConfig> config;
	NodePath deleting;
	Tree *tree = nullptr;

	PropertySelector *prop_selector = nullptr;
	SceneTreeDialog *pick_node = nullptr;
	NodePath adding_node_path;

	Button *pin = nullptr;

	Ref<Texture2D> _get_class_icon(const Node *p_node);

	void _add_pressed();
	void _np_text_submitted(const String &p_newtext);
	void _tree_item_edited();
	void _tree_button_pressed(Object *p_item, int p_column, int p_id, MouseButton p_button);
	void _update_value(const NodePath &p_prop, int p_column, int p_checked);
	void _update_config();
	void _dialog_closed(bool p_confirmed);
	void _add_property(const NodePath &p_property, bool p_spawn, SceneReplicationConfig::ReplicationMode p_mode);

	void _pick_node_filter_text_changed(const String &p_newtext);
	void _pick_node_select_recursive(TreeItem *p_item, const String &p_filter, Vector<Node *> &p_select_candidates);
	void _pick_node_selected(NodePath p_path);

	void _pick_new_property();
	void _pick_node_property_selected(String p_name);

	bool _can_drop_data_fw(const Point2 &p_point, const Variant &p_data, Control *p_from) const;
	void _drop_data_fw(const Point2 &p_point, const Variant &p_data, Control *p_from);

	void _add_sync_property(String p_path);

protected:
	static void _bind_methods();

	void _notification(int p_what);

public:
	void edit(MultiplayerSynchronizer *p_object);
	MultiplayerSynchronizer *get_current() const { return current; }

	Button *get_pin() { return pin; }
	ReplicationEditor();
};

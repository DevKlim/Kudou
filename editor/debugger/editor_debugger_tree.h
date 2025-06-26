#pragma once

#include "scene/gui/tree.h"

class AcceptDialog;
class SceneDebuggerTree;
class EditorFileDialog;

class EditorDebuggerTree : public Tree {
	GDCLASS(EditorDebuggerTree, Tree);

private:
	struct ParentItem {
		TreeItem *tree_item;
		int child_count;
		bool matches_filter;

		ParentItem(TreeItem *p_tree_item = nullptr, int p_child_count = 0, bool p_matches_filter = false) {
			tree_item = p_tree_item;
			child_count = p_child_count;
			matches_filter = p_matches_filter;
		}
	};

	enum ItemMenu {
		ITEM_MENU_SAVE_REMOTE_NODE,
		ITEM_MENU_COPY_NODE_PATH,
		ITEM_MENU_EXPAND_COLLAPSE,
	};

	TypedArray<uint64_t> inspected_object_ids;
	int debugger_id = 0;
	bool updating_scene_tree = false;
	bool scrolling_to_item = false;
	bool notify_selection_queued = false;
	bool selection_surpassed_limit = false;
	bool selection_uncollapse_all = false;
	HashSet<ObjectID> unfold_cache;
	PopupMenu *item_menu = nullptr;
	EditorFileDialog *file_dialog = nullptr;
	AcceptDialog *accept = nullptr;
	String last_filter;

	void _scene_tree_folded(Object *p_obj);
	void _scene_tree_selection_changed(TreeItem *p_item, int p_column, bool p_selected);
	void _scene_tree_nothing_selected();
	void _notify_selection_changed();
	void _scene_tree_rmb_selected(const Vector2 &p_position, MouseButton p_button);
	void _item_menu_id_pressed(int p_option);
	void _file_selected(const String &p_file);

protected:
	static void _bind_methods();
	void _notification(int p_what);

public:
	enum Button {
		BUTTON_SUBSCENE = 0,
		BUTTON_VISIBILITY = 1,
	};

	virtual Variant get_drag_data(const Point2 &p_point) override;

	void update_icon_max_width();
	String get_selected_path();
	ObjectID get_selected_object();
	int get_current_debugger(); // Would love to have one tree for every debugger.
	inline TypedArray<uint64_t> get_selection() const { return inspected_object_ids.duplicate(); }
	void update_scene_tree(const SceneDebuggerTree *p_tree, int p_debugger);
	void select_nodes(const TypedArray<int64_t> &p_ids);
	void clear_selection();

	EditorDebuggerTree();
};

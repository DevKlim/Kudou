#ifndef KUDOU_TREE_H
#define KUDOU_TREE_H

#include "scene/gui/tree.h"
#include "scene/main/node.h"

class KudouTree : public Tree {
	GDCLASS(KudouTree, Tree);

private:
	void _on_item_edited();
	void _propagate_check_down(TreeItem *p_item, bool p_checked);
	void _update_parent_check_state(TreeItem *p_item);
	void _get_checked_items_recursive(TreeItem *p_item, PackedStringArray &r_items);
	void _update_scene_tree(Node *p_node = nullptr);
	void _add_nodes_recursively(Node *p_node, TreeItem *p_parent_item);
	void _clear_tree();

protected:
	void _notification(int p_what);
	static void _bind_methods();

public:
    void update_tree();
	void get_checked_items(PackedStringArray &r_items);
	KudouTree();
};

#endif // KUDOU_TREE_H
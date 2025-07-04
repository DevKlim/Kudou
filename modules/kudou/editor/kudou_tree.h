#ifndef KUDOU_TREE_H
#define KUDOU_TREE_H

#include "scene/gui/tree.h"

class KudouTree : public Tree {
	GDCLASS(KudouTree, Tree);

private:
	void _on_item_edited();
	void _propagate_check_down(TreeItem *p_item, bool p_checked);
	void _update_parent_check_state(TreeItem *p_item);
	void _get_checked_items_recursive(TreeItem *p_item, PackedStringArray &r_items);

protected:
	void _notification(int p_what);
	static void _bind_methods();

public:
	void get_checked_items(PackedStringArray &r_items);
	KudouTree();
};

#endif // KUDOU_TREE_H
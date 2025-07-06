#include "kudou_tree.h"

#include "core/string/string_name.h" // Required for SNAME
#include "scene/gui/tree.h" // Required for Tree and TreeItem
#include "editor/editor_node.h"
#include "editor/themes/editor_scale.h"
#include "scene/main/scene_tree.h"

void KudouTree::_bind_methods() {
	// No methods are exposed to scripting API yet.
}

void KudouTree::_notification(int p_what) {
	if (p_what == NOTIFICATION_READY) {
		// Connect signals when the node is ready in the scene tree.
		connect(SNAME("item_edited"), callable_mp(this, &KudouTree::_on_item_edited));
	}
}

KudouTree::KudouTree() {
	// Signal connection is now handled in _notification(NOTIFICATION_READY) for safety.
}

void KudouTree::_on_item_edited() {
	TreeItem *item = get_edited();
	if (!item) {
		return;
	}

	int col = get_edited_column();
	// We only care about checkbox clicks in the first column.
	if (col != 0 || item->get_cell_mode(col) != TreeItem::CELL_MODE_CHECK) {
		return;
	}

	// When an item is checked/unchecked by the user, it can't be indeterminate.
	item->set_indeterminate(col, false);

	bool checked = item->is_checked(col);

	// Propagate to children.
	_propagate_check_down(item, checked);

	// Propagate to parents.
	_update_parent_check_state(item);
}

void KudouTree::_propagate_check_down(TreeItem *p_item, bool p_checked) {
	if (!p_item) {
		return;
	}
	for (TreeItem *child = p_item->get_first_child(); child; child = child->get_next()) {
		if (child->get_cell_mode(0) == TreeItem::CELL_MODE_CHECK) {
			child->set_checked(0, p_checked);
			// When a child's state is explicitly set, it should no longer be indeterminate.
			child->set_indeterminate(0, false);
			_propagate_check_down(child, p_checked);
		}
	}
}

void KudouTree::_update_parent_check_state(TreeItem *p_item) {
	if (!p_item || !p_item->get_parent()) {
		return;
	}

	TreeItem *parent = p_item->get_parent();
	if (parent->get_cell_mode(0) != TreeItem::CELL_MODE_CHECK) {
		return;
	}

	bool all_checked = true;
	bool any_checked = false;
	bool any_indeterminate = false;

	for (TreeItem *child = parent->get_first_child(); child; child = child->get_next()) {
		if (child->get_cell_mode(0) == TreeItem::CELL_MODE_CHECK) {
			if (child->is_indeterminate(0)) {
				any_indeterminate = true;
				break; // If any child is indeterminate, the parent must be indeterminate.
			}
			if (child->is_checked(0)) {
				any_checked = true;
			} else {
				all_checked = false;
			}
		}
	}

	bool state_changed = false;
	if (any_indeterminate || (any_checked && !all_checked)) {
		// Parent should be indeterminate if any child is indeterminate, or if some but not all children are checked.
		if (!parent->is_indeterminate(0)) {
			parent->set_indeterminate(0, true);
			state_changed = true;
		}
	} else {
		// Parent should be fully checked if all children are checked, or unchecked if no children are checked.
		if (parent->is_indeterminate(0) || parent->is_checked(0) != all_checked) {
			parent->set_checked(0, all_checked); // This also sets indeterminate to false.
			state_changed = true;
		}
	}

	if (state_changed) {
		// Recursively update the parent's parent.
		_update_parent_check_state(parent);
	}
}

void KudouTree::get_checked_items(PackedStringArray &r_items) {
	_get_checked_items_recursive(get_root(), r_items);
}

void KudouTree::_get_checked_items_recursive(TreeItem *p_item, PackedStringArray &r_items) {
	if (!p_item) {
		return;
	}

	// If an item is checked, we add it and don't look at its children,
	// as the parent's context is assumed to include all children.
	if (p_item->is_checked(0)) {
		String path = p_item->get_metadata(0);
		if (!path.is_empty()) {
			r_items.push_back(path);
		}
		return; // Stop recursion here.
	}

	// If an item is not checked (could be indeterminate), we need to check its children.
	for (TreeItem *child = p_item->get_first_child(); child; child = child->get_next()) {
		_get_checked_items_recursive(child, r_items);
	}
}

void KudouTree::_clear_tree() {
    clear();
    if (get_root()) {
        get_root()->clear_children();
    }
}

void KudouTree::_update_scene_tree(Node *p_node) {
    update_tree();
}

void KudouTree::update_tree() {
    _clear_tree();
    SceneTree *st = get_tree();
    if (st) {
        Node *scene_root = st->get_edited_scene_root();
        if (scene_root) {
            _add_nodes_recursively(scene_root, nullptr);
        }
    }
}

void KudouTree::_add_nodes_recursively(Node *p_node, TreeItem *p_parent_item) {
    if (!p_node) {
        return;
    }

    TreeItem *item = create_item(p_parent_item);
    item->set_text(0, p_node->get_name());
    item->set_icon(0, EditorNode::get_singleton()->get_object_icon(p_node, "Node"));
    item->set_metadata(0, p_node->get_path());
    item->set_cell_mode(0, TreeItem::CELL_MODE_CHECK);


    for (int i = 0; i < p_node->get_child_count(); i++) {
        _add_nodes_recursively(p_node->get_child(i), item);
    }
}
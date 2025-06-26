#pragma once

#include "scene/gui/control.h"

class Container : public Control {
	GDCLASS(Container, Control);

	bool pending_sort = false;
	void _sort_children();
	void _child_minsize_changed();

protected:
	enum class SortableVisibilityMode {
		VISIBLE,
		VISIBLE_IN_TREE,
		IGNORE,
	};

	void queue_sort();
	Control *as_sortable_control(Node *p_node, SortableVisibilityMode p_visibility_mode = SortableVisibilityMode::VISIBLE_IN_TREE) const;

	virtual void add_child_notify(Node *p_child) override;
	virtual void move_child_notify(Node *p_child) override;
	virtual void remove_child_notify(Node *p_child) override;

	GDVIRTUAL0RC(Vector<int>, _get_allowed_size_flags_horizontal)
	GDVIRTUAL0RC(Vector<int>, _get_allowed_size_flags_vertical)

	void _notification(int p_what);
	static void _bind_methods();

public:
	enum {
		NOTIFICATION_PRE_SORT_CHILDREN = 50,
		NOTIFICATION_SORT_CHILDREN = 51,
	};

	void fit_child_in_rect(Control *p_child, const Rect2 &p_rect);

	virtual Vector<int> get_allowed_size_flags_horizontal() const;
	virtual Vector<int> get_allowed_size_flags_vertical() const;

	PackedStringArray get_configuration_warnings() const override;

	Container();
};

#pragma once

#include "core/object/ref_counted.h"

class MultiNodeEdit : public RefCounted {
	GDCLASS(MultiNodeEdit, RefCounted);

	LocalVector<NodePath> nodes;
	bool notify_property_list_changed_pending = false;
	struct PLData {
		int uses = 0;
		PropertyInfo info;
	};

	bool _set_impl(const StringName &p_name, const Variant &p_value, const String &p_field);
	void _queue_notify_property_list_changed();
	void _notify_property_list_changed();

protected:
	static void _bind_methods();

	bool _set(const StringName &p_name, const Variant &p_value);
	bool _get(const StringName &p_name, Variant &r_ret) const;
	void _get_property_list(List<PropertyInfo> *p_list) const;

public:
	bool _hide_script_from_inspector() { return true; }
	bool _hide_metadata_from_inspector() { return true; }

	bool _property_can_revert(const StringName &p_name) const;
	bool _property_get_revert(const StringName &p_name, Variant &r_property) const;
	String _get_editor_name() const;

	void add_node(const NodePath &p_node);

	int get_node_count() const;
	NodePath get_node(int p_index) const;
	StringName get_edited_class_name() const;

	void set_property_field(const StringName &p_property, const Variant &p_value, const String &p_field);

	// If the nodes selected are the same independently of order then return true.
	bool is_same_selection(const MultiNodeEdit *p_other) const {
		if (get_node_count() != p_other->get_node_count()) {
			return false;
		}
		for (int i = 0; i < get_node_count(); i++) {
			if (!nodes.has(p_other->get_node(i))) {
				return false;
			}
		}

		return true;
	}
};

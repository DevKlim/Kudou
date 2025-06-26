#pragma once

#include "scene/3d/node_3d.h"
#include "scene/resources/3d/shape_3d.h"

class CollisionObject3D;
class CollisionShape3D : public Node3D {
	GDCLASS(CollisionShape3D, Node3D);

	Ref<Shape3D> shape;

	uint32_t owner_id = 0;
	CollisionObject3D *collision_object = nullptr;

	Color debug_color;
	bool debug_fill = true;

	Color _get_default_debug_color() const;

#ifdef DEBUG_ENABLED
	void _shape_changed();
#endif // DEBUG_ENABLED

#ifndef DISABLE_DEPRECATED
	void resource_changed(Ref<Resource> res);
#endif
	bool disabled = false;

protected:
	void _update_in_shape_owner(bool p_xform_only = false);

protected:
	void _notification(int p_what);
	static void _bind_methods();

#ifdef DEBUG_ENABLED
	bool _property_can_revert(const StringName &p_name) const;
	bool _property_get_revert(const StringName &p_name, Variant &r_property) const;
	void _validate_property(PropertyInfo &p_property) const;
#endif // DEBUG_ENABLED

public:
	void make_convex_from_siblings();

	void set_shape(const Ref<Shape3D> &p_shape);
	Ref<Shape3D> get_shape() const;

	void set_disabled(bool p_disabled);
	bool is_disabled() const;

	void set_debug_color(const Color &p_color);
	Color get_debug_color() const;

	void set_debug_fill_enabled(bool p_enable);
	bool get_debug_fill_enabled() const;

	PackedStringArray get_configuration_warnings() const override;

	CollisionShape3D();
	~CollisionShape3D();
};

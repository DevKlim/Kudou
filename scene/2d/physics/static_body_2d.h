#pragma once

#include "scene/2d/physics/physics_body_2d.h"

class NavigationPolygon;
class NavigationMeshSourceGeometryData2D;

class StaticBody2D : public PhysicsBody2D {
	GDCLASS(StaticBody2D, PhysicsBody2D);

private:
	Vector2 constant_linear_velocity;
	real_t constant_angular_velocity = 0.0;

	Ref<PhysicsMaterial> physics_material_override;

protected:
	static void _bind_methods();

public:
	void set_physics_material_override(const Ref<PhysicsMaterial> &p_physics_material_override);
	Ref<PhysicsMaterial> get_physics_material_override() const;

	void set_constant_linear_velocity(const Vector2 &p_vel);
	void set_constant_angular_velocity(real_t p_vel);

	Vector2 get_constant_linear_velocity() const;
	real_t get_constant_angular_velocity() const;

	StaticBody2D(PhysicsServer2D::BodyMode p_mode = PhysicsServer2D::BODY_MODE_STATIC);

private:
	static Callable _navmesh_source_geometry_parsing_callback;
	static RID _navmesh_source_geometry_parser;

#ifndef NAVIGATION_2D_DISABLED
public:
	static void navmesh_parse_init();
	static void navmesh_parse_source_geometry(const Ref<NavigationPolygon> &p_navigation_mesh, Ref<NavigationMeshSourceGeometryData2D> p_source_geometry_data, Node *p_node);
#endif // NAVIGATION_2D_DISABLED

private:
	void _reload_physics_characteristics();
};

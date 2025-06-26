#pragma once

#include "scene/3d/physics/physics_body_3d.h"

#ifndef NAVIGATION_3D_DISABLED
class NavigationMesh;
class NavigationMeshSourceGeometryData3D;
#endif // NAVIGATION_3D_DISABLED

class StaticBody3D : public PhysicsBody3D {
	GDCLASS(StaticBody3D, PhysicsBody3D);

private:
	Vector3 constant_linear_velocity;
	Vector3 constant_angular_velocity;

	Ref<PhysicsMaterial> physics_material_override;

protected:
	static void _bind_methods();

public:
	void set_physics_material_override(const Ref<PhysicsMaterial> &p_physics_material_override);
	Ref<PhysicsMaterial> get_physics_material_override() const;

	void set_constant_linear_velocity(const Vector3 &p_vel);
	void set_constant_angular_velocity(const Vector3 &p_vel);

	Vector3 get_constant_linear_velocity() const;
	Vector3 get_constant_angular_velocity() const;

	StaticBody3D(PhysicsServer3D::BodyMode p_mode = PhysicsServer3D::BODY_MODE_STATIC);

private:
	void _reload_physics_characteristics();

#ifndef NAVIGATION_3D_DISABLED
	static Callable _navmesh_source_geometry_parsing_callback;
	static RID _navmesh_source_geometry_parser;

public:
	static void navmesh_parse_init();
	static void navmesh_parse_source_geometry(const Ref<NavigationMesh> &p_navigation_mesh, Ref<NavigationMeshSourceGeometryData3D> p_source_geometry_data, Node *p_node);
#endif // NAVIGATION_3D_DISABLED
};

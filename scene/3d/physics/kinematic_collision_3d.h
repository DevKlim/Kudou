#pragma once

#include "core/object/ref_counted.h"
#include "servers/physics_server_3d.h"

class KinematicCollision3D : public RefCounted {
	GDCLASS(KinematicCollision3D, RefCounted);

	ObjectID owner_id;
	friend class PhysicsBody3D;
	friend class CharacterBody3D;
	PhysicsServer3D::MotionResult result;

protected:
	static void _bind_methods();

public:
	Vector3 get_travel() const;
	Vector3 get_remainder() const;
	int get_collision_count() const;
	real_t get_depth() const;
	Vector3 get_position(int p_collision_index = 0) const;
	Vector3 get_normal(int p_collision_index = 0) const;
	real_t get_angle(int p_collision_index = 0, const Vector3 &p_up_direction = Vector3(0.0, 1.0, 0.0)) const;
	Object *get_local_shape(int p_collision_index = 0) const;
	Object *get_collider(int p_collision_index = 0) const;
	ObjectID get_collider_id(int p_collision_index = 0) const;
	RID get_collider_rid(int p_collision_index = 0) const;
	Object *get_collider_shape(int p_collision_index = 0) const;
	int get_collider_shape_index(int p_collision_index = 0) const;
	Vector3 get_collider_velocity(int p_collision_index = 0) const;
};

#pragma once

#include "scene/2d/physics/collision_object_2d.h"
#include "scene/2d/physics/kinematic_collision_2d.h"
#include "scene/resources/physics_material.h"
#include "servers/physics_server_2d.h"

class PhysicsBody2D : public CollisionObject2D {
	GDCLASS(PhysicsBody2D, CollisionObject2D);

protected:
	static void _bind_methods();
	PhysicsBody2D(PhysicsServer2D::BodyMode p_mode);

	Ref<KinematicCollision2D> motion_cache;

	Ref<KinematicCollision2D> _move(const Vector2 &p_motion, bool p_test_only = false, real_t p_margin = 0.08, bool p_recovery_as_collision = false);

public:
	PackedStringArray get_configuration_warnings() const override;

	bool move_and_collide(const PhysicsServer2D::MotionParameters &p_parameters, PhysicsServer2D::MotionResult &r_result, bool p_test_only = false, bool p_cancel_sliding = true);
	bool test_move(const Transform2D &p_from, const Vector2 &p_motion, const Ref<KinematicCollision2D> &r_collision = Ref<KinematicCollision2D>(), real_t p_margin = 0.08, bool p_recovery_as_collision = false);
	Vector2 get_gravity() const;

	TypedArray<PhysicsBody2D> get_collision_exceptions();
	void add_collision_exception_with(Node *p_node); //must be physicsbody
	void remove_collision_exception_with(Node *p_node);
};

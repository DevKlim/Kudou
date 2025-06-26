#pragma once

#include "scene/3d/physics/joints/joint_3d.h"

class HingeJoint3D : public Joint3D {
	GDCLASS(HingeJoint3D, Joint3D);

public:
	enum Param {
		PARAM_BIAS = PhysicsServer3D::HINGE_JOINT_BIAS,
		PARAM_LIMIT_UPPER = PhysicsServer3D::HINGE_JOINT_LIMIT_UPPER,
		PARAM_LIMIT_LOWER = PhysicsServer3D::HINGE_JOINT_LIMIT_LOWER,
		PARAM_LIMIT_BIAS = PhysicsServer3D::HINGE_JOINT_LIMIT_BIAS,
		PARAM_LIMIT_SOFTNESS = PhysicsServer3D::HINGE_JOINT_LIMIT_SOFTNESS,
		PARAM_LIMIT_RELAXATION = PhysicsServer3D::HINGE_JOINT_LIMIT_RELAXATION,
		PARAM_MOTOR_TARGET_VELOCITY = PhysicsServer3D::HINGE_JOINT_MOTOR_TARGET_VELOCITY,
		PARAM_MOTOR_MAX_IMPULSE = PhysicsServer3D::HINGE_JOINT_MOTOR_MAX_IMPULSE,
		PARAM_MAX = PhysicsServer3D::HINGE_JOINT_MAX
	};

	enum Flag {
		FLAG_USE_LIMIT = PhysicsServer3D::HINGE_JOINT_FLAG_USE_LIMIT,
		FLAG_ENABLE_MOTOR = PhysicsServer3D::HINGE_JOINT_FLAG_ENABLE_MOTOR,
		FLAG_MAX = PhysicsServer3D::HINGE_JOINT_FLAG_MAX
	};

protected:
	real_t params[PARAM_MAX];
	bool flags[FLAG_MAX];
	virtual void _configure_joint(RID p_joint, PhysicsBody3D *body_a, PhysicsBody3D *body_b) override;
	static void _bind_methods();

public:
	void set_param(Param p_param, real_t p_value);
	real_t get_param(Param p_param) const;

	void set_flag(Flag p_flag, bool p_value);
	bool get_flag(Flag p_flag) const;

	HingeJoint3D();
};

VARIANT_ENUM_CAST(HingeJoint3D::Param);
VARIANT_ENUM_CAST(HingeJoint3D::Flag);

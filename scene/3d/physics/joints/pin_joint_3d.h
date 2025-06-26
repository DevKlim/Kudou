#pragma once

#include "scene/3d/physics/joints/joint_3d.h"

class PinJoint3D : public Joint3D {
	GDCLASS(PinJoint3D, Joint3D);

public:
	enum Param {
		PARAM_BIAS = PhysicsServer3D::PIN_JOINT_BIAS,
		PARAM_DAMPING = PhysicsServer3D::PIN_JOINT_DAMPING,
		PARAM_IMPULSE_CLAMP = PhysicsServer3D::PIN_JOINT_IMPULSE_CLAMP
	};

protected:
	real_t params[3];
	virtual void _configure_joint(RID p_joint, PhysicsBody3D *body_a, PhysicsBody3D *body_b) override;
	static void _bind_methods();

public:
	void set_param(Param p_param, real_t p_value);
	real_t get_param(Param p_param) const;

	PinJoint3D();
};

VARIANT_ENUM_CAST(PinJoint3D::Param);

#pragma once

#include "scene/2d/physics/joints/joint_2d.h"

class PhysicsBody2D;

class GrooveJoint2D : public Joint2D {
	GDCLASS(GrooveJoint2D, Joint2D);

	real_t length = 50.0;
	real_t initial_offset = 25.0;

protected:
	void _notification(int p_what);
	virtual void _configure_joint(RID p_joint, PhysicsBody2D *body_a, PhysicsBody2D *body_b) override;
	static void _bind_methods();

public:
	void set_length(real_t p_length);
	real_t get_length() const;

	void set_initial_offset(real_t p_initial_offset);
	real_t get_initial_offset() const;

	GrooveJoint2D();
};

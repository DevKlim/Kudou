#pragma once

#include "scene/2d/physics/static_body_2d.h"

class AnimatableBody2D : public StaticBody2D {
	GDCLASS(AnimatableBody2D, StaticBody2D);

private:
	bool sync_to_physics = true;

	Transform2D last_valid_transform;

	static void _body_state_changed_callback(void *p_instance, PhysicsDirectBodyState2D *p_state);
	void _body_state_changed(PhysicsDirectBodyState2D *p_state);

protected:
	void _notification(int p_what);
	static void _bind_methods();

public:
	AnimatableBody2D();

private:
	void _update_kinematic_motion();

	void set_sync_to_physics(bool p_enable);
	bool is_sync_to_physics_enabled() const;
};

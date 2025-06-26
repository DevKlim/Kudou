#include "animatable_body_2d.h"

void AnimatableBody2D::set_sync_to_physics(bool p_enable) {
	if (sync_to_physics == p_enable) {
		return;
	}

	sync_to_physics = p_enable;

	_update_kinematic_motion();
}

bool AnimatableBody2D::is_sync_to_physics_enabled() const {
	return sync_to_physics;
}

void AnimatableBody2D::_update_kinematic_motion() {
#ifdef TOOLS_ENABLED
	if (Engine::get_singleton()->is_editor_hint()) {
		return;
	}
#endif

	if (sync_to_physics) {
		PhysicsServer2D::get_singleton()->body_set_state_sync_callback(get_rid(), callable_mp(this, &AnimatableBody2D::_body_state_changed));
		set_only_update_transform_changes(true);
		set_notify_local_transform(true);
	} else {
		PhysicsServer2D::get_singleton()->body_set_state_sync_callback(get_rid(), Callable());
		set_only_update_transform_changes(false);
		set_notify_local_transform(false);
	}
}

void AnimatableBody2D::_body_state_changed(PhysicsDirectBodyState2D *p_state) {
	if (!sync_to_physics) {
		return;
	}

	last_valid_transform = p_state->get_transform();
	set_notify_local_transform(false);
	set_global_transform(last_valid_transform);
	set_notify_local_transform(true);
}

void AnimatableBody2D::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_ENTER_TREE: {
			last_valid_transform = get_global_transform();
			_update_kinematic_motion();
		} break;

		case NOTIFICATION_EXIT_TREE: {
			set_only_update_transform_changes(false);
			set_notify_local_transform(false);
		} break;

		case NOTIFICATION_LOCAL_TRANSFORM_CHANGED: {
			// Used by sync to physics, send the new transform to the physics...
			Transform2D new_transform = get_global_transform();

			PhysicsServer2D::get_singleton()->body_set_state(get_rid(), PhysicsServer2D::BODY_STATE_TRANSFORM, new_transform);

			// ... but then revert changes.
			set_notify_local_transform(false);
			set_global_transform(last_valid_transform);
			set_notify_local_transform(true);
		} break;
	}
}

void AnimatableBody2D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_sync_to_physics", "enable"), &AnimatableBody2D::set_sync_to_physics);
	ClassDB::bind_method(D_METHOD("is_sync_to_physics_enabled"), &AnimatableBody2D::is_sync_to_physics_enabled);

	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "sync_to_physics"), "set_sync_to_physics", "is_sync_to_physics_enabled");
}

AnimatableBody2D::AnimatableBody2D() :
		StaticBody2D(PhysicsServer2D::BODY_MODE_KINEMATIC) {
}

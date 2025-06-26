#include "register_types.h"

#include "core/config/project_settings.h"
#include "godot_physics_server_2d.h"
#include "servers/physics_server_2d.h"
#include "servers/physics_server_2d_wrap_mt.h"

static PhysicsServer2D *_createGodotPhysics2DCallback() {
#ifdef THREADS_ENABLED
	bool using_threads = GLOBAL_GET("physics/2d/run_on_separate_thread");
#else
	bool using_threads = false;
#endif

	PhysicsServer2D *physics_server_2d = memnew(GodotPhysicsServer2D(using_threads));

	return memnew(PhysicsServer2DWrapMT(physics_server_2d, using_threads));
}

void initialize_godot_physics_2d_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SERVERS) {
		return;
	}
	PhysicsServer2DManager::get_singleton()->register_server("GodotPhysics2D", callable_mp_static(_createGodotPhysics2DCallback));
	PhysicsServer2DManager::get_singleton()->set_default_server("GodotPhysics2D");
}

void uninitialize_godot_physics_2d_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SERVERS) {
		return;
	}
}

#include "register_types.h"

#include "godot_physics_server_3d.h"
#include "servers/physics_server_3d.h"
#include "servers/physics_server_3d_wrap_mt.h"

static PhysicsServer3D *_createGodotPhysics3DCallback() {
#ifdef THREADS_ENABLED
	bool using_threads = GLOBAL_GET("physics/3d/run_on_separate_thread");
#else
	bool using_threads = false;
#endif

	PhysicsServer3D *physics_server_3d = memnew(GodotPhysicsServer3D(using_threads));

	return memnew(PhysicsServer3DWrapMT(physics_server_3d, using_threads));
}

void initialize_godot_physics_3d_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SERVERS) {
		return;
	}
	PhysicsServer3DManager::get_singleton()->register_server("GodotPhysics3D", callable_mp_static(_createGodotPhysics3DCallback));
	PhysicsServer3DManager::get_singleton()->set_default_server("GodotPhysics3D");
}

void uninitialize_godot_physics_3d_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SERVERS) {
		return;
	}
}

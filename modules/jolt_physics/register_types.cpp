#include "register_types.h"

#include "jolt_globals.h"
#include "jolt_physics_server_3d.h"
#include "jolt_project_settings.h"

#include "servers/physics_server_3d_wrap_mt.h"

PhysicsServer3D *create_jolt_physics_server() {
#ifdef THREADS_ENABLED
	bool run_on_separate_thread = GLOBAL_GET("physics/3d/run_on_separate_thread");
#else
	bool run_on_separate_thread = false;
#endif

	JoltPhysicsServer3D *physics_server = memnew(JoltPhysicsServer3D(run_on_separate_thread));

	return memnew(PhysicsServer3DWrapMT(physics_server, run_on_separate_thread));
}

void initialize_jolt_physics_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SERVERS) {
		return;
	}

	jolt_initialize();
	PhysicsServer3DManager::get_singleton()->register_server("Jolt Physics", callable_mp_static(&create_jolt_physics_server));
	JoltProjectSettings::register_settings();
}

void uninitialize_jolt_physics_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SERVERS) {
		return;
	}

	jolt_deinitialize();
}

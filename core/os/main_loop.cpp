#include "main_loop.h"

void MainLoop::_bind_methods() {
	BIND_CONSTANT(NOTIFICATION_OS_MEMORY_WARNING);
	BIND_CONSTANT(NOTIFICATION_TRANSLATION_CHANGED);
	BIND_CONSTANT(NOTIFICATION_WM_ABOUT);
	BIND_CONSTANT(NOTIFICATION_CRASH);
	BIND_CONSTANT(NOTIFICATION_OS_IME_UPDATE);
	BIND_CONSTANT(NOTIFICATION_APPLICATION_RESUMED);
	BIND_CONSTANT(NOTIFICATION_APPLICATION_PAUSED);
	BIND_CONSTANT(NOTIFICATION_APPLICATION_FOCUS_IN);
	BIND_CONSTANT(NOTIFICATION_APPLICATION_FOCUS_OUT);
	BIND_CONSTANT(NOTIFICATION_TEXT_SERVER_CHANGED);

	ADD_SIGNAL(MethodInfo("on_request_permissions_result", PropertyInfo(Variant::STRING, "permission"), PropertyInfo(Variant::BOOL, "granted")));

	GDVIRTUAL_BIND(_initialize);
	GDVIRTUAL_BIND(_physics_process, "delta");
	GDVIRTUAL_BIND(_process, "delta");
	GDVIRTUAL_BIND(_finalize);
}

void MainLoop::initialize() {
	GDVIRTUAL_CALL(_initialize);
}

bool MainLoop::physics_process(double p_time) {
	bool quit = false;
	GDVIRTUAL_CALL(_physics_process, p_time, quit);
	return quit;
}

bool MainLoop::process(double p_time) {
	bool quit = false;
	GDVIRTUAL_CALL(_process, p_time, quit);
	return quit;
}

void MainLoop::finalize() {
	GDVIRTUAL_CALL(_finalize);

	if (get_script_instance()) {
		set_script(Variant()); //clear script
	}
}

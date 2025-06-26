#include "register_types.h"

#include "mobile_vr_interface.h"

Ref<MobileVRInterface> mobile_vr;

void initialize_mobile_vr_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	GDREGISTER_CLASS(MobileVRInterface);

	if (XRServer::get_singleton()) {
		mobile_vr.instantiate();
		XRServer::get_singleton()->add_interface(mobile_vr);
	}
}

void uninitialize_mobile_vr_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	if (mobile_vr.is_valid()) {
		// uninitialize our interface if it is initialized
		if (mobile_vr->is_initialized()) {
			mobile_vr->uninitialize();
		}

		// unregister our interface from the XR server
		if (XRServer::get_singleton()) {
			XRServer::get_singleton()->remove_interface(mobile_vr);
		}

		// and release
		mobile_vr.unref();
	}
}

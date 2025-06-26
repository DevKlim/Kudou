#include "openxr_local_floor_extension.h"

OpenXRLocalFloorExtension *OpenXRLocalFloorExtension::singleton = nullptr;

OpenXRLocalFloorExtension *OpenXRLocalFloorExtension::get_singleton() {
	return singleton;
}

OpenXRLocalFloorExtension::OpenXRLocalFloorExtension() {
	singleton = this;
}

OpenXRLocalFloorExtension::~OpenXRLocalFloorExtension() {
	singleton = nullptr;
}

HashMap<String, bool *> OpenXRLocalFloorExtension::get_requested_extensions() {
	HashMap<String, bool *> request_extensions;

	request_extensions[XR_EXT_LOCAL_FLOOR_EXTENSION_NAME] = &available;

	return request_extensions;
}

bool OpenXRLocalFloorExtension::is_available() {
	return available;
}

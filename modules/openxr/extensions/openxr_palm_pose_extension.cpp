#include "openxr_palm_pose_extension.h"

OpenXRPalmPoseExtension *OpenXRPalmPoseExtension::singleton = nullptr;

OpenXRPalmPoseExtension *OpenXRPalmPoseExtension::get_singleton() {
	return singleton;
}

OpenXRPalmPoseExtension::OpenXRPalmPoseExtension() {
	singleton = this;
}

OpenXRPalmPoseExtension::~OpenXRPalmPoseExtension() {
	singleton = nullptr;
}

HashMap<String, bool *> OpenXRPalmPoseExtension::get_requested_extensions() {
	HashMap<String, bool *> request_extensions;

	request_extensions[XR_EXT_PALM_POSE_EXTENSION_NAME] = &available;

	return request_extensions;
}

bool OpenXRPalmPoseExtension::is_available() {
	return available;
}

#pragma once

#include "openxr_extension_wrapper.h"

class OpenXRPalmPoseExtension : public OpenXRExtensionWrapper {
	GDCLASS(OpenXRPalmPoseExtension, OpenXRExtensionWrapper);

protected:
	static void _bind_methods() {}

public:
	static OpenXRPalmPoseExtension *get_singleton();

	OpenXRPalmPoseExtension();
	virtual ~OpenXRPalmPoseExtension() override;

	virtual HashMap<String, bool *> get_requested_extensions() override;

	bool is_available();

private:
	static OpenXRPalmPoseExtension *singleton;

	bool available = false;
};

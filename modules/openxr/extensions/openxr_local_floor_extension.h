#pragma once

#include "openxr_extension_wrapper.h"

class OpenXRLocalFloorExtension : public OpenXRExtensionWrapper {
	GDCLASS(OpenXRLocalFloorExtension, OpenXRExtensionWrapper);

protected:
	static void _bind_methods() {}

public:
	static OpenXRLocalFloorExtension *get_singleton();

	OpenXRLocalFloorExtension();
	virtual ~OpenXRLocalFloorExtension() override;

	virtual HashMap<String, bool *> get_requested_extensions() override;

	bool is_available();

private:
	static OpenXRLocalFloorExtension *singleton;

	bool available = false;
};

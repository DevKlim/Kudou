#pragma once

#include "openxr_extension_wrapper.h"

class OpenXRML2ControllerExtension : public OpenXRExtensionWrapper {
	GDCLASS(OpenXRML2ControllerExtension, OpenXRExtensionWrapper);

protected:
	static void _bind_methods() {}

public:
	virtual HashMap<String, bool *> get_requested_extensions() override;

	bool is_available();

	virtual void on_register_metadata() override;

private:
	bool available = false;
};

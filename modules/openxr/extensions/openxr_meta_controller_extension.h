#pragma once

#include "openxr_extension_wrapper.h"

class OpenXRMetaControllerExtension : public OpenXRExtensionWrapper {
	GDCLASS(OpenXRMetaControllerExtension, OpenXRExtensionWrapper);

protected:
	static void _bind_methods() {}

public:
	enum MetaControllers {
		META_TOUCH_PROXIMITY, // Proximity extensions for normal touch controllers
		META_TOUCH_PRO, // Touch controller for the Quest Pro
		META_TOUCH_PLUS, // Touch controller for the Quest Plus
		META_MAX_CONTROLLERS
	};

	virtual HashMap<String, bool *> get_requested_extensions() override;

	bool is_available(MetaControllers p_type);

	virtual void on_register_metadata() override;

private:
	bool available[META_MAX_CONTROLLERS] = { false, false, false };
};

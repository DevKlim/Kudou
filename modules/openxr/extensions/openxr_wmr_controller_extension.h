#pragma once

#include "openxr_extension_wrapper.h"

class OpenXRWMRControllerExtension : public OpenXRExtensionWrapper {
	GDCLASS(OpenXRWMRControllerExtension, OpenXRExtensionWrapper);

protected:
	static void _bind_methods() {}

public:
	enum WMRControllers {
		WMR_HPMR,
		WMR_SAMSUNG_ODESSY,
		WMR_HAND_INTERACTION,
		WMR_MAX_CONTROLLERS
	};

	virtual HashMap<String, bool *> get_requested_extensions() override;

	bool is_available(WMRControllers p_type);

	virtual void on_register_metadata() override;

private:
	bool available[WMR_MAX_CONTROLLERS] = { false, false };
};

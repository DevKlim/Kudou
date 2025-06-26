#pragma once

#include "openxr_extension_wrapper.h"

class OpenXRHTCViveTrackerExtension : public OpenXRExtensionWrapper {
	GDCLASS(OpenXRHTCViveTrackerExtension, OpenXRExtensionWrapper);

protected:
	static void _bind_methods() {}

public:
	virtual HashMap<String, bool *> get_requested_extensions() override;

	PackedStringArray get_suggested_tracker_names() override;

	bool is_available();

	virtual void on_register_metadata() override;
	virtual bool on_event_polled(const XrEventDataBuffer &event) override;

private:
	bool available = false;
};

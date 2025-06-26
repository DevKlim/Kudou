#pragma once

#include "openxr_extension_wrapper.h"

class OpenXREyeGazeInteractionExtension : public OpenXRExtensionWrapper {
	GDCLASS(OpenXREyeGazeInteractionExtension, OpenXRExtensionWrapper);

protected:
	static void _bind_methods() {}

public:
	static OpenXREyeGazeInteractionExtension *get_singleton();

	OpenXREyeGazeInteractionExtension();
	~OpenXREyeGazeInteractionExtension();

	virtual HashMap<String, bool *> get_requested_extensions() override;
	virtual void *set_system_properties_and_get_next_pointer(void *p_next_pointer) override;

	PackedStringArray get_suggested_tracker_names() override;

	bool is_available();
	bool supports_eye_gaze_interaction();

	virtual void on_register_metadata() override;

	bool get_eye_gaze_pose(double p_dist, Vector3 &r_eye_pose);

private:
	static OpenXREyeGazeInteractionExtension *singleton;

	bool available = false;
	XrSystemEyeGazeInteractionPropertiesEXT properties;

	bool init_eye_gaze_pose = false;
	RID eye_tracker;
	RID eye_action;
};

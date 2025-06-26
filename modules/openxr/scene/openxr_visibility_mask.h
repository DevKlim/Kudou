#pragma once

#include "scene/3d/visual_instance_3d.h"

class OpenXRVisibilityMask : public VisualInstance3D {
	GDCLASS(OpenXRVisibilityMask, VisualInstance3D);

protected:
	static void _bind_methods();

	void _notification(int p_what);

	void _on_openxr_session_begun();
	void _on_openxr_session_stopping();

public:
	virtual PackedStringArray get_configuration_warnings() const override;

	virtual AABB get_aabb() const override;

	OpenXRVisibilityMask();
	~OpenXRVisibilityMask();
};

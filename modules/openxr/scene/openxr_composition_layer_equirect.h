#pragma once

#include <openxr/openxr.h>

#include "openxr_composition_layer.h"

class OpenXRCompositionLayerEquirect : public OpenXRCompositionLayer {
	GDCLASS(OpenXRCompositionLayerEquirect, OpenXRCompositionLayer);

	XrCompositionLayerEquirect2KHR composition_layer = {
		XR_TYPE_COMPOSITION_LAYER_EQUIRECT2_KHR, // type
		nullptr, // next
		0, // layerFlags
		XR_NULL_HANDLE, // space
		XR_EYE_VISIBILITY_BOTH, // eyeVisibility
		{}, // subImage
		{ { 0, 0, 0, 0 }, { 0, 0, 0 } }, // pose
		1.0, // radius
		Math::PI / 2.0, // centralHorizontalAngle
		Math::PI / 4.0, // upperVerticalAngle
		-Math::PI / 4.0, // lowerVerticalAngle
	};

	float radius = 1.0;
	float central_horizontal_angle = Math::PI / 2.0;
	float upper_vertical_angle = Math::PI / 4.0;
	float lower_vertical_angle = Math::PI / 4.0;
	uint32_t fallback_segments = 10;

protected:
	static void _bind_methods();

	void _notification(int p_what);

	void update_transform();

	virtual Ref<Mesh> _create_fallback_mesh() override;

public:
	void set_radius(float p_radius);
	float get_radius() const;

	void set_central_horizontal_angle(float p_angle);
	float get_central_horizontal_angle() const;

	void set_upper_vertical_angle(float p_angle);
	float get_upper_vertical_angle() const;

	void set_lower_vertical_angle(float p_angle);
	float get_lower_vertical_angle() const;

	void set_fallback_segments(uint32_t p_fallback_segments);
	uint32_t get_fallback_segments() const;

	virtual Vector2 intersects_ray(const Vector3 &p_origin, const Vector3 &p_direction) const override;

	OpenXRCompositionLayerEquirect();
	~OpenXRCompositionLayerEquirect();
};

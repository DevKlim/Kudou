#pragma once

#include <openxr/openxr.h>

#include "openxr_composition_layer.h"

class OpenXRCompositionLayerCylinder : public OpenXRCompositionLayer {
	GDCLASS(OpenXRCompositionLayerCylinder, OpenXRCompositionLayer);

	XrCompositionLayerCylinderKHR composition_layer = {
		XR_TYPE_COMPOSITION_LAYER_CYLINDER_KHR, // type
		nullptr, // next
		0, // layerFlags
		XR_NULL_HANDLE, // space
		XR_EYE_VISIBILITY_BOTH, // eyeVisibility
		{}, // subImage
		{ { 0, 0, 0, 0 }, { 0, 0, 0 } }, // pose
		1.0, // radius
		Math::PI / 2.0, // centralAngle
		1.0, // aspectRatio
	};

	float radius = 1.0;
	float aspect_ratio = 1.0;
	float central_angle = Math::PI / 2.0;
	uint32_t fallback_segments = 10;

protected:
	static void _bind_methods();

	void _notification(int p_what);

	void update_transform();

	virtual Ref<Mesh> _create_fallback_mesh() override;

public:
	void set_radius(float p_radius);
	float get_radius() const;

	void set_aspect_ratio(float p_aspect_ratio);
	float get_aspect_ratio() const;

	void set_central_angle(float p_angle);
	float get_central_angle() const;

	void set_fallback_segments(uint32_t p_fallback_segments);
	uint32_t get_fallback_segments() const;

	virtual Vector2 intersects_ray(const Vector3 &p_origin, const Vector3 &p_direction) const override;

	OpenXRCompositionLayerCylinder();
	~OpenXRCompositionLayerCylinder();
};

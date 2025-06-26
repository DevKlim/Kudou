#pragma once

#include <openxr/openxr.h>

#include "openxr_composition_layer.h"

class OpenXRCompositionLayerQuad : public OpenXRCompositionLayer {
	GDCLASS(OpenXRCompositionLayerQuad, OpenXRCompositionLayer);

	XrCompositionLayerQuad composition_layer = {
		XR_TYPE_COMPOSITION_LAYER_QUAD, // type
		nullptr, // next
		0, // layerFlags
		XR_NULL_HANDLE, // space
		XR_EYE_VISIBILITY_BOTH, // eyeVisibility
		{}, // subImage
		{ { 0, 0, 0, 0 }, { 0, 0, 0 } }, // pose
		{ 1.0, 1.0 }, // size
	};

	Size2 quad_size = Size2(1.0, 1.0);

protected:
	static void _bind_methods();

	void _notification(int p_what);

	void update_transform();

	virtual Ref<Mesh> _create_fallback_mesh() override;

public:
	void set_quad_size(const Size2 &p_size);
	Size2 get_quad_size() const;

	virtual Vector2 intersects_ray(const Vector3 &p_origin, const Vector3 &p_direction) const override;

	OpenXRCompositionLayerQuad();
	~OpenXRCompositionLayerQuad();
};

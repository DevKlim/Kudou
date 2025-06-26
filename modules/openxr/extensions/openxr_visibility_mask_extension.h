#pragma once

#include "../util.h"

#include "core/templates/vector.h"
#include "openxr_extension_wrapper.h"
#include "scene/resources/mesh.h"

// The OpenXR visibility mask extension provides a mesh for each eye that
// can be used as a mask to determine which part of our rendered result
// is actually visible to the user. Due to lens distortion the edges of
// the rendered image are never used in the final result output on the HMD.
//
// Blacking out this are of the render result can remove a fair amount of
// overhead in rendering part of the screen that is unused.
//
// https://registry.khronos.org/OpenXR/specs/1.0/html/xrspec.html#XR_KHR_visibility_mask

class OpenXRVisibilityMaskExtension : public OpenXRExtensionWrapper {
	GDCLASS(OpenXRVisibilityMaskExtension, OpenXRExtensionWrapper);

protected:
	static void _bind_methods() {}

public:
	static OpenXRVisibilityMaskExtension *get_singleton();

	OpenXRVisibilityMaskExtension();
	virtual ~OpenXRVisibilityMaskExtension() override;

	virtual HashMap<String, bool *> get_requested_extensions() override;

	virtual void on_instance_created(const XrInstance p_instance) override;

	virtual void on_session_created(const XrSession p_instance) override;
	virtual void on_session_destroyed() override;

	virtual void on_pre_render() override;
	virtual bool on_event_polled(const XrEventDataBuffer &event) override;

	bool is_available();
	RID get_mesh();

private:
	static OpenXRVisibilityMaskExtension *singleton;

	bool available = false;
	bool is_dirty = false;

	RID shader;
	RID material;
	RID mesh;

	struct MeshData {
		Vector<XrVector2f> vertices;
		Vector<uint32_t> indices;
	};

	uint32_t mesh_count = 0;
	MeshData mesh_data[4];

	void _update_mesh_data(uint32_t p_view);
	void _update_mesh();

	// OpenXR API call wrappers
	EXT_PROTO_XRRESULT_FUNC5(xrGetVisibilityMaskKHR, (XrSession), session, (XrViewConfigurationType), viewConfigurationType, (uint32_t), viewIndex, (XrVisibilityMaskTypeKHR), visibilityMaskType, (XrVisibilityMaskKHR *), visibilityMask);
};

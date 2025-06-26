#pragma once

#include "core/string/ustring.h"

#include <openxr/openxr.h>

class OpenXRUtil {
public:
	static String get_view_configuration_name(XrViewConfigurationType p_view_configuration);
	static String get_reference_space_name(XrReferenceSpaceType p_reference_space);
	static String get_structure_type_name(XrStructureType p_structure_type);
	static String get_session_state_name(XrSessionState p_session_state);
	static String get_action_type_name(XrActionType p_action_type);
	static String get_environment_blend_mode_name(XrEnvironmentBlendMode p_blend_mode);
	static String make_xr_version_string(XrVersion p_version);

	// Copied from OpenXR xr_linear.h private header, so we can still link against
	// system-provided packages without relying on our `thirdparty` code.

	// Column-major, pre-multiplied. This type does not exist in the OpenXR API and is provided for convenience.
	typedef struct XrMatrix4x4f {
		float m[16];
	} XrMatrix4x4f;

	typedef enum GraphicsAPI {
		GRAPHICS_VULKAN,
		GRAPHICS_OPENGL,
		GRAPHICS_OPENGL_ES,
		GRAPHICS_D3D
	} GraphicsAPI;

	static void XrMatrix4x4f_CreateProjection(XrMatrix4x4f *result, GraphicsAPI graphicsApi, const float tanAngleLeft,
			const float tanAngleRight, const float tanAngleUp, float const tanAngleDown,
			const float nearZ, const float farZ);
	static void XrMatrix4x4f_CreateProjectionFov(XrMatrix4x4f *result, GraphicsAPI graphicsApi, const XrFovf fov,
			const float nearZ, const float farZ);
};

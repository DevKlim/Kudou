#pragma once

// This extension implements the FB update swapchain extension.
// This is an extension Meta added to further configure the swapchain.
// Other Android based devices are implementing this as well, see:
// https://github.khronos.org/OpenXR-Inventory/extension_support.html#XR_FB_swapchain_update_state

#include "../openxr_api.h"
#include "../util.h"
#include "openxr_composition_layer_extension.h"
#include "openxr_extension_wrapper.h"

class OpenXRFBUpdateSwapchainExtension : public OpenXRExtensionWrapper {
	GDCLASS(OpenXRFBUpdateSwapchainExtension, OpenXRExtensionWrapper);

protected:
	static void _bind_methods() {}

	friend class OpenXRFBFoveationExtension;

public:
	static OpenXRFBUpdateSwapchainExtension *get_singleton();

	OpenXRFBUpdateSwapchainExtension(const String &p_rendering_driver);
	virtual ~OpenXRFBUpdateSwapchainExtension() override;

	virtual HashMap<String, bool *> get_requested_extensions() override;

	virtual void on_instance_created(const XrInstance p_instance) override;
	virtual void on_instance_destroyed() override;

	bool is_enabled() const;
	bool is_android_ext_enabled() const;

	void update_swapchain_state(XrSwapchain p_swapchain, const OpenXRViewportCompositionLayerProvider::SwapchainState *p_swapchain_state);

	void update_swapchain_surface_size(XrSwapchain p_swapchain, const Size2i &p_size);

private:
	static OpenXRFBUpdateSwapchainExtension *singleton;

	// Setup
	String rendering_driver;
	bool fb_swapchain_update_state_ext = false;
	bool fb_swapchain_update_state_vulkan_ext = false;
	bool fb_swapchain_update_state_opengles_ext = false;
	bool fb_swapchain_update_state_android_ext = false;

	uint32_t filter_to_gl(OpenXRViewportCompositionLayerProvider::Filter p_filter, OpenXRViewportCompositionLayerProvider::MipmapMode p_mipmap_mode = OpenXRViewportCompositionLayerProvider::MipmapMode::MIPMAP_MODE_DISABLED);
	uint32_t wrap_to_gl(OpenXRViewportCompositionLayerProvider::Wrap p_wrap);
	uint32_t swizzle_to_gl(OpenXRViewportCompositionLayerProvider::Swizzle p_swizzle);

	uint32_t filter_to_vk(OpenXRViewportCompositionLayerProvider::Filter p_filter);
	uint32_t mipmap_mode_to_vk(OpenXRViewportCompositionLayerProvider::MipmapMode p_mipmap);
	uint32_t wrap_to_vk(OpenXRViewportCompositionLayerProvider::Wrap p_wrap);
	uint32_t swizzle_to_vk(OpenXRViewportCompositionLayerProvider::Swizzle p_swizzle);

	// OpenXR API call wrappers
	EXT_PROTO_XRRESULT_FUNC2(xrUpdateSwapchainFB, (XrSwapchain), swapchain, (const XrSwapchainStateBaseHeaderFB *), state);
	EXT_PROTO_XRRESULT_FUNC2(xrGetSwapchainStateFB, (XrSwapchain), swapchain, (XrSwapchainStateBaseHeaderFB *), state);
};

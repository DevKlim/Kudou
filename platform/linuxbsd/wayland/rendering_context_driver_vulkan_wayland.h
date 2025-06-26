#pragma once

#ifdef VULKAN_ENABLED

#include "drivers/vulkan/rendering_context_driver_vulkan.h"

class RenderingContextDriverVulkanWayland : public RenderingContextDriverVulkan {
private:
	virtual const char *_get_platform_surface_extension() const override final;

protected:
	SurfaceID surface_create(const void *p_platform_data) override final;

public:
	struct WindowPlatformData {
		struct wl_display *display;
		struct wl_surface *surface;
	};

	RenderingContextDriverVulkanWayland();
	~RenderingContextDriverVulkanWayland();
};

#endif // VULKAN_ENABLED

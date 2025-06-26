#pragma once

#ifdef VULKAN_ENABLED

#include "drivers/vulkan/rendering_context_driver_vulkan.h"

struct ANativeWindow;

class RenderingContextDriverVulkanAndroid : public RenderingContextDriverVulkan {
private:
	virtual const char *_get_platform_surface_extension() const override final;

protected:
	SurfaceID surface_create(const void *p_platform_data) override final;
	bool _use_validation_layers() const override final;

public:
	struct WindowPlatformData {
		ANativeWindow *window;
	};

	RenderingContextDriverVulkanAndroid() = default;
	~RenderingContextDriverVulkanAndroid() override = default;
};

#endif // VULKAN_ENABLED

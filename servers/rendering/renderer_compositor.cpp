#include "renderer_compositor.h"

#include "core/config/project_settings.h"

#ifndef XR_DISABLED
#include "servers/xr_server.h"
#endif // XR_DISABLED

RendererCompositor *RendererCompositor::singleton = nullptr;

RendererCompositor *(*RendererCompositor::_create_func)() = nullptr;
bool RendererCompositor::low_end = false;

RendererCompositor *RendererCompositor::create() {
	return _create_func();
}

bool RendererCompositor::is_xr_enabled() const {
	return xr_enabled;
}

RendererCompositor::RendererCompositor() {
	ERR_FAIL_COND_MSG(singleton != nullptr, "A RendererCompositor singleton already exists.");
	singleton = this;

#ifndef XR_DISABLED
	if (XRServer::get_xr_mode() == XRServer::XRMODE_DEFAULT) {
		xr_enabled = GLOBAL_GET("xr/shaders/enabled");
	} else {
		xr_enabled = XRServer::get_xr_mode() == XRServer::XRMODE_ON;
	}
#endif // XR_DISABLED
}

RendererCompositor::~RendererCompositor() {
	singleton = nullptr;
}

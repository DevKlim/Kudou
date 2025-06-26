#pragma once

#if defined(X11_ENABLED) && defined(GLES3_ENABLED)

#include "core/os/os.h"
#include "core/templates/local_vector.h"
#include "drivers/egl/egl_manager.h"
#include "servers/display_server.h"

#include <X11/Xlib.h>

class GLManagerEGL_X11 : public EGLManager {
private:
	virtual const char *_get_platform_extension_name() const override;
	virtual EGLenum _get_platform_extension_enum() const override;
	virtual EGLenum _get_platform_api_enum() const override;
	virtual Vector<EGLAttrib> _get_platform_display_attributes() const override;
	virtual Vector<EGLint> _get_platform_context_attribs() const override;

public:
	void window_resize(DisplayServer::WindowID p_window_id, int p_width, int p_height) {}

	GLManagerEGL_X11() {}
	~GLManagerEGL_X11() {}
};

#endif // X11_ENABLED && GLES3_ENABLED

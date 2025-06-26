#pragma once

#if defined(MACOS_ENABLED) && defined(GLES3_ENABLED)

#include "core/os/os.h"
#include "core/templates/local_vector.h"
#include "drivers/egl/egl_manager.h"
#include "servers/display_server.h"

// Suppress redefinition conflicts
#define FontVariation __FontVariation
#define BitMap __BitMap

#import <AppKit/AppKit.h>
#import <ApplicationServices/ApplicationServices.h>
#import <CoreVideo/CoreVideo.h>

#undef BitMap
#undef FontVariation

class GLManagerANGLE_MacOS : public EGLManager {
private:
	virtual const char *_get_platform_extension_name() const override;
	virtual EGLenum _get_platform_extension_enum() const override;
	virtual EGLenum _get_platform_api_enum() const override;
	virtual Vector<EGLAttrib> _get_platform_display_attributes() const override;
	virtual Vector<EGLint> _get_platform_context_attribs() const override;

public:
	void window_resize(DisplayServer::WindowID p_window_id, int p_width, int p_height) {}

	GLManagerANGLE_MacOS() {}
	~GLManagerANGLE_MacOS() {}
};

#endif // MACOS_ENABLED && GLES3_ENABLED

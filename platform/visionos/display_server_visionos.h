#pragma once

#include "drivers/apple_embedded/display_server_apple_embedded.h"

class DisplayServerVisionOS : public DisplayServerAppleEmbedded {
	GDSOFTCLASS(DisplayServerVisionOS, DisplayServerAppleEmbedded);

	_THREAD_SAFE_CLASS_

	DisplayServerVisionOS(const String &p_rendering_driver, DisplayServer::WindowMode p_mode, DisplayServer::VSyncMode p_vsync_mode, uint32_t p_flags, const Vector2i *p_position, const Vector2i &p_resolution, int p_screen, Context p_context, int64_t p_parent_window, Error &r_error);
	~DisplayServerVisionOS();

public:
	static DisplayServerVisionOS *get_singleton();

	static void register_visionos_driver();
	static DisplayServer *create_func(const String &p_rendering_driver, WindowMode p_mode, DisplayServer::VSyncMode p_vsync_mode, uint32_t p_flags, const Vector2i *p_position, const Vector2i &p_resolution, int p_screen, Context p_context, int64_t p_parent_window, Error &r_error);

	virtual String get_name() const override;

	virtual int screen_get_dpi(int p_screen = SCREEN_OF_MAIN_WINDOW) const override;
	virtual float screen_get_scale(int p_screen = SCREEN_OF_MAIN_WINDOW) const override;
	virtual float screen_get_refresh_rate(int p_screen = SCREEN_OF_MAIN_WINDOW) const override;
};

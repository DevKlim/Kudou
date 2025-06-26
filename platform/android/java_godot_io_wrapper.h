#pragma once

#include "jni_utils.h"

#include "core/math/rect2i.h"
#include "core/variant/typed_array.h"

#include <android/log.h>
#include <jni.h>

// Class that makes functions in java/src/org/godotengine/godot/GodotIO.java callable from C++
class GodotIOJavaWrapper {
private:
	jobject godot_io_instance;
	jclass cls;

	jmethodID _open_URI = 0;
	jmethodID _get_cache_dir = 0;
	jmethodID _get_data_dir = 0;
	jmethodID _get_temp_dir = 0;
	jmethodID _get_display_cutouts = 0;
	jmethodID _get_display_rotation = 0;
	jmethodID _get_display_safe_area = 0;
	jmethodID _get_locale = 0;
	jmethodID _get_model = 0;
	jmethodID _get_screen_DPI = 0;
	jmethodID _get_scaled_density = 0;
	jmethodID _get_screen_refresh_rate = 0;
	jmethodID _get_unique_id = 0;
	jmethodID _show_keyboard = 0;
	jmethodID _hide_keyboard = 0;
	jmethodID _has_hardware_keyboard = 0;
	jmethodID _set_screen_orientation = 0;
	jmethodID _get_screen_orientation = 0;
	jmethodID _get_system_dir = 0;

public:
	GodotIOJavaWrapper(JNIEnv *p_env, jobject p_godot_io_instance);
	~GodotIOJavaWrapper();

	jobject get_instance();

	Error open_uri(const String &p_uri);
	String get_cache_dir();
	String get_temp_dir();
	String get_user_data_dir(const String &p_user_dir);
	String get_locale();
	String get_model();
	int get_screen_dpi();
	float get_scaled_density();
	float get_screen_refresh_rate(float fallback);
	TypedArray<Rect2> get_display_cutouts();
	Rect2i get_display_safe_area();
	String get_unique_id();
	bool has_vk();
	bool has_hardware_keyboard();
	void show_vk(const String &p_existing, int p_type, int p_max_input_length, int p_cursor_start, int p_cursor_end);
	void hide_vk();
	int get_vk_height();
	void set_vk_height(int p_height);
	void set_screen_orientation(int p_orient);
	int get_screen_orientation();
	String get_system_dir(int p_dir, bool p_shared_storage);
	int get_display_rotation();
};

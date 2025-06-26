#pragma once

#include "jni_utils.h"

#include "core/math/vector2.h"

#include <android/log.h>
#include <jni.h>

// Class that makes functions in java/src/org/godotengine/godot/GodotRenderView.java callable from C++
class GodotJavaViewWrapper {
private:
	jclass _cls;

	jobject _godot_view;

	jmethodID _can_capture_pointer = 0;
	jmethodID _request_pointer_capture = 0;
	jmethodID _release_pointer_capture = 0;

	jmethodID _configure_pointer_icon = 0;
	jmethodID _set_pointer_icon = 0;

public:
	GodotJavaViewWrapper(jobject godot_view);

	bool can_update_pointer_icon() const;
	bool can_capture_pointer() const;

	void request_pointer_capture();
	void release_pointer_capture();

	void configure_pointer_icon(int pointer_type, const String &image_path, const Vector2 &p_hotspot);
	void set_pointer_icon(int pointer_type);

	~GodotJavaViewWrapper();
};

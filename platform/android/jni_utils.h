#pragma once

#include "thread_jandroid.h"

#include "core/config/engine.h"
#include "core/string/ustring.h"
#include "core/variant/variant.h"

#include <jni.h>

struct jvalret {
	jobject obj;
	jvalue val;
	jvalret() { obj = nullptr; }
};

jvalret _variant_to_jvalue(JNIEnv *env, Variant::Type p_type, const Variant *p_arg, bool force_jobject = false);

String _get_class_name(JNIEnv *env, jclass cls, bool *array);

Variant _jobject_to_variant(JNIEnv *env, jobject obj);

Variant::Type get_jni_type(const String &p_type);

/**
 * Convert a Godot Callable to a org.godotengine.godot.variant.Callable java object.
 * @param p_env JNI environment instance
 * @param p_callable Callable parameter to convert. If null or invalid type, a null jobject is returned.
 * @return org.godotengine.godot.variant.Callable jobject or null
 */
jobject callable_to_jcallable(JNIEnv *p_env, const Variant &p_callable);

/**
 * Convert a org.godotengine.godot.variant.Callable java object to a Godot Callable variant.
 * @param p_env JNI environment instance
 * @param p_jcallable_obj org.godotengine.godot.variant.Callable java object to convert.
 * @return Callable variant
 */
Callable jcallable_to_callable(JNIEnv *p_env, jobject p_jcallable_obj);

/**
 * Converts a java.lang.CharSequence object to a Godot String.
 * @param p_env  JNI environment instance
 * @param p_charsequence java.lang.CharSequence object to convert
 * @return Godot String instance.
 */
String charsequence_to_string(JNIEnv *p_env, jobject p_charsequence);

/**
 * Converts JNI jstring to Godot String.
 * @param source Source JNI string. If null an empty string is returned.
 * @param env JNI environment instance. If null obtained by get_jni_env().
 * @return Godot string instance.
 */
static inline String jstring_to_string(jstring source, JNIEnv *env = nullptr) {
	String result;
	if (source) {
		if (!env) {
			env = get_jni_env();
		}
		const char *const source_utf8 = env->GetStringUTFChars(source, nullptr);
		if (source_utf8) {
			result.append_utf8(source_utf8);
			env->ReleaseStringUTFChars(source, source_utf8);
		}
	}
	return result;
}

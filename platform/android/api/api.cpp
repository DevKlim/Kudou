#include "api.h"

#include "java_class_wrapper.h"
#include "jni_singleton.h"

#include "core/config/engine.h"

#if !defined(ANDROID_ENABLED)
static JavaClassWrapper *java_class_wrapper = nullptr;
#endif

void register_android_api() {
#if !defined(ANDROID_ENABLED)
	// On Android platforms, the `java_class_wrapper` instantiation occurs in
	// `platform/android/java_godot_lib_jni.cpp#Java_org_godotengine_godot_GodotLib_setup`
	java_class_wrapper = memnew(JavaClassWrapper);
#endif
	GDREGISTER_CLASS(JNISingleton);
	GDREGISTER_CLASS(JavaClass);
	GDREGISTER_CLASS(JavaObject);
	GDREGISTER_CLASS(JavaClassWrapper);
	Engine::get_singleton()->add_singleton(Engine::Singleton("JavaClassWrapper", JavaClassWrapper::get_singleton()));
}

void unregister_android_api() {
#if !defined(ANDROID_ENABLED)
	memdelete(java_class_wrapper);
#endif
}

void JavaClass::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_java_class_name"), &JavaClass::get_java_class_name);
	ClassDB::bind_method(D_METHOD("get_java_method_list"), &JavaClass::get_java_method_list);
	ClassDB::bind_method(D_METHOD("get_java_parent_class"), &JavaClass::get_java_parent_class);
}

void JavaObject::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_java_class"), &JavaObject::get_java_class);
}

void JavaClassWrapper::_bind_methods() {
	ClassDB::bind_method(D_METHOD("wrap", "name"), &JavaClassWrapper::wrap);
	ClassDB::bind_method(D_METHOD("get_exception"), &JavaClassWrapper::get_exception);
}

#if !defined(ANDROID_ENABLED)
bool JavaClass::_get(const StringName &p_name, Variant &r_ret) const {
	return false;
}

Variant JavaClass::callp(const StringName &, const Variant **, int, Callable::CallError &) {
	return Variant();
}

String JavaClass::get_java_class_name() const {
	return "";
}

TypedArray<Dictionary> JavaClass::get_java_method_list() const {
	return TypedArray<Dictionary>();
}

Ref<JavaClass> JavaClass::get_java_parent_class() const {
	return Ref<JavaClass>();
}

JavaClass::JavaClass() {
}

JavaClass::~JavaClass() {
}

Variant JavaObject::callp(const StringName &, const Variant **, int, Callable::CallError &) {
	return Variant();
}

Ref<JavaClass> JavaObject::get_java_class() const {
	return Ref<JavaClass>();
}

JavaClassWrapper *JavaClassWrapper::singleton = nullptr;

Ref<JavaClass> JavaClassWrapper::_wrap(const String &, bool) {
	return Ref<JavaClass>();
}

JavaClassWrapper::JavaClassWrapper() {
	singleton = this;
}

#endif

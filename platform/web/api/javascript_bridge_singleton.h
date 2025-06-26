#pragma once

#include "core/object/class_db.h"
#include "core/object/ref_counted.h"

class JavaScriptObject : public RefCounted {
private:
	GDCLASS(JavaScriptObject, RefCounted);

protected:
	virtual bool _set(const StringName &p_name, const Variant &p_value) { return false; }
	virtual bool _get(const StringName &p_name, Variant &r_ret) const { return false; }
	virtual void _get_property_list(List<PropertyInfo> *p_list) const {}
};

class JavaScriptBridge : public Object {
private:
	GDCLASS(JavaScriptBridge, Object);

	static JavaScriptBridge *singleton;

protected:
	static void _bind_methods();

public:
	Variant eval(const String &p_code, bool p_use_global_exec_context = false);
	Ref<JavaScriptObject> get_interface(const String &p_interface);
	Ref<JavaScriptObject> create_callback(const Callable &p_callable);
	bool is_js_buffer(Ref<JavaScriptObject> p_js_obj);
	PackedByteArray js_buffer_to_packed_byte_array(Ref<JavaScriptObject> p_js_obj);
	Variant _create_object_bind(const Variant **p_args, int p_argcount, Callable::CallError &r_error);
	void download_buffer(Vector<uint8_t> p_arr, const String &p_name, const String &p_mime = "application/octet-stream");
	bool pwa_needs_update() const;
	Error pwa_update();
	void force_fs_sync();

	static JavaScriptBridge *get_singleton();
	JavaScriptBridge();
	~JavaScriptBridge();
};

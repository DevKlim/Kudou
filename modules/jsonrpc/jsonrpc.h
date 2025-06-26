#pragma once

#include "core/object/class_db.h"
#include "core/variant/variant.h"

class JSONRPC : public Object {
	GDCLASS(JSONRPC, Object)

	HashMap<String, Callable> methods;

protected:
	static void _bind_methods();

#ifndef DISABLE_DEPRECATED
	void _set_scope_bind_compat_104890(const String &p_scope, Object *p_obj);
	static void _bind_compatibility_methods();
#endif

public:
	JSONRPC();
	~JSONRPC();

	enum ErrorCode {
		PARSE_ERROR = -32700,
		INVALID_REQUEST = -32600,
		METHOD_NOT_FOUND = -32601,
		INVALID_PARAMS = -32602,
		INTERNAL_ERROR = -32603,
	};

	Dictionary make_response_error(int p_code, const String &p_message, const Variant &p_id = Variant()) const;
	Dictionary make_response(const Variant &p_value, const Variant &p_id);
	Dictionary make_notification(const String &p_method, const Variant &p_params);
	Dictionary make_request(const String &p_method, const Variant &p_params, const Variant &p_id);

	Variant process_action(const Variant &p_action, bool p_process_arr_elements = false);
	String process_string(const String &p_input);

	void set_method(const String &p_name, const Callable &p_callback);
};

VARIANT_ENUM_CAST(JSONRPC::ErrorCode);

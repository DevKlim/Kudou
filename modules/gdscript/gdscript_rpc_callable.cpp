#include "gdscript_rpc_callable.h"

#include "core/object/script_language.h"
#include "core/templates/hashfuncs.h"
#include "scene/main/node.h"

bool GDScriptRPCCallable::compare_equal(const CallableCustom *p_a, const CallableCustom *p_b) {
	return p_a->hash() == p_b->hash();
}

bool GDScriptRPCCallable::compare_less(const CallableCustom *p_a, const CallableCustom *p_b) {
	return p_a->hash() < p_b->hash();
}

uint32_t GDScriptRPCCallable::hash() const {
	return h;
}

String GDScriptRPCCallable::get_as_text() const {
	String class_name = object->get_class();
	Ref<Script> script = object->get_script();
	if (script.is_valid()) {
		if (!script->get_global_name().is_empty()) {
			class_name += "(" + script->get_global_name() + ")";
		} else if (script->get_path().is_resource_file()) {
			class_name += "(" + script->get_path().get_file() + ")";
		}
	}
	return class_name + "::" + String(method) + " (rpc)";
}

CallableCustom::CompareEqualFunc GDScriptRPCCallable::get_compare_equal_func() const {
	return compare_equal;
}

CallableCustom::CompareLessFunc GDScriptRPCCallable::get_compare_less_func() const {
	return compare_less;
}

ObjectID GDScriptRPCCallable::get_object() const {
	return object->get_instance_id();
}

StringName GDScriptRPCCallable::get_method() const {
	return method;
}

int GDScriptRPCCallable::get_argument_count(bool &r_is_valid) const {
	return object->get_method_argument_count(method, &r_is_valid);
}

void GDScriptRPCCallable::call(const Variant **p_arguments, int p_argcount, Variant &r_return_value, Callable::CallError &r_call_error) const {
	r_return_value = object->callp(method, p_arguments, p_argcount, r_call_error);
}

GDScriptRPCCallable::GDScriptRPCCallable(Object *p_object, const StringName &p_method) {
	ERR_FAIL_NULL(p_object);
	object = p_object;
	method = p_method;
	h = method.hash();
	h = hash_murmur3_one_64(object->get_instance_id(), h);
	node = Object::cast_to<Node>(object);
	ERR_FAIL_NULL_MSG(node, "RPC can only be defined on class that extends Node.");
}

Error GDScriptRPCCallable::rpc(int p_peer_id, const Variant **p_arguments, int p_argcount, Callable::CallError &r_call_error) const {
	if (unlikely(!node)) {
		r_call_error.error = Callable::CallError::CALL_ERROR_INSTANCE_IS_NULL;
		return ERR_UNCONFIGURED;
	}
	r_call_error.error = Callable::CallError::CALL_OK;
	return node->rpcp(p_peer_id, method, p_arguments, p_argcount);
}

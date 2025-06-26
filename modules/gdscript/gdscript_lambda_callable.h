#pragma once

#include "gdscript.h"

#include "core/object/ref_counted.h"
#include "core/templates/vector.h"
#include "core/variant/callable.h"
#include "core/variant/variant.h"

class GDScriptFunction;
class GDScriptInstance;

class GDScriptLambdaCallable : public CallableCustom {
	GDScript::UpdatableFuncPtr function;
	Ref<GDScript> script;
	uint32_t h;

	Vector<Variant> captures;

	static bool compare_equal(const CallableCustom *p_a, const CallableCustom *p_b);
	static bool compare_less(const CallableCustom *p_a, const CallableCustom *p_b);

public:
	bool is_valid() const override;
	uint32_t hash() const override;
	String get_as_text() const override;
	CompareEqualFunc get_compare_equal_func() const override;
	CompareLessFunc get_compare_less_func() const override;
	ObjectID get_object() const override;
	StringName get_method() const override;
	int get_argument_count(bool &r_is_valid) const override;
	void call(const Variant **p_arguments, int p_argcount, Variant &r_return_value, Callable::CallError &r_call_error) const override;

	GDScriptLambdaCallable(GDScriptLambdaCallable &) = delete;
	GDScriptLambdaCallable(const GDScriptLambdaCallable &) = delete;
	GDScriptLambdaCallable(Ref<GDScript> p_script, GDScriptFunction *p_function, const Vector<Variant> &p_captures);
	virtual ~GDScriptLambdaCallable() = default;
};

// Lambda callable that references a particular object, so it can use `self` in the body.
class GDScriptLambdaSelfCallable : public CallableCustom {
	GDScript::UpdatableFuncPtr function;
	Ref<RefCounted> reference; // For objects that are RefCounted, keep a reference.
	Object *object = nullptr; // For non RefCounted objects, use a direct pointer.
	uint32_t h;

	Vector<Variant> captures;

	static bool compare_equal(const CallableCustom *p_a, const CallableCustom *p_b);
	static bool compare_less(const CallableCustom *p_a, const CallableCustom *p_b);

public:
	bool is_valid() const override;
	uint32_t hash() const override;
	String get_as_text() const override;
	CompareEqualFunc get_compare_equal_func() const override;
	CompareLessFunc get_compare_less_func() const override;
	ObjectID get_object() const override;
	StringName get_method() const override;
	int get_argument_count(bool &r_is_valid) const override;
	void call(const Variant **p_arguments, int p_argcount, Variant &r_return_value, Callable::CallError &r_call_error) const override;

	GDScriptLambdaSelfCallable(GDScriptLambdaSelfCallable &) = delete;
	GDScriptLambdaSelfCallable(const GDScriptLambdaSelfCallable &) = delete;
	GDScriptLambdaSelfCallable(Ref<RefCounted> p_self, GDScriptFunction *p_function, const Vector<Variant> &p_captures);
	GDScriptLambdaSelfCallable(Object *p_self, GDScriptFunction *p_function, const Vector<Variant> &p_captures);
	virtual ~GDScriptLambdaSelfCallable() = default;
};

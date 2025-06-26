#pragma once

#include "../gltf_defines.h"

#include "core/io/resource.h"

struct GLTFAccessor : public Resource {
	GDCLASS(GLTFAccessor, Resource);
	friend class GLTFDocument;

public:
	enum GLTFAccessorType {
		TYPE_SCALAR,
		TYPE_VEC2,
		TYPE_VEC3,
		TYPE_VEC4,
		TYPE_MAT2,
		TYPE_MAT3,
		TYPE_MAT4,
	};

	enum GLTFComponentType {
		COMPONENT_TYPE_NONE = 0,
		COMPONENT_TYPE_SIGNED_BYTE = 5120,
		COMPONENT_TYPE_UNSIGNED_BYTE = 5121,
		COMPONENT_TYPE_SIGNED_SHORT = 5122,
		COMPONENT_TYPE_UNSIGNED_SHORT = 5123,
		COMPONENT_TYPE_SIGNED_INT = 5124,
		COMPONENT_TYPE_UNSIGNED_INT = 5125,
		COMPONENT_TYPE_SINGLE_FLOAT = 5126,
		COMPONENT_TYPE_DOUBLE_FLOAT = 5130,
		COMPONENT_TYPE_HALF_FLOAT = 5131,
		COMPONENT_TYPE_SIGNED_LONG = 5134,
		COMPONENT_TYPE_UNSIGNED_LONG = 5135,
	};

private:
	GLTFBufferViewIndex buffer_view = -1;
	int64_t byte_offset = 0;
	GLTFComponentType component_type = COMPONENT_TYPE_NONE;
	bool normalized = false;
	int64_t count = 0;
	GLTFAccessorType accessor_type = GLTFAccessorType::TYPE_SCALAR;
	Vector<double> min;
	Vector<double> max;
	int64_t sparse_count = 0;
	GLTFBufferViewIndex sparse_indices_buffer_view = 0;
	int64_t sparse_indices_byte_offset = 0;
	GLTFComponentType sparse_indices_component_type = COMPONENT_TYPE_NONE;
	GLTFBufferViewIndex sparse_values_buffer_view = 0;
	int64_t sparse_values_byte_offset = 0;

protected:
	static void _bind_methods();

#ifndef DISABLE_DEPRECATED
	// 32-bit and non-const versions for compatibility.
	GLTFBufferViewIndex _get_buffer_view_bind_compat_106220();
	int _get_byte_offset_bind_compat_106220();
	int _get_component_type_bind_compat_106220();
	void _set_component_type_bind_compat_106220(int p_component_type);
	bool _get_normalized_bind_compat_106220();
	int _get_count_bind_compat_106220();
	GLTFAccessorType _get_accessor_type_bind_compat_106220();
	int _get_type_bind_compat_106220();
	Vector<double> _get_min_bind_compat_106220();
	Vector<double> _get_max_bind_compat_106220();
	int _get_sparse_count_bind_compat_106220();
	int _get_sparse_indices_buffer_view_bind_compat_106220();
	int _get_sparse_indices_byte_offset_bind_compat_106220();
	int _get_sparse_indices_component_type_bind_compat_106220();
	void _set_sparse_indices_component_type_bind_compat_106220(int p_sparse_indices_component_type);
	int _get_sparse_values_buffer_view_bind_compat_106220();
	int _get_sparse_values_byte_offset_bind_compat_106220();
	static void _bind_compatibility_methods();
#endif // DISABLE_DEPRECATED

public:
	GLTFBufferViewIndex get_buffer_view() const;
	void set_buffer_view(GLTFBufferViewIndex p_buffer_view);

	int64_t get_byte_offset() const;
	void set_byte_offset(int64_t p_byte_offset);

	GLTFComponentType get_component_type() const;
	void set_component_type(GLTFComponentType p_component_type);

	bool get_normalized() const;
	void set_normalized(bool p_normalized);

	int64_t get_count() const;
	void set_count(int64_t p_count);

	GLTFAccessorType get_accessor_type() const;
	void set_accessor_type(GLTFAccessorType p_accessor_type);

	int get_type() const;
	void set_type(int p_accessor_type);

	Vector<double> get_min() const;
	void set_min(Vector<double> p_min);

	Vector<double> get_max() const;
	void set_max(Vector<double> p_max);

	int64_t get_sparse_count() const;
	void set_sparse_count(int64_t p_sparse_count);

	GLTFBufferViewIndex get_sparse_indices_buffer_view() const;
	void set_sparse_indices_buffer_view(GLTFBufferViewIndex p_sparse_indices_buffer_view);

	int64_t get_sparse_indices_byte_offset() const;
	void set_sparse_indices_byte_offset(int64_t p_sparse_indices_byte_offset);

	GLTFComponentType get_sparse_indices_component_type() const;
	void set_sparse_indices_component_type(GLTFComponentType p_sparse_indices_component_type);

	GLTFBufferViewIndex get_sparse_values_buffer_view() const;
	void set_sparse_values_buffer_view(GLTFBufferViewIndex p_sparse_values_buffer_view);

	int64_t get_sparse_values_byte_offset() const;
	void set_sparse_values_byte_offset(int64_t p_sparse_values_byte_offset);
};

VARIANT_ENUM_CAST(GLTFAccessor::GLTFAccessorType);
VARIANT_ENUM_CAST(GLTFAccessor::GLTFComponentType);

#pragma once

#include "../gltf_defines.h"

#include "core/io/resource.h"

class GLTFBufferView : public Resource {
	GDCLASS(GLTFBufferView, Resource);
	friend class GLTFDocument;

private:
	GLTFBufferIndex buffer = -1;
	int64_t byte_offset = 0;
	int64_t byte_length = 0;
	int64_t byte_stride = -1;
	bool indices = false;
	bool vertex_attributes = false;

protected:
	static void _bind_methods();

#ifndef DISABLE_DEPRECATED
	// Non-const versions for compatibility.
	GLTFBufferIndex _get_buffer_bind_compat_86907();
	int _get_byte_offset_bind_compat_86907();
	int _get_byte_length_bind_compat_86907();
	int _get_byte_stride_bind_compat_86907();
	bool _get_indices_bind_compat_86907();
	static void _bind_compatibility_methods();
#endif // DISABLE_DEPRECATED

public:
	GLTFBufferIndex get_buffer() const;
	void set_buffer(GLTFBufferIndex p_buffer);

	int64_t get_byte_offset() const;
	void set_byte_offset(int64_t p_byte_offset);

	int64_t get_byte_length() const;
	void set_byte_length(int64_t p_byte_length);

	int64_t get_byte_stride() const;
	void set_byte_stride(int64_t p_byte_stride);

	bool get_indices() const;
	void set_indices(bool p_indices);

	bool get_vertex_attributes() const;
	void set_vertex_attributes(bool p_attributes);

	Vector<uint8_t> load_buffer_view_data(const Ref<GLTFState> p_state) const;
};

#pragma once

#include "../gltf_defines.h"

#include "scene/resources/3d/importer_mesh.h"

class GLTFMesh : public Resource {
	GDCLASS(GLTFMesh, Resource);

private:
	String original_name;
	Ref<ImporterMesh> mesh;
	Vector<float> blend_weights;
	TypedArray<Material> instance_materials;
	Dictionary additional_data;

protected:
	static void _bind_methods();

public:
	String get_original_name();
	void set_original_name(String p_name);
	Ref<ImporterMesh> get_mesh();
	void set_mesh(Ref<ImporterMesh> p_mesh);
	Vector<float> get_blend_weights();
	void set_blend_weights(Vector<float> p_blend_weights);
	TypedArray<Material> get_instance_materials();
	void set_instance_materials(TypedArray<Material> p_instance_materials);
	Variant get_additional_data(const StringName &p_extension_name);
	void set_additional_data(const StringName &p_extension_name, Variant p_additional_data);
};

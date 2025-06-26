#include "gltf_mesh.h"

#include "scene/resources/3d/importer_mesh.h"

void GLTFMesh::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_original_name"), &GLTFMesh::get_original_name);
	ClassDB::bind_method(D_METHOD("set_original_name", "original_name"), &GLTFMesh::set_original_name);
	ClassDB::bind_method(D_METHOD("get_mesh"), &GLTFMesh::get_mesh);
	ClassDB::bind_method(D_METHOD("set_mesh", "mesh"), &GLTFMesh::set_mesh);
	ClassDB::bind_method(D_METHOD("get_blend_weights"), &GLTFMesh::get_blend_weights);
	ClassDB::bind_method(D_METHOD("set_blend_weights", "blend_weights"), &GLTFMesh::set_blend_weights);
	ClassDB::bind_method(D_METHOD("get_instance_materials"), &GLTFMesh::get_instance_materials);
	ClassDB::bind_method(D_METHOD("set_instance_materials", "instance_materials"), &GLTFMesh::set_instance_materials);
	ClassDB::bind_method(D_METHOD("get_additional_data", "extension_name"), &GLTFMesh::get_additional_data);
	ClassDB::bind_method(D_METHOD("set_additional_data", "extension_name", "additional_data"), &GLTFMesh::set_additional_data);

	ADD_PROPERTY(PropertyInfo(Variant::STRING, "original_name"), "set_original_name", "get_original_name");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "mesh"), "set_mesh", "get_mesh");
	ADD_PROPERTY(PropertyInfo(Variant::PACKED_FLOAT32_ARRAY, "blend_weights"), "set_blend_weights", "get_blend_weights"); // Vector<float>
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "instance_materials"), "set_instance_materials", "get_instance_materials");
}

String GLTFMesh::get_original_name() {
	return original_name;
}

void GLTFMesh::set_original_name(String p_name) {
	original_name = p_name;
}

Ref<ImporterMesh> GLTFMesh::get_mesh() {
	return mesh;
}

void GLTFMesh::set_mesh(Ref<ImporterMesh> p_mesh) {
	mesh = p_mesh;
}

TypedArray<Material> GLTFMesh::get_instance_materials() {
	return instance_materials;
}

void GLTFMesh::set_instance_materials(TypedArray<Material> p_instance_materials) {
	instance_materials = p_instance_materials;
}

Vector<float> GLTFMesh::get_blend_weights() {
	return blend_weights;
}

void GLTFMesh::set_blend_weights(Vector<float> p_blend_weights) {
	blend_weights = p_blend_weights;
}

Variant GLTFMesh::get_additional_data(const StringName &p_extension_name) {
	return additional_data[p_extension_name];
}

void GLTFMesh::set_additional_data(const StringName &p_extension_name, Variant p_additional_data) {
	additional_data[p_extension_name] = p_additional_data;
}

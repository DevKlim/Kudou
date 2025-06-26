#pragma once

#include "../gltf_document_extension.h"
#include "gltf_physics_body.h"
#include "gltf_physics_shape.h"

class GLTFDocumentExtensionPhysics : public GLTFDocumentExtension {
	GDCLASS(GLTFDocumentExtensionPhysics, GLTFDocumentExtension);

public:
	// Import process.
	Error import_preflight(Ref<GLTFState> p_state, Vector<String> p_extensions) override;
	Vector<String> get_supported_extensions() override;
	Error parse_node_extensions(Ref<GLTFState> p_state, Ref<GLTFNode> p_gltf_node, Dictionary &p_extensions) override;
	Ref<GLTFObjectModelProperty> import_object_model_property(Ref<GLTFState> p_state, const PackedStringArray &p_split_json_pointer, const TypedArray<NodePath> &p_partial_paths) override;
	Node3D *generate_scene_node(Ref<GLTFState> p_state, Ref<GLTFNode> p_gltf_node, Node *p_scene_parent) override;
	// Export process.
	void convert_scene_node(Ref<GLTFState> p_state, Ref<GLTFNode> p_gltf_node, Node *p_scene_node) override;
	Error export_preserialize(Ref<GLTFState> p_state) override;
	Ref<GLTFObjectModelProperty> export_object_model_property(Ref<GLTFState> p_state, const NodePath &p_node_path, const Node *p_godot_node, GLTFNodeIndex p_gltf_node_index, const Object *p_target_object, int p_target_depth) override;
	Error export_node(Ref<GLTFState> p_state, Ref<GLTFNode> p_gltf_node, Dictionary &r_node_json, Node *p_scene_node) override;
};

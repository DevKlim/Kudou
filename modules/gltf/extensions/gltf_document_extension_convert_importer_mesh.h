#pragma once

#include "gltf_document_extension.h"

class MeshInstance3D;

class GLTFDocumentExtensionConvertImporterMesh : public GLTFDocumentExtension {
	GDCLASS(GLTFDocumentExtensionConvertImporterMesh, GLTFDocumentExtension);

protected:
	static void _copy_meta(Object *p_src_object, Object *p_dst_object);

public:
	static MeshInstance3D *convert_importer_mesh_instance_3d(ImporterMeshInstance3D *p_importer_mesh_instance_3d);
	Error import_post(Ref<GLTFState> p_state, Node *p_root) override;
};

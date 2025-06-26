#pragma once

#include "resource_importer_scene.h"

class PostImportPluginSkeletonRenamer : public EditorScenePostImportPlugin {
	GDCLASS(PostImportPluginSkeletonRenamer, EditorScenePostImportPlugin);

public:
	virtual void get_internal_import_options(InternalImportCategory p_category, List<ResourceImporter::ImportOption> *r_options) override;
	virtual void internal_process(InternalImportCategory p_category, Node *p_base_scene, Node *p_node, Ref<Resource> p_resource, const Dictionary &p_options) override;

	void _internal_process(InternalImportCategory p_category, Node *p_base_scene, Node *p_node, Ref<Resource> p_resource, const Dictionary &p_options, const HashMap<String, String> &p_rename_map);
};

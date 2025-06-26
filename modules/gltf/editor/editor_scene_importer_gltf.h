#pragma once

#include "editor/import/3d/resource_importer_scene.h"

class Animation;
class Node;

class EditorSceneFormatImporterGLTF : public EditorSceneFormatImporter {
	GDCLASS(EditorSceneFormatImporterGLTF, EditorSceneFormatImporter);

public:
	virtual void get_extensions(List<String> *r_extensions) const override;
	virtual Node *import_scene(const String &p_path, uint32_t p_flags,
			const HashMap<StringName, Variant> &p_options,
			List<String> *r_missing_deps, Error *r_err = nullptr) override;
	virtual void get_import_options(const String &p_path,
			List<ResourceImporter::ImportOption> *r_options) override;
	virtual void handle_compatibility_options(HashMap<StringName, Variant> &p_import_params) const override;
	virtual Variant get_option_visibility(const String &p_path, const String &p_scene_import_type,
			const String &p_option, const HashMap<StringName, Variant> &p_options) override;
};

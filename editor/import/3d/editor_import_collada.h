#pragma once

#include "editor/import/3d/resource_importer_scene.h"

class EditorSceneFormatImporterCollada : public EditorSceneFormatImporter {
	GDCLASS(EditorSceneFormatImporterCollada, EditorSceneFormatImporter);

public:
	virtual void get_extensions(List<String> *r_extensions) const override;
	virtual Node *import_scene(const String &p_path, uint32_t p_flags, const HashMap<StringName, Variant> &p_options, List<String> *r_missing_deps = nullptr, Error *r_err = nullptr) override;
};

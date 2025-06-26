#pragma once

#include "../gltf_document.h"
#include "editor_scene_exporter_gltf_settings.h"

#include "editor/plugins/editor_plugin.h"

class EditorFileDialog;
class EditorInspector;

class SceneExporterGLTFPlugin : public EditorPlugin {
	GDCLASS(SceneExporterGLTFPlugin, EditorPlugin);

	Ref<GLTFDocument> _gltf_document;
	Ref<EditorSceneExporterGLTFSettings> _export_settings;
	EditorInspector *_settings_inspector = nullptr;
	EditorFileDialog *_file_dialog = nullptr;
	void _popup_gltf_export_dialog();
	void _export_scene_as_gltf(const String &p_file_path);

public:
	virtual String get_plugin_name() const override;
	bool has_main_screen() const override;
	SceneExporterGLTFPlugin();
};

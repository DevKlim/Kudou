#pragma once

#include "editor/plugins/editor_plugin.h"
#include "scene/3d/occluder_instance_3d.h"
#include "scene/resources/material.h"

class EditorFileDialog;

class OccluderInstance3DEditorPlugin : public EditorPlugin {
	GDCLASS(OccluderInstance3DEditorPlugin, EditorPlugin);

	OccluderInstance3D *occluder_instance = nullptr;

	Button *bake = nullptr;

	EditorFileDialog *file_dialog = nullptr;

	void _bake_select_file(const String &p_file);
	void _bake();

protected:
	static void _bind_methods();

public:
	virtual String get_plugin_name() const override { return "OccluderInstance3D"; }
	bool has_main_screen() const override { return false; }
	virtual void edit(Object *p_object) override;
	virtual bool handles(Object *p_object) const override;
	virtual void make_visible(bool p_visible) override;

	OccluderInstance3DEditorPlugin();
};

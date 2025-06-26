#pragma once

#include "editor/plugins/editor_plugin.h"
#include "scene/3d/voxel_gi.h"

class EditorFileDialog;
struct EditorProgress;
class HBoxContainer;

class VoxelGIEditorPlugin : public EditorPlugin {
	GDCLASS(VoxelGIEditorPlugin, EditorPlugin);

	VoxelGI *voxel_gi = nullptr;

	HBoxContainer *bake_hb = nullptr;
	Button *bake = nullptr;

	EditorFileDialog *probe_file = nullptr;

	static EditorProgress *tmp_progress;
	static void bake_func_begin();
	static bool bake_func_step(int p_progress, const String &p_description);
	static void bake_func_end();

	void _bake();
	void _voxel_gi_save_path_and_bake(const String &p_path);

protected:
	void _notification(int p_what);

public:
	virtual String get_plugin_name() const override { return "VoxelGI"; }
	bool has_main_screen() const override { return false; }
	virtual void edit(Object *p_object) override;
	virtual bool handles(Object *p_object) const override;
	virtual void make_visible(bool p_visible) override;

	VoxelGIEditorPlugin();
};

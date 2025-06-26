#pragma once

#include "editor/plugins/editor_plugin.h"

class Button;
class SkeletonIK3D;

class SkeletonIK3DEditorPlugin : public EditorPlugin {
	GDCLASS(SkeletonIK3DEditorPlugin, EditorPlugin);

	SkeletonIK3D *skeleton_ik = nullptr;

	Button *play_btn = nullptr;

	void _play();

public:
	virtual String get_plugin_name() const override { return "SkeletonIK3D"; }
	bool has_main_screen() const override { return false; }
	virtual void edit(Object *p_object) override;
	virtual bool handles(Object *p_object) const override;
	virtual void make_visible(bool p_visible) override;

	SkeletonIK3DEditorPlugin();
};

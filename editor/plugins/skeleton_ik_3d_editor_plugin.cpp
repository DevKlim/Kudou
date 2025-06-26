#include "skeleton_ik_3d_editor_plugin.h"

#include "editor/editor_node.h"
#include "editor/editor_string_names.h"
#include "scene/3d/skeleton_ik_3d.h"
#include "scene/gui/button.h"

void SkeletonIK3DEditorPlugin::_play() {
	if (!skeleton_ik) {
		return;
	}

	if (!skeleton_ik->get_parent_skeleton()) {
		return;
	}

	if (play_btn->is_pressed()) {
		skeleton_ik->start();
	} else {
		skeleton_ik->stop();
	}
}

void SkeletonIK3DEditorPlugin::edit(Object *p_object) {
	SkeletonIK3D *s = Object::cast_to<SkeletonIK3D>(p_object);
	if (!s) {
		return;
	}

	skeleton_ik = s;
	play_btn->set_pressed(skeleton_ik->is_running());
}

bool SkeletonIK3DEditorPlugin::handles(Object *p_object) const {
	return p_object->is_class("SkeletonIK3D");
}

void SkeletonIK3DEditorPlugin::make_visible(bool p_visible) {
	if (p_visible) {
		play_btn->show();
	} else {
		play_btn->hide();
	}
}

SkeletonIK3DEditorPlugin::SkeletonIK3DEditorPlugin() {
	play_btn = memnew(Button);
	play_btn->set_button_icon(EditorNode::get_singleton()->get_editor_theme()->get_icon(SNAME("Play"), EditorStringName(EditorIcons)));
	play_btn->set_text(TTR("Play IK"));
	play_btn->set_toggle_mode(true);
	play_btn->hide();
	play_btn->connect(SceneStringName(pressed), callable_mp(this, &SkeletonIK3DEditorPlugin::_play));
	add_control_to_container(CONTAINER_SPATIAL_EDITOR_MENU, play_btn);
	skeleton_ik = nullptr;
}

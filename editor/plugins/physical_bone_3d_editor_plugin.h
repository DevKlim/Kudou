#pragma once

#include "editor/plugins/editor_plugin.h"
#include "scene/gui/box_container.h"
#include "scene/gui/button.h"

class PhysicalBone3D;

class PhysicalBone3DEditor : public Object {
	GDCLASS(PhysicalBone3DEditor, Object);

	HBoxContainer *spatial_editor_hb = nullptr;
	Button *button_transform_joint = nullptr;

	PhysicalBone3D *selected = nullptr;

private:
	void _on_toggle_button_transform_joint(bool p_is_pressed);
	void _set_move_joint();

public:
	PhysicalBone3DEditor();

	void set_selected(PhysicalBone3D *p_pb);

	void hide();
	void show();
};

class PhysicalBone3DEditorPlugin : public EditorPlugin {
	GDCLASS(PhysicalBone3DEditorPlugin, EditorPlugin);

	PhysicalBone3D *selected = nullptr;
	PhysicalBone3DEditor physical_bone_editor;

public:
	virtual String get_plugin_name() const override { return "PhysicalBone3D"; }
	virtual bool handles(Object *p_object) const override { return p_object->is_class("PhysicalBone3D"); }
	virtual void make_visible(bool p_visible) override;
	virtual void edit(Object *p_node) override;
};

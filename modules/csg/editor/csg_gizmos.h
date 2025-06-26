#pragma once

#include "../csg_shape.h"

#include "editor/plugins/editor_plugin.h"
#include "editor/plugins/node_3d_editor_gizmos.h"
#include "scene/gui/control.h"

class AcceptDialog;
class Gizmo3DHelper;
class MenuButton;

class CSGShape3DGizmoPlugin : public EditorNode3DGizmoPlugin {
	GDCLASS(CSGShape3DGizmoPlugin, EditorNode3DGizmoPlugin);

	Ref<Gizmo3DHelper> helper;

public:
	virtual bool has_gizmo(Node3D *p_spatial) override;
	virtual String get_gizmo_name() const override;
	virtual int get_priority() const override;
	virtual bool is_selectable_when_hidden() const override;
	virtual void redraw(EditorNode3DGizmo *p_gizmo) override;

	virtual String get_handle_name(const EditorNode3DGizmo *p_gizmo, int p_id, bool p_secondary) const override;
	virtual Variant get_handle_value(const EditorNode3DGizmo *p_gizmo, int p_id, bool p_secondary) const override;
	void begin_handle_action(const EditorNode3DGizmo *p_gizmo, int p_id, bool p_secondary) override;
	virtual void set_handle(const EditorNode3DGizmo *p_gizmo, int p_id, bool p_secondary, Camera3D *p_camera, const Point2 &p_point) override;
	virtual void commit_handle(const EditorNode3DGizmo *p_gizmo, int p_id, bool p_secondary, const Variant &p_restore, bool p_cancel) override;

	CSGShape3DGizmoPlugin();
};

class CSGShapeEditor : public Control {
	GDCLASS(CSGShapeEditor, Control);

	enum Menu {
		MENU_OPTION_BAKE_MESH_INSTANCE,
		MENU_OPTION_BAKE_COLLISION_SHAPE,
	};

	CSGShape3D *node = nullptr;
	MenuButton *options = nullptr;
	AcceptDialog *err_dialog = nullptr;

	void _menu_option(int p_option);

	void _create_baked_mesh_instance();
	void _create_baked_collision_shape();

protected:
	void _node_removed(Node *p_node);

	void _notification(int p_what);

public:
	void edit(CSGShape3D *p_csg_shape);
	CSGShapeEditor();
};

class EditorPluginCSG : public EditorPlugin {
	GDCLASS(EditorPluginCSG, EditorPlugin);

	CSGShapeEditor *csg_shape_editor = nullptr;

public:
	virtual String get_plugin_name() const override { return "CSGShape3D"; }
	virtual void edit(Object *p_object) override;
	virtual bool handles(Object *p_object) const override;

	EditorPluginCSG();
};

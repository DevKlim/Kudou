#pragma once

#include "editor/plugins/node_3d_editor_gizmos.h"

class SoftBody3DGizmoPlugin : public EditorNode3DGizmoPlugin {
	GDCLASS(SoftBody3DGizmoPlugin, EditorNode3DGizmoPlugin);

public:
	bool has_gizmo(Node3D *p_spatial) override;
	String get_gizmo_name() const override;
	int get_priority() const override;
	bool is_selectable_when_hidden() const override;
	void redraw(EditorNode3DGizmo *p_gizmo) override;

	String get_handle_name(const EditorNode3DGizmo *p_gizmo, int p_id, bool p_secondary) const override;
	Variant get_handle_value(const EditorNode3DGizmo *p_gizmo, int p_id, bool p_secondary) const override;
	void commit_handle(const EditorNode3DGizmo *p_gizmo, int p_id, bool p_secondary, const Variant &p_restore, bool p_cancel = false) override;
	bool is_handle_highlighted(const EditorNode3DGizmo *p_gizmo, int p_id, bool p_secondary) const override;

	SoftBody3DGizmoPlugin();
};

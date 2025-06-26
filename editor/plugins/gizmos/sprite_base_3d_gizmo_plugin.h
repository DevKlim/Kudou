#pragma once

#include "editor/plugins/node_3d_editor_gizmos.h"

class SpriteBase3DGizmoPlugin : public EditorNode3DGizmoPlugin {
	GDCLASS(SpriteBase3DGizmoPlugin, EditorNode3DGizmoPlugin);

public:
	bool has_gizmo(Node3D *p_spatial) override;
	String get_gizmo_name() const override;
	int get_priority() const override;
	bool can_be_hidden() const override;
	void redraw(EditorNode3DGizmo *p_gizmo) override;
};

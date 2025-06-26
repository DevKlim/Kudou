#pragma once

#include "editor/plugins/node_3d_editor_gizmos.h"

class Marker3DGizmoPlugin : public EditorNode3DGizmoPlugin {
	GDCLASS(Marker3DGizmoPlugin, EditorNode3DGizmoPlugin);

	Ref<ArrayMesh> pos3d_mesh;

public:
	bool has_gizmo(Node3D *p_spatial) override;
	String get_gizmo_name() const override;
	int get_priority() const override;
	void redraw(EditorNode3DGizmo *p_gizmo) override;

	Marker3DGizmoPlugin();
};

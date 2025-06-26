#pragma once

#include "editor/plugins/node_3d_editor_gizmos.h"

class LightmapProbeGizmoPlugin : public EditorNode3DGizmoPlugin {
	GDCLASS(LightmapProbeGizmoPlugin, EditorNode3DGizmoPlugin);

	float probe_size = 0.4f;

public:
	bool has_gizmo(Node3D *p_spatial) override;
	String get_gizmo_name() const override;
	int get_priority() const override;
	void redraw(EditorNode3DGizmo *p_gizmo) override;

	LightmapProbeGizmoPlugin();
};

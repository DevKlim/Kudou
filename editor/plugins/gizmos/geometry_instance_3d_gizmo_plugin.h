#pragma once

#include "editor/plugins/node_3d_editor_gizmos.h"

class GeometryInstance3DGizmoPlugin : public EditorNode3DGizmoPlugin {
	GDCLASS(GeometryInstance3DGizmoPlugin, EditorNode3DGizmoPlugin);

public:
	virtual bool has_gizmo(Node3D *p_spatial) override;
	virtual String get_gizmo_name() const override;
	virtual int get_priority() const override;

	virtual void redraw(EditorNode3DGizmo *p_gizmo) override;
};

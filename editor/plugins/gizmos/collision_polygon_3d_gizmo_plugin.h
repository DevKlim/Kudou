#pragma once

#include "editor/plugins/node_3d_editor_gizmos.h"

class CollisionPolygon3DGizmoPlugin : public EditorNode3DGizmoPlugin {
	GDCLASS(CollisionPolygon3DGizmoPlugin, EditorNode3DGizmoPlugin);

	void create_collision_material(const String &p_name, float p_alpha);

public:
	bool has_gizmo(Node3D *p_spatial) override;
	String get_gizmo_name() const override;
	int get_priority() const override;
	void redraw(EditorNode3DGizmo *p_gizmo) override;
	CollisionPolygon3DGizmoPlugin();
};

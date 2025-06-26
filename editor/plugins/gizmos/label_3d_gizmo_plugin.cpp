#include "label_3d_gizmo_plugin.h"

#include "scene/3d/label_3d.h"

bool Label3DGizmoPlugin::has_gizmo(Node3D *p_spatial) {
	return Object::cast_to<Label3D>(p_spatial) != nullptr;
}

String Label3DGizmoPlugin::get_gizmo_name() const {
	return "Label3D";
}

int Label3DGizmoPlugin::get_priority() const {
	return -1;
}

bool Label3DGizmoPlugin::can_be_hidden() const {
	return false;
}

void Label3DGizmoPlugin::redraw(EditorNode3DGizmo *p_gizmo) {
	Label3D *label = Object::cast_to<Label3D>(p_gizmo->get_node_3d());

	p_gizmo->clear();

	Ref<TriangleMesh> tm = label->generate_triangle_mesh();
	if (tm.is_valid()) {
		p_gizmo->add_collision_triangles(tm);
	}
}

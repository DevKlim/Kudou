#include "sprite_base_3d_gizmo_plugin.h"

#include "scene/3d/sprite_3d.h"

bool SpriteBase3DGizmoPlugin::has_gizmo(Node3D *p_spatial) {
	return Object::cast_to<SpriteBase3D>(p_spatial) != nullptr;
}

String SpriteBase3DGizmoPlugin::get_gizmo_name() const {
	return "SpriteBase3D";
}

int SpriteBase3DGizmoPlugin::get_priority() const {
	return -1;
}

bool SpriteBase3DGizmoPlugin::can_be_hidden() const {
	return false;
}

void SpriteBase3DGizmoPlugin::redraw(EditorNode3DGizmo *p_gizmo) {
	SpriteBase3D *sprite_base = Object::cast_to<SpriteBase3D>(p_gizmo->get_node_3d());

	p_gizmo->clear();

	Ref<TriangleMesh> tm = sprite_base->generate_triangle_mesh();
	if (tm.is_valid()) {
		p_gizmo->add_collision_triangles(tm);
	}
}

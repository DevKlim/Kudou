#include "ray_cast_3d_gizmo_plugin.h"

#include "scene/3d/physics/ray_cast_3d.h"

RayCast3DGizmoPlugin::RayCast3DGizmoPlugin() {
	const Color gizmo_color = SceneTree::get_singleton()->get_debug_collisions_color();
	create_material("shape_material", gizmo_color);
	const float gizmo_value = gizmo_color.get_v();
	const Color gizmo_color_disabled = Color(gizmo_value, gizmo_value, gizmo_value, 0.65);
	create_material("shape_material_disabled", gizmo_color_disabled);
}

bool RayCast3DGizmoPlugin::has_gizmo(Node3D *p_spatial) {
	return Object::cast_to<RayCast3D>(p_spatial) != nullptr;
}

String RayCast3DGizmoPlugin::get_gizmo_name() const {
	return "RayCast3D";
}

int RayCast3DGizmoPlugin::get_priority() const {
	return -1;
}

void RayCast3DGizmoPlugin::redraw(EditorNode3DGizmo *p_gizmo) {
	RayCast3D *raycast = Object::cast_to<RayCast3D>(p_gizmo->get_node_3d());

	p_gizmo->clear();

	const Ref<StandardMaterial3D> material = raycast->is_enabled() ? raycast->get_debug_material() : get_material("shape_material_disabled");

	p_gizmo->add_lines(raycast->get_debug_line_vertices(), material);

	if (raycast->get_debug_shape_thickness() > 1) {
		p_gizmo->add_vertices(raycast->get_debug_shape_vertices(), material, Mesh::PRIMITIVE_TRIANGLE_STRIP);
	}

	p_gizmo->add_collision_segments(raycast->get_debug_line_vertices());
}

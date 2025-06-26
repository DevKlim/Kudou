#include "spring_arm_3d_gizmo_plugin.h"

#include "scene/3d/physics/spring_arm_3d.h"

void SpringArm3DGizmoPlugin::redraw(EditorNode3DGizmo *p_gizmo) {
	SpringArm3D *spring_arm = Object::cast_to<SpringArm3D>(p_gizmo->get_node_3d());

	p_gizmo->clear();

	Vector<Vector3> lines = {
		Vector3(),
		Vector3(0, 0, 1.0) * spring_arm->get_length()
	};

	Ref<StandardMaterial3D> material = get_material("shape_material", p_gizmo);

	p_gizmo->add_lines(lines, material);
	p_gizmo->add_collision_segments(lines);
}

SpringArm3DGizmoPlugin::SpringArm3DGizmoPlugin() {
	Color gizmo_color = SceneTree::get_singleton()->get_debug_collisions_color();
	create_material("shape_material", gizmo_color);
}

bool SpringArm3DGizmoPlugin::has_gizmo(Node3D *p_spatial) {
	return Object::cast_to<SpringArm3D>(p_spatial) != nullptr;
}

String SpringArm3DGizmoPlugin::get_gizmo_name() const {
	return "SpringArm3D";
}

int SpringArm3DGizmoPlugin::get_priority() const {
	return -1;
}

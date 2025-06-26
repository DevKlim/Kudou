#include "geometry_instance_3d_gizmo_plugin.h"

#include "editor/editor_settings.h"
#include "scene/3d/visual_instance_3d.h"

bool GeometryInstance3DGizmoPlugin::has_gizmo(Node3D *p_spatial) {
	return Object::cast_to<GeometryInstance3D>(p_spatial) != nullptr;
}

String GeometryInstance3DGizmoPlugin::get_gizmo_name() const {
	return "MeshInstance3DCustomAABB";
}

int GeometryInstance3DGizmoPlugin::get_priority() const {
	return -1;
}

void GeometryInstance3DGizmoPlugin::redraw(EditorNode3DGizmo *p_gizmo) {
	GeometryInstance3D *geometry = Object::cast_to<GeometryInstance3D>(p_gizmo->get_node_3d());

	p_gizmo->clear();

	if (p_gizmo->is_selected()) {
		AABB aabb = geometry->get_custom_aabb();

		Vector<Vector3> lines;
		for (int i = 0; i < 12; i++) {
			Vector3 a;
			Vector3 b;
			aabb.get_edge(i, a, b);

			lines.push_back(a);
			lines.push_back(b);
		}

		Ref<StandardMaterial3D> mat = memnew(StandardMaterial3D);
		mat->set_shading_mode(StandardMaterial3D::SHADING_MODE_UNSHADED);
		mat->set_flag(StandardMaterial3D::FLAG_DISABLE_FOG, true);
		const Color selection_box_color = EDITOR_GET("editors/3d_gizmos/gizmo_colors/aabb");
		mat->set_albedo(selection_box_color);
		mat->set_transparency(StandardMaterial3D::TRANSPARENCY_ALPHA);
		p_gizmo->add_lines(lines, mat);
	}
}

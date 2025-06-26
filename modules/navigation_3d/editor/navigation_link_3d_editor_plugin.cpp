#include "navigation_link_3d_editor_plugin.h"

#include "editor/plugins/node_3d_editor_plugin.h"
#include "scene/3d/navigation/navigation_link_3d.h"

void NavigationLink3DEditorPlugin::edit(Object *p_object) {
}

bool NavigationLink3DEditorPlugin::handles(Object *p_object) const {
	return Object::cast_to<NavigationLink3D>(p_object) != nullptr;
}

NavigationLink3DEditorPlugin::NavigationLink3DEditorPlugin() {
	gizmo_plugin.instantiate();
	Node3DEditor::get_singleton()->add_gizmo_plugin(gizmo_plugin);
}

#include "register_types.h"

#include "3d/godot_navigation_server_3d.h"

#ifndef DISABLE_DEPRECATED
#include "3d/navigation_mesh_generator.h"
#endif // DISABLE_DEPRECATED

#ifdef TOOLS_ENABLED
#include "editor/navigation_link_3d_editor_plugin.h"
#include "editor/navigation_obstacle_3d_editor_plugin.h"
#include "editor/navigation_region_3d_editor_plugin.h"
#endif

#include "core/config/engine.h"
#include "servers/navigation_server_3d.h"

#ifndef DISABLE_DEPRECATED
NavigationMeshGenerator *_nav_mesh_generator = nullptr;
#endif // DISABLE_DEPRECATED

NavigationServer3D *new_navigation_server_3d() {
	return memnew(GodotNavigationServer3D);
}

void initialize_navigation_3d_module(ModuleInitializationLevel p_level) {
	if (p_level == MODULE_INITIALIZATION_LEVEL_SERVERS) {
		NavigationServer3DManager::set_default_server(new_navigation_server_3d);

#ifndef DISABLE_DEPRECATED
		_nav_mesh_generator = memnew(NavigationMeshGenerator);
		GDREGISTER_CLASS(NavigationMeshGenerator);
		Engine::get_singleton()->add_singleton(Engine::Singleton("NavigationMeshGenerator", NavigationMeshGenerator::get_singleton()));
#endif // DISABLE_DEPRECATED
	}

#ifdef TOOLS_ENABLED
	if (p_level == MODULE_INITIALIZATION_LEVEL_EDITOR) {
		EditorPlugins::add_by_type<NavigationLink3DEditorPlugin>();
		EditorPlugins::add_by_type<NavigationRegion3DEditorPlugin>();
		EditorPlugins::add_by_type<NavigationObstacle3DEditorPlugin>();
	}
#endif
}

void uninitialize_navigation_3d_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SERVERS) {
		return;
	}

#ifndef DISABLE_DEPRECATED
	if (_nav_mesh_generator) {
		memdelete(_nav_mesh_generator);
	}
#endif // DISABLE_DEPRECATED
}

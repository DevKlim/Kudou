#include "register_types.h"

#include "2d/godot_navigation_server_2d.h"

#include "core/config/engine.h"
#include "servers/navigation_server_2d.h"

#ifdef TOOLS_ENABLED
#include "editor/navigation_link_2d_editor_plugin.h"
#include "editor/navigation_obstacle_2d_editor_plugin.h"
#include "editor/navigation_region_2d_editor_plugin.h"
#endif

NavigationServer2D *new_navigation_server_2d() {
	return memnew(GodotNavigationServer2D);
}

void initialize_navigation_2d_module(ModuleInitializationLevel p_level) {
	if (p_level == MODULE_INITIALIZATION_LEVEL_SERVERS) {
		NavigationServer2DManager::set_default_server(new_navigation_server_2d);
	}

#ifdef TOOLS_ENABLED
	if (p_level == MODULE_INITIALIZATION_LEVEL_EDITOR) {
		EditorPlugins::add_by_type<NavigationLink2DEditorPlugin>();
		EditorPlugins::add_by_type<NavigationRegion2DEditorPlugin>();
		EditorPlugins::add_by_type<NavigationObstacle2DEditorPlugin>();
	}
#endif
}

void uninitialize_navigation_2d_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SERVERS) {
		return;
	}
}

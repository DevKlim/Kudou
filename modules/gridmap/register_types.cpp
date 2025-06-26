#ifndef _3D_DISABLED

#include "register_types.h"

#include "grid_map.h"

#ifdef TOOLS_ENABLED
#include "editor/grid_map_editor_plugin.h"
#endif

#include "core/object/class_db.h"

void initialize_gridmap_module(ModuleInitializationLevel p_level) {
	if (p_level == MODULE_INITIALIZATION_LEVEL_SCENE) {
		GDREGISTER_CLASS(GridMap);
#ifndef NAVIGATION_3D_DISABLED
		GridMap::navmesh_parse_init();
#endif // NAVIGATION_3D_DISABLED
	}
#ifdef TOOLS_ENABLED
	if (p_level == MODULE_INITIALIZATION_LEVEL_EDITOR) {
		GDREGISTER_CLASS(GridMapEditorPlugin);
		EditorPlugins::add_by_type<GridMapEditorPlugin>();
	}
#endif
}

void uninitialize_gridmap_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
}

#endif // _3D_DISABLED

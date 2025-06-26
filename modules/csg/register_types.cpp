#include "register_types.h"

#include "csg_shape.h"

#ifdef TOOLS_ENABLED
#include "editor/csg_gizmos.h"
#endif

void initialize_csg_module(ModuleInitializationLevel p_level) {
	if (p_level == MODULE_INITIALIZATION_LEVEL_SCENE) {
		GDREGISTER_ABSTRACT_CLASS(CSGShape3D);
		GDREGISTER_ABSTRACT_CLASS(CSGPrimitive3D);
		GDREGISTER_CLASS(CSGMesh3D);
		GDREGISTER_CLASS(CSGSphere3D);
		GDREGISTER_CLASS(CSGBox3D);
		GDREGISTER_CLASS(CSGCylinder3D);
		GDREGISTER_CLASS(CSGTorus3D);
		GDREGISTER_CLASS(CSGPolygon3D);
		GDREGISTER_CLASS(CSGCombiner3D);
#ifndef NAVIGATION_3D_DISABLED
		CSGShape3D::navmesh_parse_init();
#endif // NAVIGATION_3D_DISABLED
	}
#ifdef TOOLS_ENABLED
	if (p_level == MODULE_INITIALIZATION_LEVEL_EDITOR) {
		EditorPlugins::add_by_type<EditorPluginCSG>();
	}
#endif
}

void uninitialize_csg_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
}

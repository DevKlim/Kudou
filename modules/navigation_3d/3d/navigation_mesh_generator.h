#pragma once

#include "scene/3d/navigation/navigation_region_3d.h"
#include "scene/resources/navigation_mesh.h"

class NavigationMeshSourceGeometryData3D;

class NavigationMeshGenerator : public Object {
	GDCLASS(NavigationMeshGenerator, Object);

	static NavigationMeshGenerator *singleton;

protected:
	static void _bind_methods();

public:
	static NavigationMeshGenerator *get_singleton();

	NavigationMeshGenerator();
	~NavigationMeshGenerator();

	void bake(const Ref<NavigationMesh> &p_navigation_mesh, Node *p_root_node);
	void clear(Ref<NavigationMesh> p_navigation_mesh);

	void parse_source_geometry_data(const Ref<NavigationMesh> &p_navigation_mesh, Ref<NavigationMeshSourceGeometryData3D> p_source_geometry_data, Node *p_root_node, const Callable &p_callback = Callable());
	void bake_from_source_geometry_data(Ref<NavigationMesh> p_navigation_mesh, const Ref<NavigationMeshSourceGeometryData3D> &p_source_geometry_data, const Callable &p_callback = Callable());
};

#pragma once

#include "scene/3d/visual_instance_3d.h"
#include "scene/resources/multimesh.h"

#ifndef NAVIGATION_3D_DISABLED
class NavigationMesh;
class NavigationMeshSourceGeometryData3D;
#endif // NAVIGATION_3D_DISABLED

class MultiMeshInstance3D : public GeometryInstance3D {
	GDCLASS(MultiMeshInstance3D, GeometryInstance3D);

	Ref<MultiMesh> multimesh;

	void _refresh_interpolated();

protected:
	virtual void _physics_interpolated_changed() override;
	static void _bind_methods();
	void _notification(int p_what);

public:
	void set_multimesh(const Ref<MultiMesh> &p_multimesh);
	Ref<MultiMesh> get_multimesh() const;

	Array get_meshes() const;

	virtual AABB get_aabb() const override;

private:
#ifndef NAVIGATION_3D_DISABLED
	static Callable _navmesh_source_geometry_parsing_callback;
	static RID _navmesh_source_geometry_parser;
#endif // NAVIGATION_3D_DISABLED

public:
#ifndef NAVIGATION_3D_DISABLED
	static void navmesh_parse_init();
	static void navmesh_parse_source_geometry(const Ref<NavigationMesh> &p_navigation_mesh, Ref<NavigationMeshSourceGeometryData3D> p_source_geometry_data, Node *p_node);
#endif // NAVIGATION_3D_DISABLED

	MultiMeshInstance3D();
	~MultiMeshInstance3D();
};

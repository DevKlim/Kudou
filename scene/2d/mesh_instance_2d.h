#pragma once

#include "scene/2d/node_2d.h"

class NavigationPolygon;
class NavigationMeshSourceGeometryData2D;

class MeshInstance2D : public Node2D {
	GDCLASS(MeshInstance2D, Node2D);

	Ref<Mesh> mesh;

	Ref<Texture2D> texture;

protected:
	void _notification(int p_what);
	static void _bind_methods();

public:
#ifdef DEBUG_ENABLED
	virtual Rect2 _edit_get_rect() const override;
	virtual bool _edit_use_rect() const override;
#endif // DEBUG_ENABLED

	void set_mesh(const Ref<Mesh> &p_mesh);
	Ref<Mesh> get_mesh() const;

	void set_texture(const Ref<Texture2D> &p_texture);
	Ref<Texture2D> get_texture() const;

private:
	static Callable _navmesh_source_geometry_parsing_callback;
	static RID _navmesh_source_geometry_parser;

public:
#ifndef NAVIGATION_2D_DISABLED
	static void navmesh_parse_init();
	static void navmesh_parse_source_geometry(const Ref<NavigationPolygon> &p_navigation_mesh, Ref<NavigationMeshSourceGeometryData2D> p_source_geometry_data, Node *p_node);
#endif // NAVIGATION_2D_DISABLED

	MeshInstance2D();
};

#pragma once

#include "scene/2d/node_2d.h"
#include "scene/resources/multimesh.h"

class NavigationPolygon;
class NavigationMeshSourceGeometryData2D;

class MultiMeshInstance2D : public Node2D {
	GDCLASS(MultiMeshInstance2D, Node2D);

	Ref<MultiMesh> multimesh;

	Ref<Texture2D> texture;

protected:
	void _notification(int p_what);
	static void _bind_methods();

public:
#ifdef DEBUG_ENABLED
	virtual Rect2 _edit_get_rect() const override;
#endif // DEBUG_ENABLED

	void set_multimesh(const Ref<MultiMesh> &p_multimesh);
	Ref<MultiMesh> get_multimesh() const;

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

	MultiMeshInstance2D();
	~MultiMeshInstance2D();
};

#pragma once

#include "core/io/resource.h"

#ifndef PHYSICS_2D_DISABLED
#include "servers/physics_server_2d.h"
#endif // PHYSICS_2D_DISABLED

class VisibleOnScreenNotifier2D;
class Viewport;
struct SpatialIndexer2D;

class World2D : public Resource {
	GDCLASS(World2D, Resource);

	RID canvas;
#ifndef NAVIGATION_2D_DISABLED
	mutable RID navigation_map;
#endif // NAVIGATION_2D_DISABLED
#ifndef PHYSICS_2D_DISABLED
	mutable RID space;
#endif // PHYSICS_2D_DISABLED

	HashSet<Viewport *> viewports;

protected:
	static void _bind_methods();
	friend class Viewport;

public:
	RID get_canvas() const;
#ifndef NAVIGATION_2D_DISABLED
	RID get_navigation_map() const;
#endif // NAVIGATION_2D_DISABLED

#ifndef PHYSICS_2D_DISABLED
	RID get_space() const;
	PhysicsDirectSpaceState2D *get_direct_space_state();
#endif // PHYSICS_2D_DISABLED

	void register_viewport(Viewport *p_viewport);
	void remove_viewport(Viewport *p_viewport);

	_FORCE_INLINE_ const HashSet<Viewport *> &get_viewports() { return viewports; }

	World2D();
	~World2D();
};

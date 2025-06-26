#pragma once

#include "scene/3d/node_3d.h"

class Marker3D : public Node3D {
	GDCLASS(Marker3D, Node3D);

	real_t gizmo_extents = 0.25;

protected:
	static void _bind_methods();

public:
	void set_gizmo_extents(real_t p_extents);
	real_t get_gizmo_extents() const;

	Marker3D();
};

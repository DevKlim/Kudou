#pragma once

#include "scene/resources/3d/shape_3d.h"

class ArrayMesh;

class WorldBoundaryShape3D : public Shape3D {
	GDCLASS(WorldBoundaryShape3D, Shape3D);
	Plane plane;

protected:
	static void _bind_methods();
	virtual void _update_shape() override;

public:
	void set_plane(const Plane &p_plane);
	const Plane &get_plane() const;

	virtual Vector<Vector3> get_debug_mesh_lines() const override;
	virtual Ref<ArrayMesh> get_debug_arraymesh_faces(const Color &p_modulate) const override;
	virtual real_t get_enclosing_radius() const override {
		// Should be infinite?
		return 0;
	}

	WorldBoundaryShape3D();
};

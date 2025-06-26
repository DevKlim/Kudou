#pragma once

#include "scene/resources/3d/shape_3d.h"

class ArrayMesh;

class CylinderShape3D : public Shape3D {
	GDCLASS(CylinderShape3D, Shape3D);
	float radius = 0.5;
	float height = 2.0;

protected:
	static void _bind_methods();
	virtual void _update_shape() override;

public:
	void set_radius(float p_radius);
	float get_radius() const;
	void set_height(float p_height);
	float get_height() const;

	virtual Vector<Vector3> get_debug_mesh_lines() const override;
	virtual Ref<ArrayMesh> get_debug_arraymesh_faces(const Color &p_modulate) const override;
	virtual real_t get_enclosing_radius() const override;

	CylinderShape3D();
};

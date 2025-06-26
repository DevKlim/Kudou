#pragma once

#include "scene/3d/visual_instance_3d.h"
#include "scene/resources/immediate_mesh.h"
class RootMotionView : public VisualInstance3D {
	GDCLASS(RootMotionView, VisualInstance3D);

public:
	Ref<ImmediateMesh> immediate;
	NodePath path;
	real_t cell_size = 1.0;
	real_t radius = 10.0;
	bool use_in_game = false;
	Color color = Color(0.5, 0.5, 1.0);
	bool first = true;
	bool zero_y = true;

	Ref<Material> immediate_material;

	Transform3D accumulated;

private:
	void _notification(int p_what);
	static void _bind_methods();

public:
	void set_animation_mixer(const NodePath &p_path);
	NodePath get_animation_mixer() const;

	void set_color(const Color &p_color);
	Color get_color() const;

	void set_cell_size(float p_size);
	float get_cell_size() const;

	void set_radius(float p_radius);
	float get_radius() const;

	void set_zero_y(bool p_zero_y);
	bool get_zero_y() const;

	virtual AABB get_aabb() const override;

	RootMotionView();
	~RootMotionView();
};

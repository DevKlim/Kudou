#pragma once

#include "scene/3d/node_3d.h"
#include "scene/3d/visual_instance_3d.h"
#include "scene/resources/3d/skin.h"

class ImporterMesh;

class ImporterMeshInstance3D : public Node3D {
	GDCLASS(ImporterMeshInstance3D, Node3D)

	Ref<ImporterMesh> mesh;
	Ref<Skin> skin;
	NodePath skeleton_path;
	Vector<Ref<Material>> surface_materials;
	uint32_t layer_mask = 1;
	GeometryInstance3D::ShadowCastingSetting shadow_casting_setting = GeometryInstance3D::SHADOW_CASTING_SETTING_ON;
	float visibility_range_begin = 0.0;
	float visibility_range_end = 0.0;
	float visibility_range_begin_margin = 0.0;
	float visibility_range_end_margin = 0.0;
	GeometryInstance3D::VisibilityRangeFadeMode visibility_range_fade_mode = GeometryInstance3D::VISIBILITY_RANGE_FADE_DISABLED;

protected:
	static void _bind_methods();

public:
	void set_mesh(const Ref<ImporterMesh> &p_mesh);
	Ref<ImporterMesh> get_mesh() const;

	void set_skin(const Ref<Skin> &p_skin);
	Ref<Skin> get_skin() const;

	void set_surface_material(int p_idx, const Ref<Material> &p_material);
	Ref<Material> get_surface_material(int p_idx) const;

	void set_skeleton_path(const NodePath &p_path);
	NodePath get_skeleton_path() const;

	void set_layer_mask(const uint32_t p_layer_mask);
	uint32_t get_layer_mask() const;

	void set_cast_shadows_setting(GeometryInstance3D::ShadowCastingSetting p_shadow_casting_setting);
	GeometryInstance3D::ShadowCastingSetting get_cast_shadows_setting() const;

	void set_visibility_range_begin(float p_dist);
	float get_visibility_range_begin() const;

	void set_visibility_range_end(float p_dist);
	float get_visibility_range_end() const;

	void set_visibility_range_begin_margin(float p_dist);
	float get_visibility_range_begin_margin() const;

	void set_visibility_range_end_margin(float p_dist);
	float get_visibility_range_end_margin() const;

	void set_visibility_range_fade_mode(GeometryInstance3D::VisibilityRangeFadeMode p_mode);
	GeometryInstance3D::VisibilityRangeFadeMode get_visibility_range_fade_mode() const;
};

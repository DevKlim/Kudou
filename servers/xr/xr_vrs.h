#pragma once

#include "core/object/class_db.h"
#include "core/object/object.h"
#include "core/templates/rid.h"
#include "core/variant/variant.h"

/* This is a helper class for generating stereoscopic VRS images */

class XRVRS : public Object {
	GDCLASS(XRVRS, Object);

private:
	float vrs_min_radius = 20.0;
	float vrs_strength = 1.0;
	Rect2i vrs_render_region;
	bool vrs_dirty = true;

	RID vrs_texture;
	Size2i target_size;
	PackedVector2Array eye_foci;

protected:
	static void _bind_methods();

public:
	~XRVRS();

	float get_vrs_min_radius() const;
	void set_vrs_min_radius(float p_vrs_min_radius);
	float get_vrs_strength() const;
	void set_vrs_strength(float p_vrs_strength);
	Rect2i get_vrs_render_region() const;
	void set_vrs_render_region(const Rect2i &p_vrs_render_region);

	RID make_vrs_texture(const Size2 &p_target_size, const PackedVector2Array &p_eye_foci);
};

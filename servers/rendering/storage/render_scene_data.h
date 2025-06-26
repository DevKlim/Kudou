#pragma once

#include "core/object/class_db.h"
#include "core/object/gdvirtual.gen.inc"
#include "core/object/object.h"
#include "core/object/script_language.h"

class RenderSceneData : public Object {
	GDCLASS(RenderSceneData, Object);

protected:
	static void _bind_methods();

public:
	virtual Transform3D get_cam_transform() const = 0;
	virtual Projection get_cam_projection() const = 0;

	virtual uint32_t get_view_count() const = 0;
	virtual Vector3 get_view_eye_offset(uint32_t p_view) const = 0;
	virtual Projection get_view_projection(uint32_t p_view) const = 0;

	virtual RID get_uniform_buffer() const = 0;
};

class RenderSceneDataExtension : public RenderSceneData {
	GDCLASS(RenderSceneDataExtension, RenderSceneData);

protected:
	static void _bind_methods();

public:
	virtual Transform3D get_cam_transform() const override;
	virtual Projection get_cam_projection() const override;

	virtual uint32_t get_view_count() const override;
	virtual Vector3 get_view_eye_offset(uint32_t p_view) const override;
	virtual Projection get_view_projection(uint32_t p_view) const override;

	virtual RID get_uniform_buffer() const override;

	GDVIRTUAL0RC(Transform3D, _get_cam_transform)
	GDVIRTUAL0RC(Projection, _get_cam_projection)

	GDVIRTUAL0RC(uint32_t, _get_view_count)
	GDVIRTUAL1RC(Vector3, _get_view_eye_offset, uint32_t)
	GDVIRTUAL1RC(Projection, _get_view_projection, uint32_t)

	GDVIRTUAL0RC(RID, _get_uniform_buffer)
};

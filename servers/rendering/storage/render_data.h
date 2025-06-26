#pragma once

#include "core/object/object.h"
#include "render_scene_buffers.h"
#include "render_scene_data.h"

class RenderData : public Object {
	GDCLASS(RenderData, Object);

protected:
	static void _bind_methods();

public:
	virtual Ref<RenderSceneBuffers> get_render_scene_buffers() const = 0;
	virtual RenderSceneData *get_render_scene_data() const = 0;

	virtual RID get_environment() const = 0;
	virtual RID get_camera_attributes() const = 0;
};

class RenderDataExtension : public RenderData {
	GDCLASS(RenderDataExtension, RenderData);

protected:
	static void _bind_methods();

	virtual Ref<RenderSceneBuffers> get_render_scene_buffers() const override;
	virtual RenderSceneData *get_render_scene_data() const override;

	virtual RID get_environment() const override;
	virtual RID get_camera_attributes() const override;

	GDVIRTUAL0RC(Ref<RenderSceneBuffers>, _get_render_scene_buffers)
	GDVIRTUAL0RC(RenderSceneData *, _get_render_scene_data)
	GDVIRTUAL0RC(RID, _get_environment)
	GDVIRTUAL0RC(RID, _get_camera_attributes)
};

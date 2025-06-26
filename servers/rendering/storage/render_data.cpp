#include "render_data.h"

void RenderData::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_render_scene_buffers"), &RenderData::get_render_scene_buffers);
	ClassDB::bind_method(D_METHOD("get_render_scene_data"), &RenderData::get_render_scene_data);
	ClassDB::bind_method(D_METHOD("get_environment"), &RenderData::get_environment);
	ClassDB::bind_method(D_METHOD("get_camera_attributes"), &RenderData::get_camera_attributes);
}

void RenderDataExtension::_bind_methods() {
	GDVIRTUAL_BIND(_get_render_scene_buffers);
	GDVIRTUAL_BIND(_get_render_scene_data)
	GDVIRTUAL_BIND(_get_environment)
	GDVIRTUAL_BIND(_get_camera_attributes)
}

Ref<RenderSceneBuffers> RenderDataExtension::get_render_scene_buffers() const {
	Ref<RenderSceneBuffers> ret;
	GDVIRTUAL_CALL(_get_render_scene_buffers, ret);
	return ret;
}

RenderSceneData *RenderDataExtension::get_render_scene_data() const {
	RenderSceneData *ret = nullptr;
	GDVIRTUAL_CALL(_get_render_scene_data, ret);
	return ret;
}

RID RenderDataExtension::get_environment() const {
	RID ret;
	GDVIRTUAL_CALL(_get_environment, ret);
	return ret;
}

RID RenderDataExtension::get_camera_attributes() const {
	RID ret;
	GDVIRTUAL_CALL(_get_camera_attributes, ret);
	return ret;
}

#include "render_scene_data.h"

void RenderSceneData::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_cam_transform"), &RenderSceneData::get_cam_transform);
	ClassDB::bind_method(D_METHOD("get_cam_projection"), &RenderSceneData::get_cam_projection);

	ClassDB::bind_method(D_METHOD("get_view_count"), &RenderSceneData::get_view_count);
	ClassDB::bind_method(D_METHOD("get_view_eye_offset", "view"), &RenderSceneData::get_view_eye_offset);
	ClassDB::bind_method(D_METHOD("get_view_projection", "view"), &RenderSceneData::get_view_projection);

	ClassDB::bind_method(D_METHOD("get_uniform_buffer"), &RenderSceneData::get_uniform_buffer);
}

void RenderSceneDataExtension::_bind_methods() {
	GDVIRTUAL_BIND(_get_cam_transform);
	GDVIRTUAL_BIND(_get_cam_projection);
	GDVIRTUAL_BIND(_get_view_count);
	GDVIRTUAL_BIND(_get_view_eye_offset, "view");
	GDVIRTUAL_BIND(_get_view_projection, "view");

	GDVIRTUAL_BIND(_get_uniform_buffer);
}

Transform3D RenderSceneDataExtension::get_cam_transform() const {
	Transform3D ret;
	GDVIRTUAL_CALL(_get_cam_transform, ret);
	return ret;
}

Projection RenderSceneDataExtension::get_cam_projection() const {
	Projection ret;
	GDVIRTUAL_CALL(_get_cam_projection, ret);
	return ret;
}

uint32_t RenderSceneDataExtension::get_view_count() const {
	uint32_t ret = 0;
	GDVIRTUAL_CALL(_get_view_count, ret);
	return ret;
}

Vector3 RenderSceneDataExtension::get_view_eye_offset(uint32_t p_view) const {
	Vector3 ret;
	GDVIRTUAL_CALL(_get_view_eye_offset, p_view, ret);
	return ret;
}

Projection RenderSceneDataExtension::get_view_projection(uint32_t p_view) const {
	Projection ret;
	GDVIRTUAL_CALL(_get_view_projection, p_view, ret);
	return ret;
}

RID RenderSceneDataExtension::get_uniform_buffer() const {
	RID ret;
	GDVIRTUAL_CALL(_get_uniform_buffer, ret);
	return ret;
}

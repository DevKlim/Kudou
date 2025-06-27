#pragma once

#include "scene/3d/node_3d.h"

#include <openxr/openxr.h>

class OpenXRRenderModel : public Node3D {
	GDCLASS(OpenXRRenderModel, Node3D);

private:
	RID render_model;
	Node3D *scene = nullptr;
	HashMap<String, Node3D *> animatable_nodes;

	void _load_render_model_scene();
	void _on_render_model_top_level_path_changed(RID p_render_model);

protected:
	static void _bind_methods();

	void _notification(int p_what);

public:
	virtual PackedStringArray get_configuration_warnings() const override;

	RID get_render_model() const;
	void set_render_model(RID p_render_model);

	String get_top_level_path() const;
};

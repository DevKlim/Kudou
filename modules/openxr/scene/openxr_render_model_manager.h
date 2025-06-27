#pragma once

#include "openxr_render_model.h"

#include "scene/3d/node_3d.h"
#include "scene/resources/packed_scene.h"
#include "servers/xr/xr_positional_tracker.h"

#include <openxr/openxr.h>

class OpenXRRenderModelManager : public Node3D {
	GDCLASS(OpenXRRenderModelManager, Node3D);

public:
	enum RenderModelTracker {
		RENDER_MODEL_TRACKER_ANY,
		RENDER_MODEL_TRACKER_NONE_SET,
		RENDER_MODEL_TRACKER_LEFT_HAND,
		RENDER_MODEL_TRACKER_RIGHT_HAND,
	};

	virtual PackedStringArray get_configuration_warnings() const override;

	void set_tracker(RenderModelTracker p_tracker);
	RenderModelTracker get_tracker() const;

	void set_make_local_to_pose(const String &p_action);
	String get_make_local_to_pose() const;

private:
	HashMap<RID, Node3D *> render_models;
	Node3D *container = nullptr;

	bool is_dirty = false;
	RenderModelTracker tracker = RENDER_MODEL_TRACKER_ANY;
	String make_local_to_pose;

	// cached values
	Ref<XRPositionalTracker> positional_tracker;
	XrPath xr_path = XR_NULL_PATH;

	bool _has_filters();
	void _on_render_model_added(RID p_render_model);
	void _on_render_model_removed(RID p_render_model);
	void _on_render_model_top_level_path_changed(RID p_path);
	void _update_models();

protected:
	static void _bind_methods();

	void _notification(int p_what);
};

VARIANT_ENUM_CAST(OpenXRRenderModelManager::RenderModelTracker);

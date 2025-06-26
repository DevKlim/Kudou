#pragma once

#include "scene/3d/mesh_instance_3d.h"
#include "scene/3d/node_3d.h"

/**
	The XRFaceModifier3D node drives the blend shapes of a MeshInstance3D
	with facial expressions from an XRFaceTracking instance.

	The blend shapes provided by the mesh are interrogated, and used to
	deduce an optimal mapping from the Unified Expressions blend shapes
	provided by the	XRFaceTracking instance to drive the face.
 */

class XRFaceModifier3D : public Node3D {
	GDCLASS(XRFaceModifier3D, Node3D);

private:
	StringName tracker_name = "/user/face_tracker";
	NodePath target;

	// Map from XRFaceTracker blend shape index to mesh blend shape index.
	RBMap<int, int> blend_mapping;

	MeshInstance3D *get_mesh_instance() const;
	void _get_blend_data();
	void _update_face_blends() const;

protected:
	static void _bind_methods();

public:
	void set_face_tracker(const StringName &p_tracker_name);
	StringName get_face_tracker() const;

	void set_target(const NodePath &p_target);
	NodePath get_target() const;

	void _notification(int p_what);
};

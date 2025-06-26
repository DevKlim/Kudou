#pragma once

#include "scene/3d/node_3d.h"
#include "scene/3d/skeleton_3d.h"

#include <openxr/openxr.h>

class OpenXRAPI;
class OpenXRHandTrackingExtension;

class OpenXRHand : public Node3D {
	GDCLASS(OpenXRHand, Node3D);

public:
	enum Hands { // Deprecated, need to change this to OpenXRInterface::Hands.
		HAND_LEFT,
		HAND_RIGHT,
		HAND_MAX
	};

	enum MotionRange { // Deprecated, need to change this to OpenXRInterface::HandMotionRange.
		MOTION_RANGE_UNOBSTRUCTED,
		MOTION_RANGE_CONFORM_TO_CONTROLLER,
		MOTION_RANGE_MAX
	};

	enum SkeletonRig {
		SKELETON_RIG_OPENXR,
		SKELETON_RIG_HUMANOID,
		SKELETON_RIG_MAX
	};

	enum BoneUpdate {
		BONE_UPDATE_FULL,
		BONE_UPDATE_ROTATION_ONLY,
		BONE_UPDATE_MAX
	};

private:
	struct JointData {
		int bone = -1;
		int parent_joint = -1;
	};

	OpenXRAPI *openxr_api = nullptr;
	OpenXRHandTrackingExtension *hand_tracking_ext = nullptr;

	Hands hand = HAND_LEFT;
	MotionRange motion_range = MOTION_RANGE_UNOBSTRUCTED;
	NodePath hand_skeleton;
	SkeletonRig skeleton_rig = SKELETON_RIG_OPENXR;
	BoneUpdate bone_update = BONE_UPDATE_FULL;

	JointData joints[XR_HAND_JOINT_COUNT_EXT];

	void _set_motion_range();

	Skeleton3D *get_skeleton();
	void _get_joint_data();
	void _update_skeleton();

protected:
	static void _bind_methods();

public:
	OpenXRHand();

	void set_hand(Hands p_hand);
	Hands get_hand() const;

	void set_motion_range(MotionRange p_motion_range);
	MotionRange get_motion_range() const;

	void set_hand_skeleton(const NodePath &p_hand_skeleton);
	NodePath get_hand_skeleton() const;

	void set_skeleton_rig(SkeletonRig p_skeleton_rig);
	SkeletonRig get_skeleton_rig() const;

	void set_bone_update(BoneUpdate p_bone_update);
	BoneUpdate get_bone_update() const;

	void _notification(int p_what);
};

VARIANT_ENUM_CAST(OpenXRHand::Hands)
VARIANT_ENUM_CAST(OpenXRHand::MotionRange)
VARIANT_ENUM_CAST(OpenXRHand::SkeletonRig)
VARIANT_ENUM_CAST(OpenXRHand::BoneUpdate)

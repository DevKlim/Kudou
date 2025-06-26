#include "modifier_bone_target_3d.h"

void ModifierBoneTarget3D::_validate_bone_names() {
	// Prior bone name.
	if (!bone_name.is_empty()) {
		set_bone_name(bone_name);
	} else if (bone != -1) {
		set_bone(bone);
	}
}

void ModifierBoneTarget3D::set_bone_name(const String &p_bone_name) {
	bone_name = p_bone_name;
	Skeleton3D *sk = get_skeleton();
	if (sk) {
		set_bone(sk->find_bone(bone_name));
	}
}

String ModifierBoneTarget3D::get_bone_name() const {
	return bone_name;
}

void ModifierBoneTarget3D::set_bone(int p_bone) {
	bone = p_bone;
	Skeleton3D *sk = get_skeleton();
	if (sk) {
		if (bone <= -1 || bone >= sk->get_bone_count()) {
			WARN_PRINT("Bone index out of range!");
			bone = -1;
		} else {
			bone_name = sk->get_bone_name(bone);
		}
	}
}

int ModifierBoneTarget3D::get_bone() const {
	return bone;
}

void ModifierBoneTarget3D::_validate_property(PropertyInfo &p_property) const {
	if (p_property.name == "influence") {
		p_property.usage = PROPERTY_USAGE_READ_ONLY;
	}
}

void ModifierBoneTarget3D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_bone_name", "bone_name"), &ModifierBoneTarget3D::set_bone_name);
	ClassDB::bind_method(D_METHOD("get_bone_name"), &ModifierBoneTarget3D::get_bone_name);
	ClassDB::bind_method(D_METHOD("set_bone", "bone"), &ModifierBoneTarget3D::set_bone);
	ClassDB::bind_method(D_METHOD("get_bone"), &ModifierBoneTarget3D::get_bone);

	ADD_PROPERTY(PropertyInfo(Variant::STRING, "bone_name", PROPERTY_HINT_ENUM_SUGGESTION, ""), "set_bone_name", "get_bone_name");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "bone", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NO_EDITOR), "set_bone", "get_bone");
}

void ModifierBoneTarget3D::_process_modification(double p_delta) {
	if (!is_inside_tree()) {
		return;
	}

	Skeleton3D *skeleton = get_skeleton();
	if (!skeleton || bone < 0 || bone >= skeleton->get_bone_count()) {
		return;
	}

	set_transform(skeleton->get_bone_global_pose(bone));
}

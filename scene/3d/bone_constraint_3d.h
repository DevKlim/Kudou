#pragma once

#include "scene/3d/skeleton_modifier_3d.h"

class BoneConstraint3D : public SkeletonModifier3D {
	GDCLASS(BoneConstraint3D, SkeletonModifier3D);

public:
	struct BoneConstraint3DSetting {
		float amount = 1.0;

		String apply_bone_name;
		int apply_bone = -1;

		String reference_bone_name;
		int reference_bone = -1;
	};

protected:
	Vector<BoneConstraint3DSetting *> settings;

	bool _get(const StringName &p_path, Variant &r_ret) const;
	bool _set(const StringName &p_path, const Variant &p_value);

	// Define get_property_list() instead of _get_property_list()
	// to merge child class properties into parent class array inspector.
	void get_property_list(List<PropertyInfo> *p_list) const; // Will be called by child classes.

	virtual void _validate_bone_names() override;
	static void _bind_methods();

	virtual void _process_modification(double p_delta) override;

	virtual void _process_constraint(int p_index, Skeleton3D *p_skeleton, int p_apply_bone, int p_reference_bone, float p_amount);
	virtual void _validate_setting(int p_index);

public:
	void set_amount(int p_index, float p_amount);
	float get_amount(int p_index) const;

	void set_apply_bone_name(int p_index, const String &p_bone_name);
	String get_apply_bone_name(int p_index) const;
	void set_apply_bone(int p_index, int p_bone);
	int get_apply_bone(int p_index) const;

	void set_reference_bone_name(int p_index, const String &p_bone_name);
	String get_reference_bone_name(int p_index) const;
	void set_reference_bone(int p_index, int p_bone);
	int get_reference_bone(int p_index) const;

	void set_setting_count(int p_count);
	int get_setting_count() const;

	void clear_settings();

	static double symmetrize_angle(double p_angle); // Helper to make angle 0->TAU become -PI->PI.
	static double get_roll_angle(const Quaternion &p_rotation, const Vector3 &p_roll_axis);

	~BoneConstraint3D();
};

#pragma once

#include "scene/3d/skeleton_modifier_3d.h"

class ModifierBoneTarget3D : public SkeletonModifier3D {
	GDCLASS(ModifierBoneTarget3D, SkeletonModifier3D);

	String bone_name;
	int bone = -1;

protected:
	void _validate_property(PropertyInfo &p_property) const;
	virtual void _validate_bone_names() override;
	static void _bind_methods();
	virtual void _process_modification(double p_delta) override;

public:
	void set_bone_name(const String &p_bone_name);
	String get_bone_name() const;
	void set_bone(int p_bone);
	int get_bone() const;
};

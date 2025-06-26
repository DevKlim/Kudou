#pragma once

#include "scene/3d/spring_bone_collision_3d.h"

class SpringBoneCollisionPlane3D : public SpringBoneCollision3D {
	GDCLASS(SpringBoneCollisionPlane3D, SpringBoneCollision3D);

protected:
	virtual Vector3 _collide(const Transform3D &p_center, float p_bone_radius, float p_bone_length, const Vector3 &p_current) const override;
};

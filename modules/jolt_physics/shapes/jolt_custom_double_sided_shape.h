#pragma once

#include "jolt_custom_decorated_shape.h"
#include "jolt_custom_shape_type.h"

class JoltCustomDoubleSidedShapeSettings final : public JoltCustomDecoratedShapeSettings {
public:
	bool back_face_collision = false;

	JoltCustomDoubleSidedShapeSettings() = default;

	JoltCustomDoubleSidedShapeSettings(const ShapeSettings *p_inner_settings, bool p_back_face_collision) :
			JoltCustomDecoratedShapeSettings(p_inner_settings), back_face_collision(p_back_face_collision) {}

	JoltCustomDoubleSidedShapeSettings(const JPH::Shape *p_inner_shape, bool p_back_face_collision) :
			JoltCustomDecoratedShapeSettings(p_inner_shape), back_face_collision(p_back_face_collision) {}

	virtual JPH::Shape::ShapeResult Create() const override;
};

class JoltCustomDoubleSidedShape final : public JoltCustomDecoratedShape {
	bool back_face_collision = false;

public:
	static void register_type();

	JoltCustomDoubleSidedShape() :
			JoltCustomDecoratedShape(JoltCustomShapeSubType::DOUBLE_SIDED) {}

	JoltCustomDoubleSidedShape(const JoltCustomDoubleSidedShapeSettings &p_settings, JPH::Shape::ShapeResult &p_result) :
			JoltCustomDecoratedShape(JoltCustomShapeSubType::DOUBLE_SIDED, p_settings, p_result), back_face_collision(p_settings.back_face_collision) {
		if (!p_result.HasError()) {
			p_result.Set(this);
		}
	}

	JoltCustomDoubleSidedShape(const JPH::Shape *p_inner_shape, bool p_back_face_collision) :
			JoltCustomDecoratedShape(JoltCustomShapeSubType::DOUBLE_SIDED, p_inner_shape), back_face_collision(p_back_face_collision) {}

	virtual void CastRay(const JPH::RayCast &p_ray, const JPH::RayCastSettings &p_ray_cast_settings, const JPH::SubShapeIDCreator &p_sub_shape_id_creator, JPH::CastRayCollector &p_collector, const JPH::ShapeFilter &p_shape_filter = JPH::ShapeFilter()) const override;

	bool should_collide_with_back_faces() const { return back_face_collision; }
};

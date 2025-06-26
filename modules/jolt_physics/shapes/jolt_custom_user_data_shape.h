#pragma once

#include "jolt_custom_decorated_shape.h"
#include "jolt_custom_shape_type.h"

class JoltCustomUserDataShapeSettings final : public JoltCustomDecoratedShapeSettings {
public:
	using JoltCustomDecoratedShapeSettings::JoltCustomDecoratedShapeSettings;

	virtual ShapeResult Create() const override;
};

class JoltCustomUserDataShape final : public JoltCustomDecoratedShape {
public:
	static void register_type();

	JoltCustomUserDataShape() :
			JoltCustomDecoratedShape(JoltCustomShapeSubType::OVERRIDE_USER_DATA) {}

	JoltCustomUserDataShape(const JoltCustomUserDataShapeSettings &p_settings, ShapeResult &p_result) :
			JoltCustomDecoratedShape(JoltCustomShapeSubType::OVERRIDE_USER_DATA, p_settings, p_result) {
		if (!p_result.HasError()) {
			p_result.Set(this);
		}
	}

	virtual JPH::uint64 GetSubShapeUserData(const JPH::SubShapeID &p_sub_shape_id) const override { return GetUserData(); }
};

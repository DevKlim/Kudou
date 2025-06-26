#pragma once

#include "core/object/ref_counted.h"
#include "servers/navigation/navigation_utilities.h"

class NavigationPathQueryResult2D : public RefCounted {
	GDCLASS(NavigationPathQueryResult2D, RefCounted);

	Vector<Vector2> path;
	Vector<int32_t> path_types;
	TypedArray<RID> path_rids;
	Vector<int64_t> path_owner_ids;
	float path_length = 0.0;

protected:
	static void _bind_methods();

public:
	enum PathSegmentType {
		PATH_SEGMENT_TYPE_REGION = NavigationUtilities::PathSegmentType::PATH_SEGMENT_TYPE_REGION,
		PATH_SEGMENT_TYPE_LINK = NavigationUtilities::PathSegmentType::PATH_SEGMENT_TYPE_LINK,
	};

	void set_path(const Vector<Vector2> &p_path);
	const Vector<Vector2> &get_path() const;

	void set_path_types(const Vector<int32_t> &p_path_types);
	const Vector<int32_t> &get_path_types() const;

	void set_path_rids(const TypedArray<RID> &p_path_rids);
	TypedArray<RID> get_path_rids() const;

	void set_path_owner_ids(const Vector<int64_t> &p_path_owner_ids);
	const Vector<int64_t> &get_path_owner_ids() const;

	void set_path_length(float p_length);
	float get_path_length() const;

	void reset();

	void set_data(const LocalVector<Vector2> &p_path, const LocalVector<int32_t> &p_path_types, const LocalVector<RID> &p_path_rids, const LocalVector<int64_t> &p_path_owner_ids);
};

VARIANT_ENUM_CAST(NavigationPathQueryResult2D::PathSegmentType);

#include "fbx_state.h"

void FBXState::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_allow_geometry_helper_nodes"), &FBXState::get_allow_geometry_helper_nodes);
	ClassDB::bind_method(D_METHOD("set_allow_geometry_helper_nodes", "allow"), &FBXState::set_allow_geometry_helper_nodes);

	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "allow_geometry_helper_nodes"), "set_allow_geometry_helper_nodes", "get_allow_geometry_helper_nodes");
}

bool FBXState::get_allow_geometry_helper_nodes() {
	return allow_geometry_helper_nodes;
}

void FBXState::set_allow_geometry_helper_nodes(bool p_allow_geometry_helper_nodes) {
	allow_geometry_helper_nodes = p_allow_geometry_helper_nodes;
}

#pragma once

#include <stdint.h>

class Node3D;
class Node;
struct Transform3D;
class SceneTreeFTI;

class SceneTreeFTITests {
	SceneTreeFTI &_fti;

	void debug_verify_failed(const Node3D *p_node, const Transform3D &p_test);

public:
	void update_dirty_nodes(Node *p_node, uint32_t p_current_half_frame, float p_interpolation_fraction, bool p_active, const Transform3D *p_parent_global_xform = nullptr, int p_depth = 0);
	void frame_update(Node *p_root, uint32_t p_half_frame, float p_interpolation_fraction);

	SceneTreeFTITests(SceneTreeFTI &p_fti);
};

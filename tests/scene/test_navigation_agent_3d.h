#pragma once

#include "scene/3d/navigation/navigation_agent_3d.h"
#include "scene/3d/node_3d.h"
#include "scene/main/window.h"

#include "tests/test_macros.h"

namespace TestNavigationAgent3D {

TEST_SUITE("[Navigation3D]") {
	TEST_CASE("[SceneTree][NavigationAgent3D] New agent should have valid RID") {
		NavigationAgent3D *agent_node = memnew(NavigationAgent3D);
		CHECK(agent_node->get_rid().is_valid());
		memdelete(agent_node);
	}

	TEST_CASE("[SceneTree][NavigationAgent3D] New agent should attach to default map") {
		Node3D *node_3d = memnew(Node3D);
		SceneTree::get_singleton()->get_root()->add_child(node_3d);

		NavigationAgent3D *agent_node = memnew(NavigationAgent3D);

		// agent should not be attached to any map when outside of tree
		CHECK_FALSE(agent_node->get_navigation_map().is_valid());

		SUBCASE("Agent should attach to default map when it enters the tree") {
			node_3d->add_child(agent_node);
			CHECK(agent_node->get_navigation_map().is_valid());
			CHECK(agent_node->get_navigation_map() == node_3d->get_world_3d()->get_navigation_map());
		}

		memdelete(agent_node);
		memdelete(node_3d);
	}
}

} //namespace TestNavigationAgent3D

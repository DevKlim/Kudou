#pragma once

#include "scene/2d/navigation/navigation_agent_2d.h"
#include "scene/2d/node_2d.h"
#include "scene/main/window.h"
#include "scene/resources/world_2d.h"

#include "tests/test_macros.h"

namespace TestNavigationAgent2D {

TEST_SUITE("[Navigation2D]") {
	TEST_CASE("[SceneTree][NavigationAgent2D] New agent should have valid RID") {
		NavigationAgent2D *agent_node = memnew(NavigationAgent2D);
		CHECK(agent_node->get_rid().is_valid());
		memdelete(agent_node);
	}

	TEST_CASE("[SceneTree][NavigationAgent2D] New agent should attach to default map") {
		Node2D *node_2d = memnew(Node2D);
		SceneTree::get_singleton()->get_root()->add_child(node_2d);

		NavigationAgent2D *agent_node = memnew(NavigationAgent2D);

		// agent should not be attached to any map when outside of tree
		CHECK_FALSE(agent_node->get_navigation_map().is_valid());

		SUBCASE("Agent should attach to default map when it enters the tree") {
			node_2d->add_child(agent_node);
			CHECK(agent_node->get_navigation_map().is_valid());
			CHECK(agent_node->get_navigation_map() == node_2d->get_world_2d()->get_navigation_map());
		}

		memdelete(agent_node);
		memdelete(node_2d);
	}
}

} //namespace TestNavigationAgent2D

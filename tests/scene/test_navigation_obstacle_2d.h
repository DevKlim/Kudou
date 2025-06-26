#pragma once

#include "scene/2d/navigation/navigation_obstacle_2d.h"
#include "scene/main/window.h"

#include "tests/test_macros.h"

namespace TestNavigationObstacle2D {

TEST_SUITE("[Navigation2D]") {
	TEST_CASE("[SceneTree][NavigationObstacle2D] New obstacle should have valid RID") {
		NavigationObstacle2D *obstacle_node = memnew(NavigationObstacle2D);
		CHECK(obstacle_node->get_rid().is_valid());
		memdelete(obstacle_node);
	}

	TEST_CASE("[SceneTree][NavigationObstacle2D] New obstacle should attach to default map") {
		Node2D *node_2d = memnew(Node2D);
		SceneTree::get_singleton()->get_root()->add_child(node_2d);

		NavigationObstacle2D *obstacle_node = memnew(NavigationObstacle2D);
		// obstacle should not be attached to any map when outside of tree
		CHECK_FALSE(obstacle_node->get_navigation_map().is_valid());

		SUBCASE("Obstacle should attach to default map when it enters the tree") {
			node_2d->add_child(obstacle_node);
			CHECK(obstacle_node->get_navigation_map().is_valid());
			CHECK(obstacle_node->get_navigation_map() == node_2d->get_world_2d()->get_navigation_map());
		}

		memdelete(obstacle_node);
		memdelete(node_2d);
	}
}

} //namespace TestNavigationObstacle2D

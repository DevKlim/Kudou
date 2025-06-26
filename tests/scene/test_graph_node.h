#pragma once

#include "scene/gui/graph_node.h"
#include "scene/main/window.h"

#include "tests/test_macros.h"

namespace TestGraphNode {

TEST_CASE("[GraphNode][SceneTree]") {
	SUBCASE("[GraphNode] Graph Node only child on delete should not cause error.") {
		// Setup.
		GraphNode *test_node = memnew(GraphNode);
		test_node->set_name("Graph Node");
		Control *test_child = memnew(Control);
		test_child->set_name("child");
		test_node->add_child(test_child);

		// Test.
		test_node->remove_child(test_child);
		CHECK(test_node->get_child_count(false) == 0);

		memdelete(test_child);
		memdelete(test_node);
	}
}

} // namespace TestGraphNode

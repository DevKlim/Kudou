#pragma once

#include "scene/2d/navigation/navigation_region_2d.h"
#include "scene/main/window.h"

#include "tests/test_macros.h"

namespace TestNavigationRegion2D {

TEST_SUITE("[Navigation2D]") {
	TEST_CASE("[SceneTree][NavigationRegion2D] New region should have valid RID") {
		NavigationRegion2D *region_node = memnew(NavigationRegion2D);
		CHECK(region_node->get_rid().is_valid());
		memdelete(region_node);
	}
}

} //namespace TestNavigationRegion2D

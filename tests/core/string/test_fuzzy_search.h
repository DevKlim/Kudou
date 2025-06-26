#pragma once

#include "core/string/fuzzy_search.h"
#include "tests/test_macros.h"

namespace TestFuzzySearch {

struct FuzzySearchTestCase {
	String query;
	String expected;
};

// Ideally each of these test queries should represent a different aspect, and potentially bottleneck, of the search process.
const FuzzySearchTestCase test_cases[] = {
	// Short query, many matches, few adjacent characters
	{ "///gd", "./menu/hud/hud.gd" },
	// Filename match with typo
	{ "sm.png", "./entity/blood_sword/sam.png" },
	// Multipart filename word matches
	{ "ham ", "./entity/game_trap/ha_missed_me.wav" },
	// Single word token matches
	{ "push background", "./entity/background_zone1/background/push.png" },
	// Long token matches
	{ "background_freighter background png", "./entity/background_freighter/background/background.png" },
	// Many matches, many short tokens
	{ "menu menu characters wav", "./menu/menu/characters/smoker/0.wav" },
	// Maximize total matches
	{ "entity gd", "./entity/entity_man.gd" }
};

Vector<String> load_test_data() {
	Ref<FileAccess> fp = FileAccess::open(TestUtils::get_data_path("fuzzy_search/project_dir_tree.txt"), FileAccess::READ);
	REQUIRE(fp.is_valid());
	return fp->get_as_utf8_string().split("\n");
}

TEST_CASE("[FuzzySearch] Test fuzzy search results") {
	FuzzySearch search;
	Vector<FuzzySearchResult> results;
	Vector<String> targets = load_test_data();

	for (FuzzySearchTestCase test_case : test_cases) {
		search.set_query(test_case.query);
		search.search_all(targets, results);
		CHECK_GT(results.size(), 0);
		CHECK_EQ(results[0].target, test_case.expected);
	}
}

} //namespace TestFuzzySearch

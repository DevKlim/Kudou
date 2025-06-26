#pragma once

#include "gdscript_test_runner.h"

#include "tests/test_macros.h"

namespace GDScriptTests {

enum TestType {
	TEST_TOKENIZER,
	TEST_TOKENIZER_BUFFER,
	TEST_PARSER,
	TEST_COMPILER,
	TEST_BYTECODE,
};

void test(TestType p_type);

} // namespace GDScriptTests

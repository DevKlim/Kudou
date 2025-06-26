#define DOCTEST_CONFIG_IMPLEMENT
#include "test_macros.h"

HashMap<String, TestFunc> *test_commands = nullptr;

int register_test_command(String p_command, TestFunc p_function) {
	if (!test_commands) {
		test_commands = new HashMap<String, TestFunc>;
	}
	test_commands->insert(p_command, p_function);
	return 0;
}

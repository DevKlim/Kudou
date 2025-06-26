#include "test_jsonrpc.h"

#include "core/io/json.h"

namespace TestJSONRPC {

void check_error_code(const Dictionary &p_dict, const JSONRPC::ErrorCode &p_code) {
	CHECK(p_dict["jsonrpc"] == "2.0");
	REQUIRE(p_dict.has("error"));
	const Dictionary &err_body = p_dict["error"];
	const int &code = err_body["code"];
	CHECK(code == p_code);
}

void check_invalid(const Dictionary &p_dict) {
	check_error_code(p_dict, JSONRPC::INVALID_REQUEST);
}

void check_invalid_string(const String &p_str) {
	JSON json;
	REQUIRE(json.parse(p_str) == OK);
	const Dictionary &dict = json.get_data();
	check_invalid(dict);
}

String TestClassJSONRPC::something(const String &p_in) {
	return p_in + ", please";
}

void test_process_action(const Variant &p_in, const Variant &p_expected, bool p_process_array_elements) {
	TestClassJSONRPC json_rpc = TestClassJSONRPC();
	const Variant &observed = json_rpc.process_action(p_in, p_process_array_elements);
	CHECK(observed == p_expected);
}

void test_process_string(const String &p_in, const String &p_expected) {
	TestClassJSONRPC json_rpc = TestClassJSONRPC();
	const String &out_str = json_rpc.process_string(p_in);
	CHECK(out_str == p_expected);
}

void check_error_no_method(const Dictionary &p_dict) {
	check_error_code(p_dict, JSONRPC::METHOD_NOT_FOUND);
}

void test_process_action_bad_method(const Dictionary &p_in) {
	TestClassJSONRPC json_rpc = TestClassJSONRPC();
	const Dictionary &out_dict = json_rpc.process_action(p_in);
	check_error_no_method(out_dict);
}

void test_no_response(const Variant &p_in) {
	TestClassJSONRPC json_rpc = TestClassJSONRPC();
	const Variant &res = json_rpc.process_action(p_in, true);
	CHECK(res.get_type() == Variant::NIL);
}

} // namespace TestJSONRPC

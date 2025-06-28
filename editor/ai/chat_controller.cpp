#include "chat_controller.h"
#include "core/io/json.h"

void ChatController::_bind_methods() {
	ClassDB::bind_method(D_METHOD("_http_request_completed", "result", "response_code", "headers", "body"), &ChatController::_http_request_completed);
	ADD_SIGNAL(MethodInfo(SNAME("message_received"), PropertyInfo(Variant::STRING, "message")));
}

void ChatController::_http_request_completed(int p_result, int p_response_code, const PackedStringArray &p_headers, const PackedByteArray &p_body) {
	if (p_result != HTTPRequest::RESULT_SUCCESS || p_response_code != 200) {
		ERR_PRINT("HTTP Request Failed: " + itos(p_response_code));
		emit_signal(SNAME("message_received"), "Error: Failed to get response from LLM.");
		return;
	}

	String response_body = String::utf8((const char *)p_body.ptr(), p_body.size());

	Ref<JSON> json = memnew(JSON);
	Error err = json->parse(response_body);
	if (err != OK) {
		ERR_PRINT("JSON Parse Error: " + response_body);
		emit_signal(SNAME("message_received"), "Error: Invalid response from LLM.");
		return;
	}

	Dictionary parsed_response = json->get_data();

	if (parsed_response.has("candidates")) {
		Array candidates = parsed_response["candidates"];
		if (candidates.size() > 0) {
			Dictionary candidate = candidates[0];
			if (candidate.has("content")) {
				Dictionary content = candidate["content"];
				if (content.has("parts")) {
					Array parts = content["parts"];
					if (parts.size() > 0) {
						Dictionary part = parts[0];
						if (part.has("text")) {
							String text = part["text"];
							emit_signal(SNAME("message_received"), text);
							return;
						}
					}
				}
			}
		}
	}
	emit_signal(SNAME("message_received"), "Error: Unexpected response format from LLM.");
}

void ChatController::send_message(const String &p_message) {
	if (base_url.is_empty() || api_key.is_empty() || model.is_empty()) {
		ERR_PRINT("ChatController not configured. Set base_url, api_key, and model.");
		emit_signal(SNAME("message_received"), "Error: Chat service not configured.");
		return;
	}

	String url = base_url + "/models/" + model + ":generateContent?key=" + api_key;

	Dictionary content_part;
	content_part["text"] = p_message;

	Array parts_array;
	parts_array.append(content_part);

	Dictionary content_obj;
	content_obj["parts"] = parts_array;

	Array contents_array;
	contents_array.append(content_obj);

	Dictionary request_body_dict;
	request_body_dict["contents"] = contents_array;

	String request_body_json = JSON::stringify(request_body_dict);

	PackedStringArray headers;
	headers.append("Content-Type: application/json");

	http_request->request(url, headers, HTTPClient::METHOD_POST, request_body_json);
}

void ChatController::set_api_key(const String &p_api_key) {
	api_key = p_api_key;
}

void ChatController::set_model(const String &p_model) {
	model = p_model;
}

void ChatController::set_base_url(const String &p_base_url) {
	base_url = p_base_url;
}

ChatController::ChatController() {
	http_request = memnew(HTTPRequest);
	http_request->connect("request_completed", callable_mp(this, &ChatController::_http_request_completed));
}

ChatController::~ChatController() {
	// The HTTPRequest node is a child of the plugin, so it will be freed automatically.
	// We don't need to memdelete it here.
}
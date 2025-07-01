#include "kudou_chat_controller.h"

#include "core/io/json.h"

void KudouChatController::_bind_methods() {
	ADD_SIGNAL(MethodInfo(SNAME("message_received"), PropertyInfo(Variant::STRING, "message")));
}

void KudouChatController::_notification(int p_what) {
	if (p_what == NOTIFICATION_READY) {
		http_request = memnew(HTTPRequest);
		add_child(http_request);
		http_request->connect("request_completed", callable_mp(this, &KudouChatController::_http_request_completed));
	}
}

void KudouChatController::_http_request_completed(int p_result, int p_response_code, const PackedStringArray &p_headers, const PackedByteArray &p_body) {
	if (p_result != HTTPRequest::RESULT_SUCCESS) {
		ERR_PRINT("HTTP Request Failed with error: " + itos(p_result));
		emit_signal(SNAME("message_received"), "Error: Failed to connect to LLM service.");
		return;
	}

	if (p_response_code < 200 || p_response_code >= 300) {
		ERR_PRINT("HTTP Request Failed with code: " + itos(p_response_code));
		String response_body = String::utf8((const char *)p_body.ptr(), p_body.size());
		ERR_PRINT("Response Body: " + response_body);
		emit_signal(SNAME("message_received"), "Error: LLM service returned status " + itos(p_response_code));
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

void KudouChatController::send_message(const String &p_message) {
	if (!http_request) {
		ERR_PRINT("HTTPRequest node not ready.");
		return;
	}

	if (base_url.is_empty() || api_key.is_empty() || model.is_empty()) {
		ERR_PRINT("KudouChatController not configured. Set base_url, api_key, and model.");
		emit_signal(SNAME("message_received"), "Error: Chat service not configured in Editor Settings -> Kudou.");
		return;
	}

	String url = base_url.path_join("models").path_join(model + ":generateContent") + "?key=" + api_key;

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

	http_request->cancel_request();
	Error err = http_request->request(url, headers, HTTPClient::METHOD_POST, request_body_json);
	if (err != OK) {
		ERR_PRINT("HTTPRequest->request failed with error: " + itos(err));
		emit_signal(SNAME("message_received"), "Error: Could not send request to LLM.");
	}
}

void KudouChatController::set_api_key(const String &p_api_key) {
	api_key = p_api_key;
}

void KudouChatController::set_model(const String &p_model) {
	model = p_model;
}

void KudouChatController::set_base_url(const String &p_base_url) {
	base_url = p_base_url;
}

KudouChatController::KudouChatController() {
}

KudouChatController::~KudouChatController() {
}
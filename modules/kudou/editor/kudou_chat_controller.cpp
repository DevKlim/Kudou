#include "kudou_chat_controller.h"

#include "core/io/json.h"
#include "core/io/file_access.h"
#include "servers/display_server.h"

void KudouChatController::_bind_methods() {
	ADD_SIGNAL(MethodInfo(SNAME("message_received"), PropertyInfo(Variant::STRING, "message")));
	ADD_SIGNAL(MethodInfo(SNAME("request_finished")));
}

void KudouChatController::_notification(int p_what) {
	if (p_what == NOTIFICATION_READY) {
		http_request = memnew(HTTPRequest);
		add_child(http_request);
		http_request->connect("request_completed", callable_mp(this, &KudouChatController::_http_request_completed));
        webchat_options.push_back("AiStudio");
	}
}

void KudouChatController::_http_request_completed(int p_result, int p_response_code, const PackedStringArray &p_headers, const PackedByteArray &p_body) {
	if (p_result != HTTPRequest::RESULT_SUCCESS) {
		ERR_PRINT(vformat("HTTP Request Failed with error: %d", p_result));
		emit_signal(SNAME("message_received"), "Error: Failed to connect to LLM service.");
		emit_signal(SNAME("request_finished"));
		return;
	}

	if (p_response_code < 200 || p_response_code >= 300) {
		ERR_PRINT(vformat("HTTP Request Failed with code: %d", p_response_code));
		String response_body = String::utf8((const char *)p_body.ptr(), p_body.size());
		ERR_PRINT("Response Body: " + response_body);
		emit_signal(SNAME("message_received"), vformat("Error: LLM service returned status %d", p_response_code));
		emit_signal(SNAME("request_finished"));
		return;
	}

	String response_body = String::utf8((const char *)p_body.ptr(), p_body.size());

	Ref<JSON> json = memnew(JSON);
	Error err = json->parse(response_body);
	if (err != OK) {
		ERR_PRINT("JSON Parse Error: " + response_body);
		emit_signal(SNAME("message_received"), "Error: Invalid response from LLM.");
		emit_signal(SNAME("request_finished"));
		return;
	}

	Dictionary parsed_response = json->get_data();

	// Safely parse the nested JSON structure with type checks
	if (parsed_response.has("candidates") && parsed_response["candidates"].get_type() == Variant::ARRAY) {
		Array candidates = parsed_response["candidates"];
		if (candidates.size() > 0 && candidates[0].get_type() == Variant::DICTIONARY) {
			Dictionary candidate = candidates[0];
			if (candidate.has("content") && candidate["content"].get_type() == Variant::DICTIONARY) {
				Dictionary content = candidate["content"];
				if (content.has("parts") && content["parts"].get_type() == Variant::ARRAY) {
					Array parts = content["parts"];
					if (parts.size() > 0 && parts[0].get_type() == Variant::DICTIONARY) {
						Dictionary part = parts[0];
						if (part.has("text") && part["text"].get_type() == Variant::STRING) {
							String text = part["text"];
							emit_signal(SNAME("message_received"), text);
							emit_signal(SNAME("request_finished"));
							return;
						}
					}
				}
			}
		}
	}
	emit_signal(SNAME("message_received"), "Error: Unexpected response format from LLM.");
	emit_signal(SNAME("request_finished"));
}

void KudouChatController::_emit_message_deferred(const String &p_message) {
	emit_signal(SNAME("message_received"), p_message);
	emit_signal(SNAME("request_finished"));
}

void KudouChatController::send_message(const String &p_message, const String &p_webchat) {
	if (!http_request) {
		ERR_PRINT("HTTPRequest node not ready.");
		callable_mp(this, &KudouChatController::_emit_message_deferred).call_deferred("Error: HTTPRequest node not ready.");
		return;
	}

	if (base_url.is_empty() || api_key.is_empty() || model.is_empty()) {
		ERR_PRINT("KudouChatController not configured. Set base_url, api_key, and model.");
		callable_mp(this, &KudouChatController::_emit_message_deferred).call_deferred("Error: Chat service not configured in Editor Settings -> Kudou.");
		return;
	}

    String url;
    if (p_webchat == "AiStudio") {
        url = base_url + "/v1beta/models/" + model + ":generateContent?key=" + api_key;
    } else {
        url = base_url + "/models/" + model + ":generateContent?key=" + api_key;
    }

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
		ERR_PRINT(vformat("HTTPRequest->request failed with error: %d", err));
		callable_mp(this, &KudouChatController::_emit_message_deferred).call_deferred("Error: Could not send request to LLM.");
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

void KudouChatController::set_webchat_options(const PackedStringArray &p_options) {
    webchat_options = p_options;
}

PackedStringArray KudouChatController::get_webchat_options() const {
    return webchat_options;
}

String KudouChatController::format_prompt(const String &p_prompt, const PackedStringArray &p_file_paths) {
    String formatted_prompt = p_prompt;
    for (int i = 0; i < p_file_paths.size(); i++) {
        String file_path = p_file_paths[i];
        Ref<FileAccess> file = FileAccess::open(file_path, FileAccess::READ);
        if (file.is_valid()) {
            String content = file->get_as_text();
            String file_extension = file_path.get_extension();
            formatted_prompt += vformat("<file path = \"%s\"><![CDATA[```%s%s```]]></file>", file_path.get_file(), file_extension, content);
        }
    }
    return formatted_prompt;
}

void KudouChatController::copy_to_clipboard(const String &p_text) {
    DisplayServer::get_singleton()->clipboard_set(p_text);
}

KudouChatController::KudouChatController() {
}

KudouChatController::~KudouChatController() {
}

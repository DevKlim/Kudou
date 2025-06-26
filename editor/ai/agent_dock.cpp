#include "agent_dock.h"
#include "prompts.h"

#include "core/io/json.h"
#include "editor/editor_string_names.hh"
#include "editor/themes/editor_scale.h"
#include "scene/gui/button.h"
#include "scene/gui/line_edit.h"
#include "scene/gui/rich_text_label.h"

void AgentDock::_on_chat_input_submitted(const String &p_text) {
	if (p_text.strip_edges().is_empty()) {
		return;
	}
	_send_prompt_to_api(p_text);
}

void AgentDock::_on_send_button_pressed() {
	_on_chat_input_submitted(chat_input->get_text());
}

void AgentDock::_on_http_request_completed(int p_result, int p_response_code, const PackedStringArray &p_headers, const PackedByteArray &p_body) {
	if (p_result != HTTPRequest::RESULT_SUCCESS) {
		_append_message("Kudou", TTR("Error: Could not connect to the API."));
		_set_ui_enabled(true);
		return;
	}

	String response_body_str;
	response_body_str.parse_utf8((const char *)p_body.ptr(), p_body.size());

	Variant json_response = JSON::parse_string(response_body_str);

	if (json_response.get_type() == Variant::NIL) {
		_append_message("Kudou", TTR("Error: Could not parse API response."));
		_set_ui_enabled(true);
		return;
	}

	Dictionary response_dict = json_response;
	// This part is specific to OpenAI's API format. It will need to be adapted for other services.
	if (response_dict.has("choices") && ((Array)response_dict["choices"]).size() > 0) {
		Dictionary choice = ((Array)response_dict["choices"])[0];
		if (choice.has("message")) {
			Dictionary message = choice["message"];
			if (message.has("content")) {
				_append_message("Kudou", message["content"]);
			}
		}
	} else if (response_dict.has("error")) {
		Dictionary error = response_dict["error"];
		_append_message("Kudou", TTR("API Error: ") + (String)error["message"]);
	} else {
		_append_message("Kudou", TTR("Error: Received an unexpected response format from the API."));
	}

	_set_ui_enabled(true);
}

void AgentDock::_send_prompt_to_api(const String &p_text) {
	_append_message("You", p_text);
	_set_ui_enabled(false);

	String api_key = api_key_input->get_text();
	if (api_key.is_empty()) {
		_append_message("Kudou", TTR("Error: API Key is not set."));
		_set_ui_enabled(true);
		return;
	}

	PackedStringArray headers;
	headers.push_back("Content-Type: application/json");
	headers.push_back("Authorization: Bearer " + api_key);

	String payload = _prepare_payload();
	http_request->request("https://api.openai.com/v1/chat/completions", headers, HTTPClient::METHOD_POST, payload);

	chat_history->add_text("\n[color=gray]" + TTR("Kudou is thinking...") + "[/color]");
}

String AgentDock::_prepare_payload() {
	// Construct the JSON payload for the API.
	Dictionary payload_dict;
	payload_dict["model"] = "gpt-4-turbo-preview"; // Or any other model.

	Array messages;
	// Add the system prompt.
	Dictionary system_message;
	system_message["role"] = "system";
	// For now, file_structure is a placeholder. In the future, we'll populate it dynamically.
	system_message["content"] = String(KudouPrompts::SYSTEM_PROMPT).replace("{file_structure}", "File structure not yet implemented.");
	messages.push_back(system_message);

	// Add the conversation history.
	for (int i = 0; i < conversation_history.size(); i++) {
		messages.push_back(conversation_history[i]);
	}

	payload_dict["messages"] = messages;

	return JSON::stringify(payload_dict);
}

void AgentDock::_append_message(const String &p_role, const String &p_message) {
	// Add to UI.
	String color = (p_role == "You") ? "green" : "aqua";
	chat_history->add_text(vformat("[b][color=%s]%s:[/color][/b] %s\n\n", color, p_role, p_message));

	// Add to internal history if it's from the user or the final AI response.
	if (p_role == "You") {
		Dictionary new_message;
		new_message["role"] = "user";
		new_message["content"] = p_message;
		conversation_history.push_back(new_message);
	} else if (p_role == "Kudou") { // Assuming "Kudou" is the final AI response role for history.
		Dictionary new_message;
		new_message["role"] = "assistant";
		new_message["content"] = p_message;
		conversation_history.push_back(new_message);
	}
}

void AgentDock::_set_ui_enabled(bool p_enabled) {
	chat_input->set_editable(p_enabled);
	send_button->set_disabled(!p_enabled);
	if (p_enabled) {
		chat_input->clear();
		chat_input->grab_focus();
	}
}

void AgentDock::_bind_methods() {
}

AgentDock::AgentDock() {
	set_name(TTRC("Agent"));
	add_theme_constant_override("separation", 4 * EDSCALE);

	// HTTP request node for API communication.
	http_request = memnew(HTTPRequest);
	add_child(http_request);
	http_request->connect("request_completed", callable_mp(this, &AgentDock::_on_http_request_completed));

	// API Key input area.
	HBoxContainer *api_hbox = memnew(HBoxContainer);
	add_child(api_hbox);
	api_key_input = memnew(LineEdit);
	api_key_input->set_placeholder(TTR("API Key (e.g., OpenAI, stored locally)"));
	api_key_input->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	api_key_input->set_secret(true);
	api_hbox->add_child(api_key_input);

	// Chat history display.
	chat_history = memnew(RichTextLabel);
	chat_history->set_v_size_flags(Control::SIZE_EXPAND_FILL);
	chat_history->set_use_bbcode(true);
	chat_history->set_selection_enabled(true);
	chat_history->set_focus_mode(FOCUS_CLICK);
	add_child(chat_history);

	// Container for chat input and send button.
	HBoxContainer *chat_hbox = memnew(HBoxContainer);
	add_child(chat_hbox);

	chat_input = memnew(LineEdit);
	chat_input->set_placeholder(TTR("Enter a prompt..."));
	chat_input->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	chat_hbox->add_child(chat_input);
	chat_input->connect("text_submitted", callable_mp(this, &AgentDock::_on_chat_input_submitted));

	send_button = memnew(Button);
	send_button->set_text(TTR("Send"));
	chat_hbox->add_child(send_button);
	send_button->connect("pressed", callable_mp(this, &AgentDock::_on_send_button_pressed));
}

AgentDock::~AgentDock() {
	// Destructor remains the same.
}
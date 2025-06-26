#ifndef AGENT_DOCK_H
#define AGENT_DOCK_H

#include "core/io/http_request.h"
#include "scene/gui/box_container.h"

class Button;
class LineEdit;
class RichTextLabel;

class AgentDock : public VBoxContainer {
	GDCLASS(AgentDock, VBoxContainer);

private:
	RichTextLabel *chat_history = nullptr;
	LineEdit *api_key_input = nullptr;
	LineEdit *chat_input = nullptr;
	Button *send_button = nullptr;
	HTTPRequest *http_request = nullptr;

	Vector<Dictionary> conversation_history;

	void _on_chat_input_submitted(const String &p_text);
	void _on_send_button_pressed();
	void _on_http_request_completed(int p_result, int p_response_code, const PackedStringArray &p_headers, const PackedByteArray &p_body);

	void _send_prompt_to_api(const String &p_text);
	String _prepare_payload();
	void _append_message(const String &p_role, const String &p_message);
	void _set_ui_enabled(bool p_enabled);

protected:
	static void _bind_methods();

public:
	AgentDock();
	~AgentDock();
};

#endif // AGENT_DOCK_H
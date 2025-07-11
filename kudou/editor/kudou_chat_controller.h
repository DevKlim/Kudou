#ifndef KUDOU_CHAT_CONTROLLER_H
#define KUDOU_CHAT_CONTROLLER_H

#include "scene/main/http_request.h"
#include "scene/main/node.h"

class KudouChatController : public Node {
	GDCLASS(KudouChatController, Node);

public:
	// Constructor and Destructor
	KudouChatController();
	~KudouChatController();

	// Properties (with getters and setters)
	void set_api_key(const String &p_api_key);
	String get_api_key() const;

	void set_model(const String &p_model);
	String get_model() const;

	void set_base_url(const String &p_base_url);
	String get_base_url() const;

    void set_webchat_options(const PackedStringArray &p_options);
    PackedStringArray get_webchat_options() const;

	// Public Methods
	void send_message(const String &p_message, const String &p_webchat);
    String format_prompt(const String &p_prompt, const PackedStringArray &p_file_paths);
    void copy_to_clipboard(const String &p_text);

protected:
	// Godot's lifecycle and binding methods
	static void _bind_methods();
	void _notification(int p_what);
	void _http_request_completed(int p_result, int p_response_code, const PackedStringArray &p_headers, const PackedByteArray &p_body);

private:
	// Member variables
	String api_key;
	String model;
	String base_url;
    PackedStringArray webchat_options;
	HTTPRequest *http_request = nullptr;

	// Internal helper methods
	void _emit_message_deferred(const String &p_message);
};

#endif // KUDOU_CHAT_CONTROLLER_H'''
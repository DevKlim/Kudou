#ifndef CHAT_CONTROLLER_H
#define CHAT_CONTROLLER_H

#include "core/object/object.h"
#include "core/string/ustring.h"
#include "scene/main/http_request.h"

class ChatController : public Object {
	GDCLASS(ChatController, Object);

private:
	String api_key;
	String model;
	String base_url;

protected:
	static void _bind_methods();
	void _http_request_completed(int p_result, int p_response_code, const PackedStringArray &p_headers, const PackedByteArray &p_body);

public:
	HTTPRequest *http_request = nullptr;

	void send_message(const String &p_message);
	void set_api_key(const String &p_api_key);
	void set_model(const String &p_model);
	void set_base_url(const String &p_base_url);

	ChatController();
	~ChatController();
};

#endif // CHAT_CONTROLLER_H
#ifndef KUDOU_CHAT_CONTROLLER_H
#define KUDOU_CHAT_CONTROLLER_H

#include "scene/main/http_request.h"
#include "scene/main/node.h"

class KudouChatController : public Node {
	GDCLASS(KudouChatController, Node);

private:
	String api_key;
	String model;
	String base_url;
	HTTPRequest *http_request = nullptr;

protected:
	static void _bind_methods();
	void _notification(int p_what);
	void _http_request_completed(int p_result, int p_response_code, const PackedStringArray &p_headers, const PackedByteArray &p_body);

public:
	void send_message(const String &p_message);
	void set_api_key(const String &p_api_key);
	void set_model(const String &p_model);
	void set_base_url(const String &p_base_url);

	KudouChatController();
	~KudouChatController();
};

#endif // KUDOU_CHAT_CONTROLLER_H
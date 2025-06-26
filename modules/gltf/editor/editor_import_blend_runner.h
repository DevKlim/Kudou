#pragma once

#include "core/io/http_client.h"
#include "core/os/os.h"
#include "scene/main/node.h"
#include "scene/main/timer.h"

class EditorImportBlendRunner : public Node {
	GDCLASS(EditorImportBlendRunner, Node);

	static EditorImportBlendRunner *singleton;

	Timer *kill_timer;
	void _resources_reimported(const PackedStringArray &p_files);
	void _kill_blender();
	void _notification(int p_what);
	bool _extract_error_message_xml(const Vector<uint8_t> &p_response_data, String &r_error_message);

protected:
	int rpc_port = 0;
	OS::ProcessID blender_pid = 0;
	Error start_blender(const String &p_python_script, bool p_blocking);
	Error do_import_direct(const Dictionary &p_options);
	Error do_import_rpc(const Dictionary &p_options);

public:
	static EditorImportBlendRunner *get_singleton() { return singleton; }

	bool is_running() { return blender_pid != 0 && OS::get_singleton()->is_process_running(blender_pid); }
	bool is_using_rpc() { return rpc_port != 0; }
	Error do_import(const Dictionary &p_options);
	HTTPClient::Status connect_blender_rpc(const Ref<HTTPClient> &p_client, int p_timeout_usecs);

	EditorImportBlendRunner();
};

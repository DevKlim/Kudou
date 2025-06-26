#pragma once

#include "gdscript_language_protocol.h"

#include "editor/plugins/editor_plugin.h"

class GDScriptLanguageServer : public EditorPlugin {
	GDCLASS(GDScriptLanguageServer, EditorPlugin);

	GDScriptLanguageProtocol protocol;

	Thread thread;
	bool thread_running = false;
	bool started = false;
	bool use_thread = false;
	String host = "127.0.0.1";
	int port = 6005;
	int poll_limit_usec = 100000;
	static void thread_main(void *p_userdata);

private:
	void _notification(int p_what);

public:
	static int port_override;
	GDScriptLanguageServer();
	void start();
	void stop();
};

void register_lsp_types();

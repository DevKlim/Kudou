#include "gdscript_language_server.h"

#include "core/os/os.h"
#include "editor/editor_log.h"
#include "editor/editor_node.h"
#include "editor/editor_settings.h"

int GDScriptLanguageServer::port_override = -1;

GDScriptLanguageServer::GDScriptLanguageServer() {
	// TODO: Move to editor_settings.cpp
	_EDITOR_DEF("network/language_server/remote_host", host);
	_EDITOR_DEF("network/language_server/remote_port", port);
	_EDITOR_DEF("network/language_server/enable_smart_resolve", true);
	_EDITOR_DEF("network/language_server/show_native_symbols_in_editor", false);
	_EDITOR_DEF("network/language_server/use_thread", use_thread);
	_EDITOR_DEF("network/language_server/poll_limit_usec", poll_limit_usec);
}

void GDScriptLanguageServer::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_ENTER_TREE: {
			start();
		} break;

		case NOTIFICATION_EXIT_TREE: {
			stop();
		} break;

		case NOTIFICATION_INTERNAL_PROCESS: {
			if (started && !use_thread) {
				protocol.poll(poll_limit_usec);
			}
		} break;

		case EditorSettings::NOTIFICATION_EDITOR_SETTINGS_CHANGED: {
			if (!EditorSettings::get_singleton()->check_changed_settings_in_group("network/language_server")) {
				break;
			}

			String remote_host = String(_EDITOR_GET("network/language_server/remote_host"));
			int remote_port = (GDScriptLanguageServer::port_override > -1) ? GDScriptLanguageServer::port_override : (int)_EDITOR_GET("network/language_server/remote_port");
			bool remote_use_thread = (bool)_EDITOR_GET("network/language_server/use_thread");
			int remote_poll_limit = (int)_EDITOR_GET("network/language_server/poll_limit_usec");
			if (remote_host != host || remote_port != port || remote_use_thread != use_thread || remote_poll_limit != poll_limit_usec) {
				stop();
				start();
			}
		} break;
	}
}

void GDScriptLanguageServer::thread_main(void *p_userdata) {
	set_current_thread_safe_for_nodes(true);
	GDScriptLanguageServer *self = static_cast<GDScriptLanguageServer *>(p_userdata);
	while (self->thread_running) {
		// Poll 20 times per second
		self->protocol.poll(self->poll_limit_usec);
		OS::get_singleton()->delay_usec(50000);
	}
}

void GDScriptLanguageServer::start() {
	host = String(_EDITOR_GET("network/language_server/remote_host"));
	port = (GDScriptLanguageServer::port_override > -1) ? GDScriptLanguageServer::port_override : (int)_EDITOR_GET("network/language_server/remote_port");
	use_thread = (bool)_EDITOR_GET("network/language_server/use_thread");
	poll_limit_usec = (int)_EDITOR_GET("network/language_server/poll_limit_usec");
	if (protocol.start(port, IPAddress(host)) == OK) {
		EditorNode::get_log()->add_message("--- GDScript language server started on port " + itos(port) + " ---", EditorLog::MSG_TYPE_EDITOR);
		if (use_thread) {
			thread_running = true;
			thread.start(GDScriptLanguageServer::thread_main, this);
		}
		set_process_internal(!use_thread);
		started = true;
	}
}

void GDScriptLanguageServer::stop() {
	if (use_thread) {
		ERR_FAIL_COND(!thread.is_started());
		thread_running = false;
		thread.wait_to_finish();
	}
	protocol.stop();
	started = false;
	EditorNode::get_log()->add_message("--- GDScript language server stopped ---", EditorLog::MSG_TYPE_EDITOR);
}

void register_lsp_types() {
	GDREGISTER_CLASS(GDScriptLanguageProtocol);
	GDREGISTER_CLASS(GDScriptTextDocument);
	GDREGISTER_CLASS(GDScriptWorkspace);
}

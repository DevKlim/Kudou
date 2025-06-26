#pragma once

#include "editor/plugins/editor_debugger_plugin.h"
#include "editor/plugins/editor_plugin.h"

class EditorNetworkProfiler;
class MultiplayerEditorDebugger : public EditorDebuggerPlugin {
	GDCLASS(MultiplayerEditorDebugger, EditorDebuggerPlugin);

private:
	HashMap<int, EditorNetworkProfiler *> profilers;

	void _open_request(const String &p_path);
	void _profiler_activate(bool p_enable, int p_session_id);

protected:
	static void _bind_methods();

public:
	virtual bool has_capture(const String &p_capture) const override;
	virtual bool capture(const String &p_message, const Array &p_data, int p_index) override;
	virtual void setup_session(int p_session_id) override;
};

class ReplicationEditor;

class MultiplayerEditorPlugin : public EditorPlugin {
	GDCLASS(MultiplayerEditorPlugin, EditorPlugin);

private:
	Button *button = nullptr;
	ReplicationEditor *repl_editor = nullptr;
	Ref<MultiplayerEditorDebugger> debugger;

	void _open_request(const String &p_path);
	void _node_removed(Node *p_node);

	void _pinned();

protected:
	void _notification(int p_what);

public:
	virtual void edit(Object *p_object) override;
	virtual bool handles(Object *p_object) const override;
	virtual void make_visible(bool p_visible) override;

	MultiplayerEditorPlugin();
};

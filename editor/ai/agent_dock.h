#ifndef AGENT_DOCK_H
#define AGENT_DOCK_H

#include "editor/plugins/editor_plugin.h"

class Button;
class Control;
class EditorNode;
class LineEdit;
class RichTextLabel;

class EditorPluginAgentDock : public EditorPlugin {
	GDCLASS(EditorPluginAgentDock, EditorPlugin);

private:
	Control *agent_dock = nullptr;
	RichTextLabel *chat_history = nullptr;
	LineEdit *user_input = nullptr;
	Button *send_button = nullptr;

protected:
	void _notification(int p_what);

public:
	virtual String get_plugin_name() const override { return "Kudou Agent"; }
	virtual const Ref<Texture2D> get_plugin_icon() const override;

	EditorPluginAgentDock(EditorNode *p_node);
	~EditorPluginAgentDock();
};

#endif // AGENT_DOCK_H
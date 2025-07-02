#ifndef KUDOU_AGENT_PLUGIN_H
#define KUDOU_AGENT_PLUGIN_H

#include "editor/plugins/editor_plugin.h"

// Forward declare classes to reduce header includes
class Button;
class CheckBox;
class Control;
class EditorNode;
class Label;
class LineEdit;
class RichTextLabel;
class Tree;
class TreeItem;
class KudouChatController;

class KudouAgentPlugin : public EditorPlugin {
	GDCLASS(KudouAgentPlugin, EditorPlugin);

private:
	EditorNode *editor_node = nullptr;
	Control *agent_dock = nullptr;
	RichTextLabel *chat_history = nullptr;
	LineEdit *user_input = nullptr;
	Button *send_button = nullptr;
	Tree *file_tree = nullptr;
	Tree *scene_tree = nullptr;
	Button *refresh_scene_button = nullptr;
	CheckBox *edit_mode_checkbox = nullptr;
	KudouChatController *chat_controller = nullptr;
	Dictionary prompts;

protected:
	void _notification(int p_what);
	void _on_send_button_pressed();
	void _on_text_submitted(const String &p_text);
	void _on_chat_message_received(const String &message);
	void _on_settings_changed();
	void _on_refresh_scene_button_pressed();
	void _on_item_edited();

	// File tree helpers
	void _populate_file_tree(TreeItem *p_root);
	void _populate_file_tree_recursive(const String &p_path, TreeItem *p_parent);
	void _collect_checked_items_recursive(TreeItem *p_item, PackedStringArray &r_paths);
	String _get_tscn_node_data(const String &p_tscn_path, const String &p_node_name);
	Dictionary _parse_tscn_file(const String &p_path);

	// Scene tree helpers
	void _populate_scene_tree_recursive(Node *p_node, TreeItem *p_parent);
	PackedStringArray _get_checked_node_paths(TreeItem *p_item);
	String _get_node_data_as_string(Node *p_node);

	// General helpers
	void _load_prompts();

public:
	static void _bind_methods();

	virtual String get_plugin_name() const override { return "Kudou Agent"; }
	virtual const Ref<Texture2D> get_plugin_icon() const override;
	virtual bool has_main_screen() const override { return false; }

	KudouAgentPlugin();
	~KudouAgentPlugin();
};

#endif // KUDOU_AGENT_PLUGIN_H
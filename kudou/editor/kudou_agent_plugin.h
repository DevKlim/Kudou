#ifndef KUDOU_AGENT_PLUGIN_H
#define KUDOU_AGENT_PLUGIN_H

#include "editor/plugins/editor_plugin.h"

// Forward declare classes to reduce header includes
class Button;
class CheckBox;
class Control;
class EditorBrowser;
class EditorNode;
class Label;
class LineEdit;
class OptionButton;
class RichTextLabel;
class Tree;
class TreeItem;
class KudouChatController;
class EditorFileSystemDirectory; // Added this forward declaration
class Node; // Added this forward declaration for _populate_node_items_recursive

class KudouAgentPlugin : public EditorPlugin {
	GDCLASS(KudouAgentPlugin, EditorPlugin);

private:
	EditorBrowser *_find_web_browser_view();

private:
	EditorNode *editor_node = nullptr;
	Control *agent_dock = nullptr;
	RichTextLabel *chat_history = nullptr;
	LineEdit *user_input = nullptr;
	Button *send_button = nullptr;
	Tree *file_tree = nullptr;
	CheckBox *edit_mode_checkbox = nullptr;
	OptionButton *web_chat_button = nullptr;
	KudouChatController *chat_controller = nullptr;
	Dictionary prompts;

	// New private helper methods for tree management
	void _propagate_check_down(TreeItem *p_item, bool p_checked);
	void _update_parent_check_state(TreeItem *p_item);
	void _get_checked_items_recursive(TreeItem *p_item, PackedStringArray &r_items);
	void _populate_file_tree(EditorFileSystemDirectory *p_dir, TreeItem *p_parent);
	void _populate_nodes_for_tscn(TreeItem *p_tscn_item, const String &p_path);
	void _populate_node_items_recursive(Node *p_node, TreeItem *p_parent_item, Node *p_scene_root, const String &p_tscn_path);

protected:
	void _notification(int p_what);
	void _on_send_button_pressed();
	void _on_copy_button_pressed();
	void _process_and_send_message(const String &p_message);
	void _on_text_submitted(const String &p_text);
	void _on_chat_message_received(const String &message);
	void _on_settings_changed();
	void _on_settings_button_pressed();
	void _enable_chat_input();
	void _on_filesystem_changed();
	// Removed: void _add_checkboxes_to_tree(TreeItem *p_item);
	void _on_item_edited();

	// General helpers
	void _load_prompts();
	String _get_scene_node_context_xml(const String &p_tscn_path, const Vector<String> &p_node_paths);

public:
	static void _bind_methods();

	virtual String get_plugin_name() const override { return "Kudou Agent"; }
	virtual const Ref<Texture2D> get_plugin_icon() const override;
	virtual bool has_main_screen() const override { return false; }

	KudouAgentPlugin();
	~KudouAgentPlugin();
};

#endif // KUDOU_AGENT_PLUGIN_H
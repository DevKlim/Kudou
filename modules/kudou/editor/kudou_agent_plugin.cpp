#include "kudou_agent_plugin.h"

#include "core/io/dir_access.h"
#include "core/io/file_access.h"
#include "editor/editor_interface.h"
#include "editor/editor_node.h"
#include "editor/editor_paths.h"
#include "editor/editor_settings.h"
#include "editor/themes/editor_scale.h"
#include "scene/gui/box_container.h"
#include "scene/gui/button.h"
#include "scene/gui/check_box.h"
#include "scene/gui/line_edit.h"
#include "scene/gui/rich_text_label.h"
#include "scene/gui/tree.h"

#include "modules/kudou/editor/kudou_chat_controller.h"

KudouAgentPlugin::KudouAgentPlugin() {
	// Define settings here. This is the correct way for a module to add settings.
	EDITOR_DEF("kudou/llm/base_url", "https://generativelanguage.googleapis.com/v1beta");
	EditorSettings::get_singleton()->add_property_hint(PropertyInfo(Variant::STRING, "kudou/llm/base_url", PROPERTY_HINT_PLACEHOLDER_TEXT, "LLM API Base URL"));
	EDITOR_DEF("kudou/llm/model", "gemini-1.5-flash");
	EDITOR_DEF("kudou/llm/api_key", "");
	EditorSettings::get_singleton()->add_property_hint(PropertyInfo(Variant::STRING, "kudou/llm/api_key", PROPERTY_HINT_PASSWORD, ""));

	chat_controller = memnew(KudouChatController);
	add_child(chat_controller);
}

KudouAgentPlugin::~KudouAgentPlugin() {
	// Child nodes like chat_controller are freed automatically.
	// The agent_dock is handled in NOTIFICATION_EXIT_TREE.
}

void KudouAgentPlugin::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_ENTER_TREE: {
			editor_node = EditorNode::get_singleton();

			// Connect to the controller's signal.
			if (chat_controller) {
				chat_controller->connect(SNAME("message_received"), callable_mp(this, &KudouAgentPlugin::_on_chat_message_received));
				EditorSettings::get_singleton()->connect("settings_changed", callable_mp(this, &KudouAgentPlugin::_on_settings_changed));
			}

			// Dock panel
			agent_dock = memnew(VBoxContainer);
			agent_dock->set_name(TTR("Kudou Agent"));
			agent_dock->set_custom_minimum_size(Size2(0, 200 * EDSCALE));

			chat_history = memnew(RichTextLabel);
			chat_history->set_v_size_flags(Control::SIZE_EXPAND_FILL);
			agent_dock->add_child(chat_history);

			// File tree for context selection
			file_tree = memnew(Tree);
			file_tree->set_v_size_flags(Control::SIZE_EXPAND_FILL);
			agent_dock->add_child(file_tree);

			file_tree->clear();
			TreeItem *root_item = file_tree->create_item();
			root_item->set_text(0, "res://");
			root_item->set_icon(0, editor_node->get_gui_base()->get_theme_icon(SNAME("Folder"), SNAME("EditorIcons")));
			root_item->set_collapsed(false);
			root_item->set_metadata(0, "res://");
			root_item->set_cell_mode(0, TreeItem::CELL_MODE_CHECK);
			_populate_file_tree(root_item);

			HBoxContainer *input_container = memnew(HBoxContainer);
			agent_dock->add_child(input_container);

			user_input = memnew(LineEdit);
			user_input->set_h_size_flags(Control::SIZE_EXPAND_FILL);
			user_input->set_placeholder(TTR("Ask Kudou..."));
			user_input->connect(SNAME("text_submitted"), callable_mp(this, &KudouAgentPlugin::_on_text_submitted));
			input_container->add_child(user_input);

			send_button = memnew(Button);
			send_button->set_text(TTR("Send"));
			send_button->connect(SNAME("pressed"), callable_mp(this, &KudouAgentPlugin::_on_send_button_pressed));
			input_container->add_child(send_button);

			edit_mode_checkbox = memnew(CheckBox);
			edit_mode_checkbox->set_text(TTR("Edit Project Files"));
			agent_dock->add_child(edit_mode_checkbox);

			// Add the control to the dock.
			add_control_to_dock(DOCK_SLOT_RIGHT_UL, agent_dock);

			// Load LLM settings from EditorSettings and configure the controller.
			chat_controller->set_api_key(EDITOR_GET("kudou/llm/api_key"));
			chat_controller->set_model(EDITOR_GET("kudou/llm/model"));
			chat_controller->set_base_url(EDITOR_GET("kudou/llm/base_url"));

		} break;

		case NOTIFICATION_EXIT_TREE: {
			if (agent_dock) {
				remove_control_from_docks(agent_dock);
				memdelete(agent_dock); // agent_dock is not a child, so we must delete it.
				agent_dock = nullptr;
			}
		} break;
	}
}

const Ref<Texture2D> KudouAgentPlugin::get_plugin_icon() const {
	// Use a built-in icon for the agent.
	return EditorInterface::get_singleton()->get_base_control()->get_theme_icon(SNAME("Comment"), SNAME("EditorIcons"));
}

void KudouAgentPlugin::_on_send_button_pressed() {
	String message = user_input->get_text();
	if (message.is_empty()) {
		return;
	}
	user_input->clear();
	chat_history->add_text(vformat("You: %s\n", message));

	String context_message = "";
	PackedStringArray checked_files = _get_checked_files(file_tree->get_root());

	for (int i = 0; i < checked_files.size(); ++i) {
		String file_path = checked_files[i];
		Ref<FileAccess> file = FileAccess::open(file_path, FileAccess::READ);
		if (file.is_valid()) {
			context_message += vformat("\n\nFile: %s\n```\n%s\n```",
					file_path, file->get_as_text());
		} else {
			ERR_PRINT("Could not open file: " + file_path);
		}
	}

	String full_prompt;
	if (edit_mode_checkbox->is_pressed()) {
		full_prompt = vformat(
				"You are an AI assistant that helps edit project files. "
				"Based on the following context and user request, provide code changes in the format:\n"
				"```file_path\n"
				"code_to_replace\n"
				"---\n"
				"new_code\n"
				"```\n\n"
				"User Request: %s\n\n"
				"Context:%s",
				message, context_message);
	} else {
		full_prompt = vformat(
				"You are an AI assistant that answers questions about the project. "
				"Based on the following context and user request, provide a concise answer.\n\n"
				"User Request: %s\n\n"
				"Context:%s",
				message, context_message);
	}

	chat_controller->send_message(full_prompt);
}

void KudouAgentPlugin::_on_text_submitted(const String &p_text) {
	_on_send_button_pressed();
}

void KudouAgentPlugin::_on_chat_message_received(const String &message) {
	chat_history->add_text(vformat("Kudou: %s\n", message));

	if (edit_mode_checkbox->is_pressed()) {
		Vector<String> parts = message.split("```");
		for (int i = 1; i < parts.size(); i += 2) {
			String code_block = parts[i].strip_edges();
			if (!code_block.contains("\n---\n")) {
				continue;
			}

			String file_path = code_block.get_slice("\n", 0).strip_edges();
			String old_code = code_block.get_slice("\n", 1).get_slice("---\n", 0).strip_edges();
			String new_code = code_block.get_slice("---\n", 1).strip_edges();

			if (!file_path.is_empty() && !old_code.is_empty()) {
				Ref<FileAccess> file_read = FileAccess::open(file_path, FileAccess::READ);
				if (file_read.is_valid()) {
					String file_content = file_read->get_as_text();
					String new_file_content = file_content.replace(old_code, new_code);
					Ref<FileAccess> file_write = FileAccess::open(file_path, FileAccess::WRITE);
					if (file_write.is_valid()) {
						file_write->store_string(new_file_content);
						chat_history->add_text(vformat("Kudou: Applied changes to %s\n", file_path));
					} else {
						ERR_PRINT("Could not write to file: " + file_path);
						chat_history->add_text(vformat("Kudou: Error writing to %s\n", file_path));
					}
				} else {
					ERR_PRINT("Could not read file: " + file_path);
					chat_history->add_text(vformat("Kudou: Error reading %s\n", file_path));
				}
			}
		}
	}
}

void KudouAgentPlugin::_on_settings_changed() {
	chat_controller->set_api_key(EDITOR_GET("kudou/llm/api_key"));
	chat_controller->set_model(EDITOR_GET("kudou/llm/model"));
	chat_controller->set_base_url(EDITOR_GET("kudou/llm/base_url"));
}

void KudouAgentPlugin::_populate_file_tree_recursive(const String &p_path, TreeItem *p_parent) {
	Ref<DirAccess> dir = DirAccess::open(p_path);
	if (dir.is_null()) {
		return;
	}

	dir->list_dir_begin();
	String file = dir->get_next();
	while (!file.is_empty()) {
		if (file == "." || file == ".." || file.begins_with(".")) {
			file = dir->get_next();
			continue;
		}

		String full_path = p_path.path_join(file);
		TreeItem *item = file_tree->create_item(p_parent);
		item->set_text(0, file);
		item->set_cell_mode(0, TreeItem::CELL_MODE_CHECK);
		item->set_metadata(0, full_path);

		if (dir->current_is_dir()) {
			item->set_icon(0, editor_node->get_gui_base()->get_theme_icon(SNAME("Folder"), SNAME("EditorIcons")));
			_populate_file_tree_recursive(full_path, item);
		} else {
			item->set_icon(0, editor_node->get_gui_base()->get_theme_icon(SNAME("File"), SNAME("EditorIcons")));
		}
		file = dir->get_next();
	}
	dir->list_dir_end();
}

void KudouAgentPlugin::_populate_file_tree(TreeItem *p_root) {
	_populate_file_tree_recursive("res://", p_root);
}

PackedStringArray KudouAgentPlugin::_get_checked_files(TreeItem *p_item) {
	PackedStringArray checked_files;
	if (!p_item) {
		return checked_files;
	}

	for (TreeItem *child = p_item->get_first_child(); child; child = child->get_next()) {
		if (child->is_checked(0)) {
			String path = child->get_metadata(0);
			if (!DirAccess::dir_exists_absolute(path)) { // It's a file
				checked_files.push_back(path);
			}
		}
		// Always recurse into children to find more checked items.
		if (child->get_child_count() > 0) {
			checked_files.append_array(_get_checked_files(child));
		}
	}
	return checked_files;
}
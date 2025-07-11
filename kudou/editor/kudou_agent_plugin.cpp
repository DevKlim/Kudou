#include "kudou_agent_plugin.h"
#include "editor_browser.h"

#include "core/config/project_settings.h"
#include "core/io/config_file.h"
#include "core/io/dir_access.h"
#include "core/io/file_access.h"
#include "core/io/json.h"
#include "core/os/os.h"
#include "editor/editor_file_system.h"
#include "editor/editor_interface.h"
#include "editor/editor_node.h"
#include "editor/editor_paths.h"
#include "editor/editor_settings.h"
#include "editor/editor_settings_dialog.h" // Needed for the dialog popup.
#include "editor/themes/editor_scale.h"
#include "editor_browser.h"
#include "scene/2d/node_2d.h"
#include "scene/3d/node_3d.h"
#include "scene/gui/box_container.h"
#include "scene/gui/button.h"
#include "scene/gui/check_box.h"
#include "scene/gui/control.h"
#include "scene/gui/label.h"
#include "scene/gui/line_edit.h"
#include "scene/gui/option_button.h"
#include "scene/gui/rich_text_label.h"
#include "scene/gui/split_container.h"
#include "scene/gui/tab_container.h"
#include "scene/gui/tree.h"
#include "scene/resources/packed_scene.h"
#include "servers/display_server.h"

#include "editor/scene_tree_dock.h"
#include "kudou/editor/kudou_browser_plugin.h" // Include the browser plugin header
#include "kudou/editor/kudou_chat_controller.h"


void KudouAgentPlugin::_bind_methods() {
	ClassDB::bind_method(D_METHOD("_process_and_send_message", "message"), &KudouAgentPlugin::_process_and_send_message);
}

KudouAgentPlugin::KudouAgentPlugin() {
	// Define settings here. This is the correct way for a module to add settings.
	EDITOR_DEF("kudou/llm/base_url", "https://generativelanguage.googleapis.com/v1beta");
	EditorSettings::get_singleton()->add_property_hint(PropertyInfo(Variant::STRING, "kudou/llm/base_url", PROPERTY_HINT_PLACEHOLDER_TEXT, "LLM API Base URL"));
	EDITOR_DEF("kudou/llm/model", "gemini-2.5-flash");
	EDITOR_DEF("kudou/llm/api_key", "");
	EditorSettings::get_singleton()->add_property_hint(PropertyInfo(Variant::STRING, "kudou/llm/api_key", PROPERTY_HINT_PASSWORD, ""));

	chat_controller = memnew(KudouChatController);
	add_child(chat_controller);
}

KudouAgentPlugin::~KudouAgentPlugin() {
}

// Add a helper function to find the main browser view
EditorBrowser *KudouAgentPlugin::_find_web_browser_view() {
	EditorPluginList *over_plugins = EditorNode::get_singleton()->get_editor_plugins_over();
	if (over_plugins) {
		for (int i = 0; i < over_plugins->get_plugins_list().size(); ++i) {
			EditorPlugin *p = over_plugins->get_plugins_list()[i];
			if (KudouBrowserPlugin *kbp = Object::cast_to<KudouBrowserPlugin>(p)) {
				return kbp->get_browser_view();
			}
		}
	}
	return nullptr;
}

void KudouAgentPlugin::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_ENTER_TREE: {
			editor_node = EditorNode::get_singleton();

			if (!editor_node) {
				ERR_PRINT("EditorNode is null!");
				return;
			}
			if (!editor_node->get_gui_base()) {
				ERR_PRINT("EditorNode GUI base is null!");
				return;
			}

			_load_prompts();

			if (chat_controller) {
				chat_controller->connect(SNAME("message_received"), callable_mp(this, &KudouAgentPlugin::_on_chat_message_received));
				EditorSettings::get_singleton()->connect("settings_changed", callable_mp(this, &KudouAgentPlugin::_on_settings_changed));
			}

			EditorFileSystem::get_singleton()->connect("filesystem_changed", callable_mp(this, &KudouAgentPlugin::_on_filesystem_changed));

			agent_dock = memnew(VBoxContainer);
			agent_dock->set_name(TTR("Kudou Agent"));
			agent_dock->set_custom_minimum_size(Size2(0, 200 * EDSCALE));
			agent_dock->set_v_size_flags(Control::SIZE_EXPAND_FILL);

			// REMOVED: The TabContainer is no longer needed.
			// The browser is now a separate main screen.
			// TabContainer *tab_container = memnew(TabContainer);
			// tab_container->set_v_size_flags(Control::SIZE_EXPAND_FILL);
			// agent_dock->add_child(tab_container);

			// Agent Chat Tab (now the main content of the dock)
			// VBoxContainer *chat_tab = memnew(VBoxContainer);
			// chat_tab->set_name(TTR("Agent"));
			// tab_container->add_child(chat_tab);

			HSplitContainer *split_container = memnew(HSplitContainer);
			split_container->set_v_size_flags(Control::SIZE_EXPAND_FILL);
			agent_dock->add_child(split_container); // Add the split container directly to the dock

			// REMOVED: The browser tab is no longer part of the agent dock.
			// EditorBrowser *browser_tab = memnew(EditorBrowser);
			// browser_tab->set_name(TTR("Web"));
			// tab_container->add_child(browser_tab);

			VBoxContainer *left_panel = memnew(VBoxContainer);
			left_panel->set_custom_minimum_size(Size2(250 * EDSCALE, 0));
			split_container->add_child(left_panel);

			Label *file_tree_label = memnew(Label);
			file_tree_label->set_text(TTR("Project Files Context"));
			left_panel->add_child(file_tree_label);

			file_tree = memnew(Tree);
			file_tree->set_v_size_flags(Control::SIZE_EXPAND_FILL);
			left_panel->add_child(file_tree);
			file_tree->connect(SNAME("item_edited"), callable_mp(this, &KudouAgentPlugin::_on_item_edited));
			file_tree->set_columns(1);
			file_tree->set_hide_root(true);

			_on_filesystem_changed(); // Initial population.

			VBoxContainer *right_panel = memnew(VBoxContainer);
			split_container->add_child(right_panel);

			HBoxContainer *chat_header = memnew(HBoxContainer);
			right_panel->add_child(chat_header);

			Label *chat_label = memnew(Label);
			chat_label->set_text(TTR("Kudou Chat"));
			chat_label->set_h_size_flags(Control::SIZE_EXPAND_FILL); // Make label expand to push settings button to right
			chat_header->add_child(chat_label);

			Button *header_settings_button = memnew(Button);
			header_settings_button->set_flat(true);
			header_settings_button->set_button_icon(editor_node->get_gui_base()->get_theme_icon(SNAME("EditorSettings"), SNAME("EditorIcons")));
			header_settings_button->set_tooltip_text(TTR("LLM Settings"));
			header_settings_button->connect(SNAME("pressed"), callable_mp(this, &KudouAgentPlugin::_on_settings_button_pressed));
			chat_header->add_child(header_settings_button);

			chat_history = memnew(RichTextLabel);
			chat_history->set_v_size_flags(Control::SIZE_EXPAND_FILL);
			chat_history->set_selection_enabled(true);
			right_panel->add_child(chat_history);

			HBoxContainer *input_container = memnew(HBoxContainer);
			right_panel->add_child(input_container);

			user_input = memnew(LineEdit);
			user_input->set_h_size_flags(Control::SIZE_EXPAND_FILL);
			user_input->set_placeholder(TTR("Ask Kudou..."));
			user_input->connect(SNAME("text_submitted"), callable_mp(this, &KudouAgentPlugin::_on_text_submitted));
			input_container->add_child(user_input);

			send_button = memnew(Button);
			send_button->set_text(TTR("Send"));
			send_button->connect(SNAME("pressed"), callable_mp(this, &KudouAgentPlugin::_on_send_button_pressed));
			input_container->add_child(send_button);

			Button *copy_button = memnew(Button);
			copy_button->set_text(TTR("Copy"));
			copy_button->connect(SNAME("pressed"), callable_mp(this, &KudouAgentPlugin::_on_copy_button_pressed));
			input_container->add_child(copy_button);

			web_chat_button = memnew(OptionButton);
			web_chat_button->add_item("Kudou API", 0);
			web_chat_button->add_item("AiStudio", 1);
			web_chat_button->add_item("Gemini", 2);
			web_chat_button->add_item("ChatGPT", 3);
			web_chat_button->add_item("Perplexity", 4);
			web_chat_button->add_item("Claude", 5);
			web_chat_button->select(0);
			input_container->add_child(web_chat_button);

			edit_mode_checkbox = memnew(CheckBox);
			edit_mode_checkbox->set_text(TTR("Edit Project Files"));
			right_panel->add_child(edit_mode_checkbox);

			add_control_to_dock(DOCK_SLOT_RIGHT_UL, agent_dock);

			chat_controller->set_api_key(EDITOR_GET("kudou/llm/api_key"));
			chat_controller->set_model(EDITOR_GET("kudou/llm/model"));
			chat_controller->set_base_url(EDITOR_GET("kudou/llm/base_url"));

		} break;

		case NOTIFICATION_EXIT_TREE: {
			if (agent_dock) {
				remove_control_from_docks(agent_dock);
				memdelete(agent_dock);
				agent_dock = nullptr;
			}
		} break;
	}
}

const Ref<Texture2D> KudouAgentPlugin::get_plugin_icon() const {
	return EditorInterface::get_singleton()->get_base_control()->get_theme_icon(SNAME("Comment"), SNAME("EditorIcons"));
}

void KudouAgentPlugin::_on_send_button_pressed() {
	String message = user_input->get_text();
	if (message.is_empty() || send_button->is_disabled()) {
		return;
	}
	send_button->set_disabled(true);
	user_input->set_editable(false);

	user_input->clear();
	chat_history->add_text(vformat("You: %s\n", message));

	// Defer the heavy processing to avoid blocking the UI thread.
	callable_mp(this, &KudouAgentPlugin::_process_and_send_message).call_deferred(message);
}

void KudouAgentPlugin::_on_copy_button_pressed() {
	DisplayServer::get_singleton()->clipboard_set(user_input->get_text());
}

void KudouAgentPlugin::_process_and_send_message(const String &p_message) {
	String context_message = "<context>\n";
	const int MAX_CONTEXT_SIZE = 200000; // 200KB limit to prevent freezing on huge files.
	bool context_limit_reached = false;

	PackedStringArray collected_paths;
	if (file_tree && file_tree->get_root()) {
		_get_checked_items_recursive(file_tree->get_root(), collected_paths);
	}

	HashMap<String, Vector<String>> tscn_nodes_to_process;

	for (int i = 0; i < collected_paths.size(); ++i) {
		String item_data = collected_paths[i];

		if (item_data.contains("::")) {
			Vector<String> parts = item_data.split("::");
			String tscn_path = parts[0];
			String node_name = parts[1];
			if (!tscn_nodes_to_process.has(tscn_path)) {
				tscn_nodes_to_process[tscn_path] = Vector<String>();
			}
			tscn_nodes_to_process[tscn_path].push_back(node_name);
		} else if (DirAccess::dir_exists_absolute(item_data)) {
			context_message += vformat("\t<directory path=\"%s\"/>\n", item_data);
		} else {
			Ref<FileAccess> file = FileAccess::open(item_data, FileAccess::READ);
			if (file.is_valid()) {
				uint64_t len = file->get_length();
				if (context_message.length() + len > MAX_CONTEXT_SIZE) {
					len = MAX_CONTEXT_SIZE - context_message.length();
					context_limit_reached = true;
				}
				PackedByteArray pba = file->get_buffer(len);
				String text = String::utf8((const char *)pba.ptr(), pba.size()).xml_escape();
				context_message += vformat("\t<file path=\"%s\">\n%s\n\t</file>\n", item_data, text);
			} else {
				ERR_PRINT("Could not open file: " + item_data);
			}
		}
		if (context_limit_reached) {
			break;
		}
	}

	for (const KeyValue<String, Vector<String>> &E : tscn_nodes_to_process) {
		if (context_limit_reached) {
			break;
		}
		String node_content = _get_scene_node_context_xml(E.key, E.value);
		if (context_message.length() + node_content.length() > MAX_CONTEXT_SIZE) {
			context_limit_reached = true;
		} else {
			context_message += node_content;
		}
	}

	context_message += "</context>\n";

	if (context_limit_reached) {
		chat_history->add_text("Kudou: [Warning] Context size limit reached. Some content was truncated or skipped.\n");
	}

	String prompt_template;
	if (edit_mode_checkbox->is_pressed()) {
		prompt_template = prompts.get("edit_mode_prompt", "");
	} else {
		prompt_template = prompts.get("chat_mode_prompt", "");
	}

	String full_prompt = prompt_template.replace("{message}", p_message).replace("{context}", context_message);

	int selected_service = web_chat_button->get_selected_id();

	if (selected_service == 0) { // Kudou API
		chat_controller->send_message(full_prompt, web_chat_button->get_item_text(web_chat_button->get_selected()));
	} else {
		// --- This is the new logic to link chat to the browser ---
		EditorBrowser *web_browser_control = _find_web_browser_view();
		if (!web_browser_control) {
			chat_history->add_text("Kudou: [Error] Web browser plugin not found. Make sure KudouBrowserPlugin is enabled.\n");
			_enable_chat_input();
			return;
		}

		String url;
		switch (selected_service) {
			case 1: // AiStudio
				url = "https://aistudio.google.com/app/prompts/new";
				break;
			case 2: // Gemini
				url = "https://gemini.google.com/app";
				break;
			case 3: // ChatGPT
				url = "https://chat.openai.com";
				break;
			case 4: // Perplexity
				url = "https://www.perplexity.ai/";
				break;
			case 5: // Claude
				url = "https://claude.ai";
				break;
		}

		if (!url.is_empty()) {
			web_browser_control->load_url(url);
			chat_history->add_text(vformat("Kudou: Navigating to %s. Prompt copied to clipboard.\n", web_chat_button->get_item_text(selected_service)));
			DisplayServer::get_singleton()->clipboard_set(full_prompt);
			// Switch focus to the browser main screen
			EditorInterface::get_singleton()->set_main_screen_editor("Web");
		}
		_enable_chat_input();
	}
}

void KudouAgentPlugin::_on_text_submitted(const String &p_text) {
	_on_send_button_pressed();
}

void KudouAgentPlugin::_enable_chat_input() {
	send_button->set_disabled(false);
	user_input->set_editable(true);
	user_input->grab_focus();
}

void KudouAgentPlugin::_on_chat_message_received(const String &message) {
	chat_history->add_text(vformat("Kudou: %s\n", message));
	_enable_chat_input();

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

void KudouAgentPlugin::_on_settings_button_pressed() {
	EditorNode::get_singleton()->open_setting_override("kudou/llm");
}

void KudouAgentPlugin::_on_filesystem_changed() {
	if (!file_tree) {
		return;
	}
	file_tree->clear();
	TreeItem *root = file_tree->create_item();
	_populate_file_tree(EditorFileSystem::get_singleton()->get_filesystem(), root);
}

void KudouAgentPlugin::_populate_file_tree(EditorFileSystemDirectory *p_dir, TreeItem *p_parent) {
	if (!p_dir) {
		return;
	}

	// Subdirectories
	for (int i = 0; i < p_dir->get_subdir_count(); ++i) {
		EditorFileSystemDirectory *subdir = p_dir->get_subdir(i);
		TreeItem *dir_item = file_tree->create_item(p_parent);
		dir_item->set_cell_mode(0, TreeItem::CELL_MODE_CHECK);
		dir_item->set_text(0, subdir->get_name() + "/");
		dir_item->set_icon(0, editor_node->get_gui_base()->get_theme_icon(SNAME("Folder"), SNAME("EditorIcons")));
		dir_item->set_editable(0, true);
		dir_item->set_metadata(0, subdir->get_path());
		_populate_file_tree(subdir, dir_item);
	}

	// Files
	for (int i = 0; i < p_dir->get_file_count(); ++i) {
		String file_name = p_dir->get_file(i);
		String file_path = p_dir->get_file_path(i);
		String file_type = p_dir->get_file_type(i);

		TreeItem *file_item = file_tree->create_item(p_parent);
		file_item->set_cell_mode(0, TreeItem::CELL_MODE_CHECK);
		file_item->set_text(0, file_name);
		file_item->set_editable(0, true);
		file_item->set_metadata(0, file_path);

		// Use file type to get icon, fallback to a generic icon if not found
		if (editor_node->get_gui_base()->has_theme_icon(file_type, SNAME("EditorIcons"))) {
			file_item->set_icon(0, editor_node->get_gui_base()->get_theme_icon(file_type, SNAME("EditorIcons")));
		} else {
			file_item->set_icon(0, editor_node->get_gui_base()->get_theme_icon(SNAME("Object"), SNAME("EditorIcons")));
		}

		if (file_type == "PackedScene" && (file_path.ends_with(".tscn") || file_path.ends_with(".scn"))) {
			_populate_nodes_for_tscn(file_item, file_path);
		}
	}
}

void KudouAgentPlugin::_populate_nodes_for_tscn(TreeItem *p_tscn_item, const String &p_path) {
	Ref<PackedScene> packed_scene = ResourceLoader::load(p_path);
	if (packed_scene.is_null()) {
		WARN_PRINT(vformat("Kudou: Failed to load PackedScene at %s", p_path));
		return;
	}

	Node *scene_root = packed_scene->instantiate(PackedScene::GEN_EDIT_STATE_DISABLED);
	if (!scene_root) {
		WARN_PRINT(vformat("Kudou: Failed to instantiate scene from %s", p_path));
		return;
	}

	_populate_node_items_recursive(scene_root, p_tscn_item, scene_root, p_path);

	scene_root->queue_free();
}

void KudouAgentPlugin::_populate_node_items_recursive(Node *p_node, TreeItem *p_parent_item, Node *p_scene_root, const String &p_tscn_path) {
	if (!p_node) {
		return;
	}

	TreeItem *node_item = file_tree->create_item(p_parent_item);
	node_item->set_cell_mode(0, TreeItem::CELL_MODE_CHECK);
	node_item->set_text(0, p_node->get_name());
	node_item->set_icon(0, editor_node->get_gui_base()->get_theme_icon(p_node->get_class(), SNAME("EditorIcons")));
	node_item->set_editable(0, true);

	NodePath path_to_node = p_scene_root->get_path_to(p_node);
	String meta = p_tscn_path + "::" + String(path_to_node);
	node_item->set_metadata(0, meta);

	for (int i = 0; i < p_node->get_child_count(); i++) {
		_populate_node_items_recursive(p_node->get_child(i), node_item, p_scene_root, p_tscn_path);
	}
}

void KudouAgentPlugin::_on_item_edited() {
	TreeItem *item = file_tree->get_edited();
	if (!item) {
		return;
	}

	int col = file_tree->get_edited_column();
	// We only care about checkbox clicks in the first column.
	if (col != 0 || item->get_cell_mode(col) != TreeItem::CELL_MODE_CHECK) {
		return;
	}

	// When an item is checked/unchecked by the user, it can't be indeterminate.
	item->set_indeterminate(col, false);

	bool checked = item->is_checked(col);

	// Propagate to children.
	_propagate_check_down(item, checked);

	// Propagate to parents.
	_update_parent_check_state(item);
}

void KudouAgentPlugin::_propagate_check_down(TreeItem *p_item, bool p_checked) {
	if (!p_item) {
		return;
	}
	for (TreeItem *child = p_item->get_first_child(); child; child = child->get_next()) {
		if (child->get_cell_mode(0) == TreeItem::CELL_MODE_CHECK) {
			child->set_checked(0, p_checked);
			// When a child's state is explicitly set, it should no longer be indeterminate.
			child->set_indeterminate(0, false);
			_propagate_check_down(child, p_checked);
		}
	}
}

void KudouAgentPlugin::_update_parent_check_state(TreeItem *p_item) {
	if (!p_item || !p_item->get_parent()) {
		return;
	}

	TreeItem *parent = p_item->get_parent();
	if (parent->get_cell_mode(0) != TreeItem::CELL_MODE_CHECK) {
		return;
	}

	bool all_checked = true;
	bool any_checked = false;
	bool any_indeterminate = false;

	for (TreeItem *child = parent->get_first_child(); child; child = child->get_next()) {
		if (child->get_cell_mode(0) == TreeItem::CELL_MODE_CHECK) {
			if (child->is_indeterminate(0)) {
				any_indeterminate = true;
				break; // If any child is indeterminate, the parent must be indeterminate.
			}
			if (child->is_checked(0)) {
				any_checked = true;
			} else {
				all_checked = false;
			}
		}
	}

	bool state_changed = false;
	if (any_indeterminate || (any_checked && !all_checked)) {
		// Parent should be indeterminate if any child is indeterminate, or if some but not all children are checked.
		if (!parent->is_indeterminate(0)) {
			parent->set_indeterminate(0, true);
			state_changed = true;
		}
	} else {
		// Parent should be fully checked if all children are checked, or unchecked if no children are checked.
		if (parent->is_indeterminate(0) || parent->is_checked(0) != all_checked) {
			parent->set_checked(0, all_checked); // This also sets indeterminate to false.
			state_changed = true;
		}
	}

	if (state_changed) {
		// Recursively update the parent's parent.
		_update_parent_check_state(parent);
	}
}

void KudouAgentPlugin::_get_checked_items_recursive(TreeItem *p_item, PackedStringArray &r_items) {
	if (!p_item) {
		return;
	}

	// If an item is checked, we add it and don't look at its children,
	// as the parent's context is assumed to include all children.
	if (p_item->is_checked(0)) {
		String path = p_item->get_metadata(0);
		if (!path.is_empty()) {
			r_items.push_back(path);
		}
		return; // Stop recursion here.
	}

	// If an item is not checked (could be indeterminate), we need to check its children.
	for (TreeItem *child = p_item->get_first_child(); child; child = child->get_next()) {
		_get_checked_items_recursive(child, r_items);
	}
}

String KudouAgentPlugin::_get_scene_node_context_xml(const String &p_tscn_path, const Vector<String> &p_node_paths) {
	Ref<PackedScene> packed_scene = ResourceLoader::load(p_tscn_path);
	if (packed_scene.is_null()) {
		ERR_PRINT("Could not load .tscn file: " + p_tscn_path);
		return "";
	}

	Node *scene_root = packed_scene->instantiate();
	if (!scene_root) {
		ERR_PRINT("Could not instantiate scene root from: " + p_tscn_path);
		return "";
	}

	String xml_context;
	xml_context += vformat("\t<scene path=\"%s\">\n", p_tscn_path);

	for (int i = 0; i < p_node_paths.size(); i++) {
		Node *node = scene_root->get_node(p_node_paths[i]);
		if (node) {
			xml_context += vformat("\t\t<node name=\"%s\" type=\"%s\" path=\"%s\">\n", node->get_name(), node->get_class(), node->get_path());

			List<PropertyInfo> properties;
			node->get_property_list(&properties);
			for (const PropertyInfo &prop : properties) {
				if (prop.usage & PROPERTY_USAGE_STORAGE || prop.usage & PROPERTY_USAGE_EDITOR) {
					Variant value = node->get(prop.name);
					String value_str = value; // Implicit conversion
					xml_context += vformat("\t\t\t<property name=\"%s\" value=\"%s\"/>\n", prop.name, value_str.xml_escape());
				}
			}
			xml_context += "\t\t</node>\n";
		}
	}

	xml_context += "\t</scene>\n";

	scene_root->queue_free();

	return xml_context;
}

void KudouAgentPlugin::_load_prompts() {
	String config_dir = EditorPaths::get_singleton()->get_config_dir();
	String kudou_dir = config_dir.path_join("kudou");
	Ref<DirAccess> da = DirAccess::create(DirAccess::ACCESS_FILESYSTEM);
	if (da->make_dir_recursive(kudou_dir) != OK) {
		ERR_PRINT("Could not create Kudou config directory: " + kudou_dir);
		return;
	}

	String prompts_path = kudou_dir.path_join("prompts.json");

	if (!FileAccess::exists(prompts_path)) {
		Dictionary default_prompts;
		default_prompts["chat_mode_prompt"] = "You are an AI assistant for Godot Engine. Answer the user's question based on the provided context. If you don't know the answer, say so. User message: {message}\nContext: {context}";
		default_prompts["edit_mode_prompt"] = "You are an AI assistant for Godot Engine. The user wants to modify their project files. Provide code snippets to modify the files. Use the following format for modifications: ```\n<file_path>\n<old_code>\n---\n<new_code>\n```. User message: {message}\nContext: {context}";

		Ref<FileAccess> f = FileAccess::open(prompts_path, FileAccess::WRITE);
		if (f.is_null()) {
			ERR_PRINT("prompts.json does not exist and could not be created at: " + prompts_path);
			return;
		}
		f->store_string(JSON::stringify(default_prompts, "\t"));
	}

	Ref<FileAccess> file = FileAccess::open(prompts_path, FileAccess::READ);
	if (file.is_valid()) {
		Ref<JSON> json = memnew(JSON);
		Error err = json->parse(file->get_as_text());
		if (err == OK) {
			prompts = json->get_data();
		} else {
			ERR_PRINT("Error parsing prompts.json: " + json->get_error_message() + " at line " + itos(json->get_error_line()));
		}
	} else {
		ERR_PRINT("Could not open prompts.json file: " + prompts_path);
	}
}
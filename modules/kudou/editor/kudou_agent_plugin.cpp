#include "kudou_agent_plugin.h"

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

#include "modules/kudou/editor/kudou_chat_controller.h"
#include "modules/kudou/editor/kudou_tree.h"

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
			editor_node->connect("scene_changed", callable_mp(this, &KudouAgentPlugin::_on_refresh_scene_button_pressed));

			agent_dock = memnew(VBoxContainer);
			agent_dock->set_name(TTR("Kudou Agent"));
			agent_dock->set_custom_minimum_size(Size2(0, 200 * EDSCALE));

			HSplitContainer *split_container = memnew(HSplitContainer);
			split_container->set_v_size_flags(Control::SIZE_EXPAND_FILL);
			agent_dock->add_child(split_container);

			VBoxContainer *left_panel = memnew(VBoxContainer);
			left_panel->set_custom_minimum_size(Size2(250 * EDSCALE, 0));
			split_container->add_child(left_panel);

			Label *file_tree_label = memnew(Label);
			file_tree_label->set_text(TTR("Project Files Context"));
			left_panel->add_child(file_tree_label);

			file_tree = memnew(KudouTree);
			file_tree->set_v_size_flags(Control::SIZE_EXPAND_FILL);
			file_tree->set_columns(1);
			file_tree->set_column_expand(0, true);
			left_panel->add_child(file_tree);

			_on_filesystem_changed(); // Initial population.

			Label *scene_tree_label = memnew(Label);
			scene_tree_label->set_text(TTR("Current Scene Context"));
			left_panel->add_child(scene_tree_label);

			scene_tree = memnew(KudouTree);
			scene_tree->set_v_size_flags(Control::SIZE_EXPAND_FILL);
			scene_tree->set_columns(1);
			scene_tree->set_column_expand(0, true);
			left_panel->add_child(scene_tree);

			refresh_scene_button = memnew(Button);
			refresh_scene_button->set_text(TTR("Refresh Scene"));
			refresh_scene_button->connect(SNAME("pressed"), callable_mp(this, &KudouAgentPlugin::_on_refresh_scene_button_pressed));
			left_panel->add_child(refresh_scene_button);

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

			web_chat_button = memnew(OptionButton);
			web_chat_button->add_item("Kudou API", 0);
			web_chat_button->add_item("Gemini", 1);
			web_chat_button->add_item("ChatGPT", 2);
			web_chat_button->add_item("Perplexity", 3);
			web_chat_button->add_item("Claude", 4);
			web_chat_button->select(0);
			input_container->add_child(web_chat_button);

			edit_mode_checkbox = memnew(CheckBox);
			edit_mode_checkbox->set_text(TTR("Edit Project Files"));
			right_panel->add_child(edit_mode_checkbox);

			add_control_to_dock(DOCK_SLOT_RIGHT_UL, agent_dock);

			chat_controller->set_api_key(EDITOR_GET("kudou/llm/api_key"));
			chat_controller->set_model(EDITOR_GET("kudou/llm/model"));
			chat_controller->set_base_url(EDITOR_GET("kudou/llm/base_url"));

			_on_refresh_scene_button_pressed();

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

void KudouAgentPlugin::_process_and_send_message(const String &p_message) {
	String context_message = "";
	const int MAX_CONTEXT_SIZE = 200000; // 200KB limit to prevent freezing on huge files.
	bool context_limit_reached = false;

	PackedStringArray collected_paths;
	if (file_tree && file_tree->get_root()) {
		file_tree->get_checked_items(collected_paths);
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
			context_message += vformat("Directory: %s\n", item_data);
		} else {
			Ref<FileAccess> file = FileAccess::open(item_data, FileAccess::READ);
			if (file.is_valid()) {
				uint64_t len = file->get_length();
				if (context_message.length() + len > MAX_CONTEXT_SIZE) {
					len = MAX_CONTEXT_SIZE - context_message.length();
					context_limit_reached = true;
				}
				PackedByteArray pba = file->get_buffer(len);
				String text = String::utf8((const char *)pba.ptr(), pba.size());
				context_message += vformat("File: %s:\n%s\n", item_data, text);
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
		String node_content = _get_multiple_tscn_nodes_data(E.key, E.value);
		if (context_message.length() + node_content.length() > MAX_CONTEXT_SIZE) {
			context_limit_reached = true;
		} else {
			context_message += vformat("Nodes from %s:\n%s\n", E.key, node_content);
		}
	}

	PackedStringArray checked_node_paths;
	if (scene_tree && scene_tree->get_root()) {
		scene_tree->get_checked_items(checked_node_paths);
	}

	if (checked_node_paths.size() > 0) {
		context_message += "Current Scene Nodes:\n";
		Node *scene_root = editor_node->get_edited_scene();
		if (scene_root) {
			for (int i = 0; i < checked_node_paths.size(); ++i) {
				Node *node = scene_root->get_node_or_null(checked_node_paths[i]);
				if (node) {
					String node_str = _get_node_data_as_string(node);
					if (context_message.length() + node_str.length() > MAX_CONTEXT_SIZE) {
						context_limit_reached = true;
						break;
					}
					context_message += node_str;
				}
			}
		}
	}

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
		chat_controller->send_message(full_prompt);
	} else {
		String url;
		String encoded_prompt = full_prompt.uri_encode();
		switch (selected_service) {
			case 1: // Gemini
				url = "https://gemini.google.com/app?prompt=" + encoded_prompt;
				break;
			case 2: // ChatGPT
				url = "https://chat.openai.com";
				chat_history->add_text("Kudou: Opening ChatGPT. Please paste the prompt manually.\n");
				break;
			case 3: // Perplexity
				url = "https://www.perplexity.ai/search?q=" + encoded_prompt;
				break;
			case 4: // Claude
				url = "https://claude.ai";
				chat_history->add_text("Kudou: Opening Claude. Please paste the prompt manually.\n");
				break;
		}
		if (!url.is_empty()) {
			OS::get_singleton()->shell_open(url);
			chat_history->add_text(vformat("Kudou: Opening web chat for prompt in your browser.\n"));
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
	EditorSettingsDialog *esd = EditorNode::get_singleton()->get_editor_settings_dialog();
	esd->set_filter("kudou/llm");
	esd->popup_edit_settings();
}

void KudouAgentPlugin::_populate_file_tree_recursive(const String &p_path, TreeItem *p_parent) {
	Ref<DirAccess> dir = DirAccess::open(p_path);
	if (dir.is_null()) {
		ERR_PRINT("Could not open directory: " + p_path);
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
		item->set_cell_mode(0, TreeItem::CELL_MODE_CHECK);
		item->set_editable(0, true);
		item->set_metadata(0, full_path);

		if (dir->current_is_dir()) {
			item->set_text(0, file);
			item->set_icon(0, editor_node->get_gui_base()->get_theme_icon(SNAME("Folder"), SNAME("EditorIcons")));
			_populate_file_tree_recursive(full_path, item);
		} else {
			item->set_text(0, file);
			if (file.ends_with(".tscn")) {
				item->set_icon(0, editor_node->get_gui_base()->get_theme_icon(SNAME("PackedScene"), SNAME("EditorIcons")));
				_populate_tscn_nodes_in_tree(item, full_path);
			} else {
				String res_type = EditorFileSystem::get_singleton()->get_file_type(full_path);
				Ref<Texture2D> file_icon;
				if (!res_type.is_empty()) {
					file_icon = editor_node->get_gui_base()->get_theme_icon(res_type, SNAME("EditorIcons"));
				}
				if (file_icon.is_null()) {
					file_icon = editor_node->get_gui_base()->get_theme_icon(SNAME("Object"), SNAME("EditorIcons"));
				}
				item->set_icon(0, file_icon);
			}
		}
		file = dir->get_next();
	}
	dir->list_dir_end();
}

void KudouAgentPlugin::_on_filesystem_changed() {
	file_tree->clear();
	TreeItem *file_root_item = file_tree->create_item();
	file_root_item->set_text(0, "res://");
	file_root_item->set_icon(0, editor_node->get_gui_base()->get_theme_icon(SNAME("Folder"), SNAME("EditorIcons")));
	file_root_item->set_collapsed(false);
	file_root_item->set_metadata(0, "res://");
	file_root_item->set_cell_mode(0, TreeItem::CELL_MODE_CHECK);
	file_root_item->set_editable(0, true);
	_populate_file_tree(file_root_item);
}

void KudouAgentPlugin::_populate_file_tree(TreeItem *p_root) {
	_populate_file_tree_recursive("res://", p_root);
}

String KudouAgentPlugin::_get_multiple_tscn_nodes_data(const String &p_tscn_path, const Vector<String> &p_node_names) {
	Ref<FileAccess> file = FileAccess::open(p_tscn_path, FileAccess::READ);
	if (file.is_null()) {
		ERR_PRINT("Could not open .tscn file: " + p_tscn_path);
		return "";
	}

	String content = file->get_as_text();
	Vector<String> lines = content.split("\n");
	String result_data;

	for (const String &node_name : p_node_names) {
		String node_data;
		bool found_node = false;
		for (int i = 0; i < lines.size(); ++i) {
			String line = lines[i].strip_edges();
			if (!found_node) {
				if (line.begins_with("[node") && line.contains(vformat("name=\"%s\"", node_name))) {
					found_node = true;
					node_data += line + "\n";
				}
			} else {
				if (line.begins_with("[node") || line.begins_with("[ext_resource") || line.begins_with("[sub_resource") || line.begins_with("[connection")) {
					break;
				}
				node_data += line + "\n";
			}
		}
		result_data += node_data + "\n";
	}
	return result_data;
}

String KudouAgentPlugin::_get_tscn_node_data(const String &p_tscn_path, const String &p_node_name) {
	Vector<String> node_names;
	node_names.push_back(p_node_name);
	return _get_multiple_tscn_nodes_data(p_tscn_path, node_names);
}

void KudouAgentPlugin::_on_refresh_scene_button_pressed() {
	scene_tree->clear();
	TreeItem *root_item = scene_tree->create_item();
	Node *scene_root = editor_node->get_edited_scene();
	if (scene_root) {
		root_item->set_text(0, scene_root->get_name());
		String node_type = scene_root->get_class();
		Ref<Texture2D> node_icon = editor_node->get_gui_base()->get_theme_icon(node_type, SNAME("EditorIcons"));
		if (node_icon.is_null()) {
			node_icon = editor_node->get_gui_base()->get_theme_icon(SNAME("Node"), SNAME("EditorIcons"));
		}
		root_item->set_icon(0, node_icon);
		root_item->set_metadata(0, NodePath("."));
		root_item->set_cell_mode(0, TreeItem::CELL_MODE_CHECK);
		root_item->set_editable(0, true);
		_populate_scene_tree_recursive(scene_root, root_item);
	} else {
		root_item->set_text(0, "No scene loaded.");
		root_item->set_custom_color(0, Color(1, 1, 1, 0.5));
	}
}

void KudouAgentPlugin::_populate_scene_tree_recursive(Node *p_node, TreeItem *p_parent) {
	for (int i = 0; i < p_node->get_child_count(); ++i) {
		Node *child = p_node->get_child(i);
		if (child->get_owner() != editor_node->get_edited_scene()) {
			continue;
		}
		TreeItem *item = scene_tree->create_item(p_parent);
		item->set_text(0, child->get_name());
		String node_type = child->get_class();
		Ref<Texture2D> node_icon = editor_node->get_gui_base()->get_theme_icon(node_type, SNAME("EditorIcons"));
		if (node_icon.is_null()) {
			node_icon = editor_node->get_gui_base()->get_theme_icon(SNAME("Node"), SNAME("EditorIcons"));
		}
		item->set_icon(0, node_icon);
		item->set_metadata(0, editor_node->get_edited_scene()->get_path_to(child));
		item->set_cell_mode(0, TreeItem::CELL_MODE_CHECK);
		item->set_editable(0, true);
		_populate_scene_tree_recursive(child, item);
	}
}

String KudouAgentPlugin::_get_node_data_as_string(Node *p_node) {
	if (!p_node) {
		return "";
	}

	String node_data_string = vformat(" - Node: %s, Type: %s\n", p_node->get_name(), p_node->get_class());

	Ref<Script> node_script_ref = p_node->get_script();
	if (node_script_ref.is_valid()) {
		node_data_string += vformat("   Script: %s\n", node_script_ref->get_path());
	}

	Node2D *node_2d = Object::cast_to<Node2D>(p_node);
	if (node_2d) {
		node_data_string += vformat("   Position: %s, Rotation: %f, Scale: %s\n",
				(String)node_2d->get_position(), node_2d->get_rotation_degrees(), (String)node_2d->get_scale());
	}

	Node3D *node_3d = Object::cast_to<Node3D>(p_node);
	if (node_3d) {
		node_data_string += vformat("   Transform: %s\n", (String)node_3d->get_transform());
	}

	Control *control = Object::cast_to<Control>(p_node);
	if (control) {
		node_data_string += vformat("   Position: %s, Size: %s\n",
				(String)control->get_position(), (String)control->get_size());
	}

	node_data_string += "\n";
	return node_data_string;
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
		Ref<FileAccess> f = FileAccess::open(prompts_path, FileAccess::WRITE);
		if (f.is_null()) {
			ERR_PRINT("prompts.json does not exist and could not be created at: " + prompts_path);
			return;
		}
		f->store_string("{\n"
						"    \"chat_mode_prompt\": \"You are an AI assistant for Godot Engine. Answer the user's question based on the provided context. If you don't know the answer, say so. User message: {message}\\nContext: {context}\",\n"
						"    \"edit_mode_prompt\": \"You are an AI assistant for Godot Engine. The user wants to modify their project files. Provide code snippets to modify the files. Use the following format for modifications: ```\\n<file_path>\\n<old_code>\\n---\\n<new_code>\\n```. User message: {message}\\nContext: {context}\"\n"
						"}\n");
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

void KudouAgentPlugin::_populate_tscn_nodes_in_tree(TreeItem *p_tscn_item, const String &p_path) {
	Dictionary tscn_nodes = _parse_tscn_file(p_path);
	Array node_names = tscn_nodes.keys();
	HashMap<String, TreeItem *> item_map;

	// First pass: create all items and store them in a map.
	for (int i = 0; i < node_names.size(); ++i) {
		String node_name = node_names[i];
		Dictionary node_info = tscn_nodes[node_name];
		String node_type = node_info["type"];

		TreeItem *node_item = file_tree->create_item(); // Don't parent yet.
		node_item->set_text(0, node_name);
		node_item->set_cell_mode(0, TreeItem::CELL_MODE_CHECK);
		node_item->set_editable(0, true);
		node_item->set_metadata(0, p_path + "::" + node_name);
		Ref<Texture2D> node_icon = editor_node->get_gui_base()->get_theme_icon(node_type, SNAME("EditorIcons"));
		if (node_icon.is_null()) {
			node_icon = editor_node->get_gui_base()->get_theme_icon(SNAME("Node"), SNAME("EditorIcons"));
		}
		node_item->set_icon(0, node_icon);
		item_map[node_name] = node_item;
	}

	// Second pass: establish hierarchy.
	for (int i = 0; i < node_names.size(); ++i) {
		String node_name = node_names[i];
		Dictionary node_info = tscn_nodes[node_name];
		String parent_path_str = node_info.get("parent", ".");

		TreeItem *child_item = item_map[node_name];
		TreeItem *parent_item = p_tscn_item; // Default to the scene file root.

		if (parent_path_str != ".") {
			NodePath parent_path(parent_path_str);
			if (parent_path.get_name_count() > 0) {
				// This simplified logic assumes the parent is directly addressable by its name.
				// It works for `parent="Arm"` but might need enhancement for `parent="Arm/Hand"`.
				String parent_name = parent_path.get_name(parent_path.get_name_count() - 1);
				if (item_map.has(parent_name)) {
					parent_item = item_map[parent_name];
				}
			}
		}
		parent_item->add_child(child_item);
	}
}

Dictionary KudouAgentPlugin::_parse_tscn_file(const String &p_path) {
	Dictionary nodes_data;
	Ref<ConfigFile> cf = memnew(ConfigFile);
	Error err = cf->load(p_path);
	if (err != OK) {
		ERR_PRINT("Error loading TSCN as ConfigFile: " + p_path);
		return nodes_data;
	}

	Vector<String> sections = cf->get_sections();

	for (const String &section : sections) {
		if (section.begins_with("node ")) {
			String node_name = cf->get_value(section, "name", "");
			String node_type = cf->get_value(section, "type", "Node");
			String parent_path = cf->get_value(section, "parent", ".");

			if (!node_name.is_empty()) {
				Dictionary node_info;
				node_info["type"] = node_type;
				node_info["parent"] = parent_path;
				nodes_data[node_name] = node_info;
			}
		}
	}
	return nodes_data;
}
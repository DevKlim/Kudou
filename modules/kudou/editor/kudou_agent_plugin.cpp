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
#include "scene/resources/packed_scene.h"
#include "servers/display_server.h"

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
		String url;
		String encoded_prompt = full_prompt.uri_encode();
		chat_history->add_text(vformat("Kudou: Please copy the following prompt and paste it into the %s chatbox:\n%s\n", web_chat_button->get_item_text(selected_service), full_prompt));
		DisplayServer::get_singleton()->clipboard_set(full_prompt);
		switch (selected_service) {
			case 1: // AiStudio
				url = "https://aistudio.google.com/app/prompts/new";
				break;
			case 2: // Gemini
				url = "https://gemini.google.com/app?prompt=" + encoded_prompt;
				break;
			case 3: // ChatGPT
				url = "https://chat.openai.com";
				break;
			case 4: // Perplexity
				url = "https://www.perplexity.ai/search?q=" + encoded_prompt;
				break;
			case 5: // Claude
				url = "https://claude.ai";
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
			    Ref<PackedScene> packed_scene = ResourceLoader::load(full_path);
				if (packed_scene.is_valid()) {
					Ref<SceneState> state = packed_scene->get_state();
					for (int i = 0; i < state->get_node_count(); i++) {
						if (state->get_node_path(i, true).is_empty()) {
							_add_nodes_to_file_tree_recursively(state, item, full_path, i, "");
						}
					}
				}
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

void KudouAgentPlugin::_add_nodes_to_file_tree_recursively(Ref<SceneState> p_scene_state, TreeItem *p_parent_item, const String &p_tscn_path, int p_node_idx, const String &p_parent_path) {
	String node_name = p_scene_state->get_node_name(p_node_idx);
	String node_type = p_scene_state->get_node_type(p_node_idx);
	String current_path = p_parent_path.is_empty() ? node_name : p_parent_path + "/" + node_name;

	TreeItem *item = file_tree->create_item(p_parent_item);
	item->set_text(0, node_name);
	item->set_icon(0, editor_node->get_gui_base()->get_theme_icon(node_type, SNAME("EditorIcons")));
	item->set_metadata(0, p_tscn_path + "::" + current_path);
	item->set_cell_mode(0, TreeItem::CELL_MODE_CHECK);
	item->set_editable(0, true);

	NodePath node_path = p_scene_state->get_node_path(p_node_idx);
	for (int i = 0; i < p_scene_state->get_node_count(); i++) {
		if (p_scene_state->get_node_path(i, true) == node_path) {
			_add_nodes_to_file_tree_recursively(p_scene_state, item, p_tscn_path, i, current_path);
		}
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
	}
	else {
		ERR_PRINT("Could not open prompts.json file: " + prompts_path);
	}
}
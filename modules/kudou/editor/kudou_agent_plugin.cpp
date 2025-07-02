#include "kudou_agent_plugin.h"

#include "core/config/project_settings.h"
#include "core/io/dir_access.h"
#include "core/io/file_access.h"
#include "core/io/json.h"
#include "core/os/os.h"
#include "editor/editor_interface.h"
#include "editor/editor_node.h"
#include "editor/editor_paths.h"
#include "editor/editor_settings.h"
#include "editor/themes/editor_scale.h"
#include "scene/2d/node_2d.h"
#include "scene/3d/node_3d.h"
#include "scene/gui/box_container.h"
#include "scene/gui/button.h"
#include "scene/gui/check_box.h"
#include "scene/gui/control.h"
#include "scene/gui/label.h"
#include "scene/gui/line_edit.h"
#include "scene/gui/rich_text_label.h"
#include "scene/gui/split_container.h"
#include "scene/gui/tree.h"

#include "modules/kudou/editor/kudou_chat_controller.h"

void KudouAgentPlugin::_bind_methods() {
}

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

			if (!editor_node) {
				ERR_PRINT("EditorNode is null!");
				return;
			}
			if (!editor_node->get_gui_base()) {
				ERR_PRINT("EditorNode GUI base is null!");
				return;
			}

			// Load prompts from JSON file.
			_load_prompts();

			// Connect to the controller's signal.
			if (chat_controller) {
				chat_controller->connect(SNAME("message_received"), callable_mp(this, &KudouAgentPlugin::_on_chat_message_received));
				EditorSettings::get_singleton()->connect("settings_changed", callable_mp(this, &KudouAgentPlugin::_on_settings_changed));
			}

			// Dock panel
			agent_dock = memnew(VBoxContainer);
			agent_dock->set_name(TTR("Kudou Agent"));
			agent_dock->set_custom_minimum_size(Size2(0, 200 * EDSCALE));

			// Split container for resizable panels
			HSplitContainer *split_container = memnew(HSplitContainer);
			split_container->set_v_size_flags(Control::SIZE_EXPAND_FILL);
			agent_dock->add_child(split_container);

			// Left panel for file and scene trees
			VBoxContainer *left_panel = memnew(VBoxContainer);
			left_panel->set_custom_minimum_size(Size2(250 * EDSCALE, 0)); // Increased minimum width for better usability
			split_container->add_child(left_panel);

			// File tree Label
			Label *file_tree_label = memnew(Label);
			file_tree_label->set_text(TTR("Project Files Context"));
			left_panel->add_child(file_tree_label);

			// File tree for context selection
			file_tree = memnew(Tree);
			file_tree->set_v_size_flags(Control::SIZE_EXPAND_FILL);
			file_tree->set_columns(1); // Explicitly set columns
			// The item_edited signal is used to propagate checks to children/parents.
			file_tree->connect(SNAME("item_edited"), callable_mp(this, &KudouAgentPlugin::_on_item_edited));
			left_panel->add_child(file_tree);

			file_tree->clear();
			TreeItem *file_root_item = file_tree->create_item();
			file_root_item->set_text(0, "res://");
			file_root_item->set_icon(0, editor_node->get_gui_base()->get_theme_icon(SNAME("Folder"), SNAME("EditorIcons")));
			file_root_item->set_collapsed(false);
			file_root_item->set_metadata(0, "res://");
			file_root_item->set_cell_mode(0, TreeItem::CELL_MODE_CHECK);
			_populate_file_tree(file_root_item);

			// Scene tree Label
			Label *scene_tree_label = memnew(Label);
			scene_tree_label->set_text(TTR("Current Scene Context"));
			left_panel->add_child(scene_tree_label);

			// Scene tree for context selection
			scene_tree = memnew(Tree);
			scene_tree->set_v_size_flags(Control::SIZE_EXPAND_FILL);
			scene_tree->set_columns(1); // Explicitly set columns
			// The item_edited signal is used to propagate checks to children/parents.
			scene_tree->connect(SNAME("item_edited"), callable_mp(this, &KudouAgentPlugin::_on_item_edited));
			left_panel->add_child(scene_tree);

			refresh_scene_button = memnew(Button);
			refresh_scene_button->set_text(TTR("Refresh Scene"));
			refresh_scene_button->connect(SNAME("pressed"), callable_mp(this, &KudouAgentPlugin::_on_refresh_scene_button_pressed));
			left_panel->add_child(refresh_scene_button);

			// Right panel for chat
			VBoxContainer *right_panel = memnew(VBoxContainer);
			split_container->add_child(right_panel);

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

			edit_mode_checkbox = memnew(CheckBox);
			edit_mode_checkbox->set_text(TTR("Edit Project Files"));
			right_panel->add_child(edit_mode_checkbox);

			// Add the control to the dock.
			add_control_to_dock(DOCK_SLOT_RIGHT_UL, agent_dock);

			// Load LLM settings from EditorSettings and configure the controller.
			chat_controller->set_api_key(EDITOR_GET("kudou/llm/api_key"));
			chat_controller->set_model(EDITOR_GET("kudou/llm/model"));
			chat_controller->set_base_url(EDITOR_GET("kudou/llm/base_url"));

			_on_refresh_scene_button_pressed(); // Initial population

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

	// 1. Collect all checked non-directory items from the file tree.
	PackedStringArray collected_paths;
	if (file_tree && file_tree->get_root()) {
		_collect_checked_items_recursive(file_tree->get_root(), collected_paths);
	}

	// 2. Filter out nodes if their parent .tscn is also selected, and filter duplicate dirs/files.
	PackedStringArray final_paths;
	HashSet<String> tscn_files_checked;
	HashSet<String> final_paths_set;

	for (const String &path : collected_paths) {
		if (path.ends_with(".tscn") && !path.contains("::")) {
			tscn_files_checked.insert(path);
		}
	}

	for (const String &path : collected_paths) {
		if (path.contains("::")) {
			String tscn_path = path.get_slice("::", 0);
			if (tscn_files_checked.has(tscn_path)) {
				continue; // Skip this node, as its parent .tscn file is fully included.
			}
		}
		// Also filter files inside already-checked directories
		bool skip = false;
		String current_path = path;
		while (current_path.contains("/")) {
			current_path = current_path.get_base_dir();
			if (final_paths_set.has(current_path)) {
				skip = true;
				break;
			}
		}
		if (skip) {
			continue;
		}

		if (!final_paths_set.has(path)) {
			final_paths.push_back(path);
			final_paths_set.insert(path);
		}
	}

	// 3. Build the context string from the final list of files and nodes.
	for (int i = 0; i < final_paths.size(); ++i) {
		String item_data = final_paths[i];

		if (DirAccess::dir_exists_absolute(item_data)) {
			// This logic could be expanded to list files in a directory, but for now we just mention it.
			context_message += vformat("Directory: %s\n\n", item_data);
			continue;
		}

		if (item_data.contains("::")) {
			// This is an individual node from a .tscn file.
			Vector<String> parts = item_data.split("::");
			String tscn_path = parts[0];
			String node_name = parts[1];
			context_message += vformat("Node: %s from %s\n%s\n\n", node_name, tscn_path, _get_tscn_node_data(tscn_path, node_name));
		} else {
			// This is a regular file path (or a full .tscn file).
			String file_path = item_data;
			Ref<FileAccess> file = FileAccess::open(file_path, FileAccess::READ);
			if (file.is_valid()) {
				context_message += vformat("File: %s\n%s\n\n", file_path, file->get_as_text());
			} else {
				ERR_PRINT("Could not open file: " + file_path);
			}
		}
	}

	// 4. Build context from current scene tree.
	PackedStringArray checked_node_paths;
	if (scene_tree && scene_tree->get_root()) {
		checked_node_paths = _get_checked_node_paths(scene_tree->get_root());
	}

	if (checked_node_paths.size() > 0) {
		context_message += "Current Scene Nodes:\n";
		Node *scene_root = editor_node->get_edited_scene();
		if (scene_root) {
			for (int i = 0; i < checked_node_paths.size(); ++i) {
				Node *node = scene_root->get_node_or_null(checked_node_paths[i]);
				if (node) {
					context_message += _get_node_data_as_string(node);
				}
			}
		}
	}

	String prompt_template;
	if (edit_mode_checkbox->is_pressed()) {
		prompt_template = prompts.get("edit_mode_prompt", "");
	} else {
		prompt_template = prompts.get("chat_mode_prompt", "");
	}

	String full_prompt = prompt_template.replace("{message}", message).replace("{context}", context_message);

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
		item->set_metadata(0, full_path);

		if (dir->current_is_dir()) {
			item->set_text(0, file);
			item->set_icon(0, editor_node->get_gui_base()->get_theme_icon(SNAME("Folder"), SNAME("EditorIcons")));
			_populate_file_tree_recursive(full_path, item);
		} else {
			item->set_text(0, vformat("File: %s", file));
			if (file.ends_with(".tscn")) {
				item->set_icon(0, editor_node->get_gui_base()->get_theme_icon(SNAME("PackedScene"), SNAME("EditorIcons")));
				// Parse .tscn and add its nodes as children
				Dictionary tscn_nodes = _parse_tscn_file(full_path);
				Array node_names = tscn_nodes.keys();
				for (int i = 0; i < node_names.size(); ++i) {
					String node_name = node_names[i];
					Dictionary node_info = tscn_nodes[node_name];
					TreeItem *node_item = file_tree->create_item(item);
					node_item->set_text(0, vformat("Node: %s (%s)", node_name, String(node_info["type"])));
					node_item->set_cell_mode(0, TreeItem::CELL_MODE_CHECK);
					node_item->set_metadata(0, full_path + "::" + node_name); // Store path and node name
					node_item->set_icon(0, editor_node->get_gui_base()->get_theme_icon(SNAME("Node"), SNAME("EditorIcons")));
				}
			} else {
				item->set_icon(0, editor_node->get_gui_base()->get_theme_icon(SNAME("File"), SNAME("EditorIcons")));
			}
		}
		file = dir->get_next();
	}
	dir->list_dir_end();
}

void KudouAgentPlugin::_populate_file_tree(TreeItem *p_root) {
	_populate_file_tree_recursive("res://", p_root);
}

void KudouAgentPlugin::_collect_checked_items_recursive(TreeItem *p_item, PackedStringArray &r_paths) {
	if (!p_item) {
		return;
	}

	if (p_item->is_checked(0)) {
		String path = p_item->get_metadata(0);
		r_paths.push_back(path);
	}

	// Always recurse into children to find more checked items.
	// The sending logic will handle filtering out children of checked parents.
	for (TreeItem *child = p_item->get_first_child(); child; child = child->get_next()) {
		_collect_checked_items_recursive(child, r_paths);
	}
}

String KudouAgentPlugin::_get_tscn_node_data(const String &p_tscn_path, const String &p_node_name) {
	Ref<FileAccess> file = FileAccess::open(p_tscn_path, FileAccess::READ);
	if (file.is_null()) {
		ERR_PRINT("Could not open .tscn file: " + p_tscn_path);
		return "";
	}

	String content = file->get_as_text();
	Vector<String> lines = content.split("\n");
	String node_data = "";
	bool found_node = false;

	for (int i = 0; i < lines.size(); ++i) {
		String line = lines[i].strip_edges();
		if (!found_node) {
			if (line.begins_with("[node") && line.contains(vformat("name=\"%s\"", p_node_name))) {
				found_node = true;
				node_data += line + "\n";
			}
		} else {
			if (line.begins_with("[node") || line.begins_with("[ext_resource") || line.begins_with("[sub_resource") || line.begins_with("[connection")) {
				break; // Reached the next section, stop collecting
			}
			node_data += line + "\n";
		}
	}
	return node_data;
}

void KudouAgentPlugin::_on_refresh_scene_button_pressed() {
	scene_tree->clear();
	TreeItem *root_item = scene_tree->create_item();
	Node *scene_root = editor_node->get_edited_scene();
	if (scene_root) {
		root_item->set_text(0, "Node: " + scene_root->get_name());
		root_item->set_icon(0, editor_node->get_gui_base()->get_theme_icon(SNAME("Node"), SNAME("EditorIcons")));
		root_item->set_metadata(0, NodePath(".")); // Use relative path for root.
		root_item->set_cell_mode(0, TreeItem::CELL_MODE_CHECK);
		_populate_scene_tree_recursive(scene_root, root_item);
	} else {
		root_item->set_text(0, "No scene loaded.");
	}
}

void KudouAgentPlugin::_on_item_edited() {
	// The item_edited signal has no parameters, so we get the edited item from the tree.
	// We need to figure out which tree sent the signal. We can check which one has an edited item.
	Tree *edited_tree = nullptr;
	if (file_tree && file_tree->get_edited()) {
		edited_tree = file_tree;
	} else if (scene_tree && scene_tree->get_edited()) {
		edited_tree = scene_tree;
	}

	if (!edited_tree) {
		return;
	}

	TreeItem *item = edited_tree->get_edited();
	if (!item) {
		return;
	}

	int column = edited_tree->get_edited_column();
	if (item->get_cell_mode(column) == TreeItem::CELL_MODE_CHECK) {
		// This will handle checking/unchecking children and updating parent states.
		// The second argument determines if `check_propagated_to_item` signal is emitted for each item.
		// Since we are not listening to that signal, 'false' is more efficient.
		item->propagate_check(column, false);
	}
}

void KudouAgentPlugin::_populate_scene_tree_recursive(Node *p_node, TreeItem *p_parent) {
	for (int i = 0; i < p_node->get_child_count(); ++i) {
		Node *child = p_node->get_child(i);
		if (child->get_owner() != editor_node->get_edited_scene()) {
			continue; // Only show nodes owned by the scene
		}
		TreeItem *item = scene_tree->create_item(p_parent);
		item->set_text(0, "Node: " + child->get_name());
		item->set_icon(0, editor_node->get_gui_base()->get_theme_icon(SNAME("Node"), SNAME("EditorIcons")));
		item->set_metadata(0, editor_node->get_edited_scene()->get_path_to(child));
		item->set_cell_mode(0, TreeItem::CELL_MODE_CHECK);
		_populate_scene_tree_recursive(child, item);
	}
}

PackedStringArray KudouAgentPlugin::_get_checked_node_paths(TreeItem *p_item) {
	PackedStringArray checked_nodes;
	if (!p_item) {
		return checked_nodes;
	}

	if (p_item->is_checked(0)) {
		checked_nodes.push_back(p_item->get_metadata(0));
	}

	for (TreeItem *child = p_item->get_first_child(); child; child = child->get_next()) {
		checked_nodes.append_array(_get_checked_node_paths(child));
	}
	return checked_nodes;
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
		// Use a multiline string for readability.
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

Dictionary KudouAgentPlugin::_parse_tscn_file(const String &p_path) {
	Dictionary nodes_data;
	Ref<FileAccess> file = FileAccess::open(p_path, FileAccess::READ);
	if (file.is_null()) {
		ERR_PRINT("Could not open .tscn file: " + p_path);
		return nodes_data;
	}

	String content = file->get_as_text();
	Vector<String> lines = content.split("\n");

	for (int i = 0; i < lines.size(); ++i) {
		String line = lines[i].strip_edges();
		if (line.begins_with("[node")) {
			int name_start = line.find("name=\"") + String("name=\"").length();
			int name_end = line.find("\"", name_start);
			if (name_start < name_end) {
				String node_name = line.substr(name_start, name_end - name_start);

				int type_start = line.find("type=\"") + String("type=\"").length();
				int type_end = line.find("\"", type_start);
				String node_type = "Unknown";
				if (type_start < type_end) {
					node_type = line.substr(type_start, type_end - type_start);
				}

				Dictionary node_info;
				node_info["name"] = node_name;
				node_info["type"] = node_type;

				nodes_data[node_name] = node_info;
			}
		}
	}
	return nodes_data;
}
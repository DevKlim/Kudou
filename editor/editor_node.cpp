#include "editor_node.h"

#include "core/config/project_settings.h"
#include "core/extension/gdextension_manager.h"
#include "core/input/input.h"
#include "core/io/config_file.h"
#include "core/io/file_access.h"
#include "core/io/image.h"
#include "core/io/resource_loader.h"
#include "core/io/resource_saver.h"
#include "core/object/class_db.h"
#include "core/os/keyboard.h"
#include "core/os/os.h"
#include "core/os/time.h"
#include "core/string/print_string.h"
#include "core/string/translation_server.h"
#include "core/version.h"
#include "editor/editor_string_names.h"
#include "editor/plugins/editor_context_menu_plugin.h"
#include "main/main.h"
#include "scene/2d/node_2d.h"
#include "scene/3d/bone_attachment_3d.h"
#include "scene/animation/animation_tree.h"
#include "scene/gui/color_picker.h"
#include "scene/gui/dialogs.h"
#include "scene/gui/file_dialog.h"
#include "scene/gui/menu_bar.h"
#include "scene/gui/menu_button.h"
#include "scene/gui/panel.h"
#include "scene/gui/popup.h"
#include "scene/gui/rich_text_label.h"
#include "scene/gui/split_container.h"
#include "scene/gui/tab_container.h"
#include "scene/main/window.h"
#include "scene/property_utils.h"
#include "scene/resources/image_texture.h"
#include "scene/resources/packed_scene.h"
#include "scene/resources/portable_compressed_texture.h"
#include "scene/theme/theme_db.h"
#include "servers/display_server.h"
#include "servers/navigation_server_2d.h"
#include "servers/navigation_server_3d.h"
#include "servers/rendering_server.h"

#include "editor/audio_stream_preview.h"
#include "editor/debugger/editor_debugger_node.h"
#include "editor/debugger/script_editor_debugger.h"
#include "editor/dependency_editor.h"
#include "editor/editor_about.h"
#include "editor/editor_audio_buses.h"
#include "editor/editor_build_profile.h"
#include "editor/editor_command_palette.h"
#include "editor/editor_data.h"
#include "editor/editor_dock_manager.h"
#include "editor/editor_feature_profile.h"
#include "editor/editor_folding.h"
#include "editor/editor_help.h"
#include "editor/editor_inspector.h"
#include "editor/editor_interface.h"
#include "editor/editor_layouts_dialog.h"
#include "editor/editor_log.h"
#include "editor/editor_main_screen.h"
#include "editor/editor_native_shader_source_visualizer.h"
#include "editor/editor_paths.h"
#include "editor/editor_properties.h"
#include "editor/editor_property_name_processor.h"
#include "editor/editor_resource_picker.h"
#include "editor/editor_resource_preview.h"
#include "editor/editor_run.h"
#include "editor/editor_script.h"
#include "editor/editor_settings.h"
#include "editor/editor_settings_dialog.h"
#include "editor/editor_translation_parser.h"
#include "editor/editor_undo_redo_manager.h"
#include "editor/export/editor_export.h"
#include "editor/export/export_template_manager.h"
#include "editor/export/project_export.h"
#include "editor/export/project_zip_packer.h"
#include "editor/fbx_importer_manager.h"
#include "editor/filesystem_dock.h"
#include "editor/gui/editor_bottom_panel.h"
#include "editor/gui/editor_file_dialog.h"
#include "editor/gui/editor_quick_open_dialog.h"
#include "editor/gui/editor_run_bar.h"
#include "editor/gui/editor_scene_tabs.h"
#include "editor/gui/editor_title_bar.h"
#include "editor/gui/editor_toaster.h"
#include "editor/history_dock.h"
#include "editor/import/3d/editor_import_collada.h"
#include "editor/import/3d/resource_importer_obj.h"
#include "editor/import/3d/resource_importer_scene.h"
#include "editor/import/3d/scene_import_settings.h"
#include "editor/import/audio_stream_import_settings.h"
#include "editor/import/dynamic_font_import_settings.h"
#include "editor/import/resource_importer_bitmask.h"
#include "editor/import/resource_importer_bmfont.h"
#include "editor/import/resource_importer_csv_translation.h"
#include "editor/import/resource_importer_dynamic_font.h"
#include "editor/import/resource_importer_image.h"
#include "editor/import/resource_importer_imagefont.h"
#include "editor/import/resource_importer_layered_texture.h"
#include "editor/import/resource_importer_shader_file.h"
#include "editor/import/resource_importer_svg.h"
#include "editor/import/resource_importer_texture.h"
#include "editor/import/resource_importer_texture_atlas.h"
#include "editor/import/resource_importer_wav.h"
#include "editor/import_dock.h"
#include "editor/inspector_dock.h"
#include "editor/multi_node_edit.h"
#include "editor/node_dock.h"
#include "editor/plugins/animation_player_editor_plugin.h"
#include "editor/plugins/asset_library_editor_plugin.h"
#include "editor/plugins/canvas_item_editor_plugin.h"
#include "editor/plugins/debugger_editor_plugin.h"
#include "editor/plugins/dedicated_server_export_plugin.h"
#include "editor/plugins/editor_plugin.h"
#include "editor/plugins/editor_preview_plugins.h"
#include "editor/plugins/editor_resource_conversion_plugin.h"
#include "editor/plugins/game_view_plugin.h"
#include "editor/plugins/gdextension_export_plugin.h"
#include "editor/plugins/material_editor_plugin.h"
#include "editor/plugins/mesh_library_editor_plugin.h"
#include "editor/plugins/node_3d_editor_plugin.h"
#include "editor/plugins/packed_scene_translation_parser_plugin.h"
#include "editor/plugins/particle_process_material_editor_plugin.h"
#include "editor/plugins/plugin_config_dialog.h"
#include "editor/plugins/root_motion_editor_plugin.h"
#include "editor/plugins/script_text_editor.h"
#include "editor/plugins/shader_baker_export_plugin.h"
#include "editor/plugins/text_editor.h"
#include "editor/plugins/version_control_editor_plugin.h"
#include "editor/plugins/visual_shader_editor_plugin.h"
#include "editor/progress_dialog.h"
#include "editor/project_settings_editor.h"
#include "editor/project_upgrade_tool.h"
#include "editor/register_exporters.h"
#include "editor/scene_tree_dock.h"
#include "editor/themes/editor_scale.h"
#include "editor/themes/editor_theme_manager.h"
#include "editor/window_wrapper.h"
#include "editor/ai/agent_dock.h"

#ifdef VULKAN_ENABLED
#include "editor/plugins/shader_baker/shader_baker_export_plugin_platform_vulkan.h"
#endif

#ifdef D3D12_ENABLED
#include "editor/plugins/shader_baker/shader_baker_export_plugin_platform_d3d12.h"
#endif

#ifdef METAL_ENABLED
#include "editor/plugins/shader_baker/shader_baker_export_plugin_platform_metal.h"
#endif

#include "modules/modules_enabled.gen.h" // For gdscript, mono.

#ifndef PHYSICS_2D_DISABLED
#include "servers/physics_server_2d.h"
#endif // PHYSICS_2D_DISABLED

#ifndef PHYSICS_3D_DISABLED
#include "servers/physics_server_3d.h"
#endif // PHYSICS_3D_DISABLED

#ifdef ANDROID_ENABLED
#include "editor/gui/touch_actions_panel.h"
#endif // ANDROID_ENABLED

#include <cstdlib>

EditorNode *EditorNode::singleton = nullptr;

static const String EDITOR_NODE_CONFIG_SECTION = "EditorNode";

static const String REMOVE_ANDROID_BUILD_TEMPLATE_MESSAGE = TTRC("The Android build template is already installed in this project and it won't be overwritten.\nRemove the \"%s\" directory manually before attempting this operation again.");
static const String INSTALL_ANDROID_BUILD_TEMPLATE_MESSAGE = TTRC("This will set up your project for gradle Android builds by installing the source template to \"%s\".\nNote that in order to make gradle builds instead of using pre-built APKs, the \"Use Gradle Build\" option should be enabled in the Android export preset.");

bool EditorProgress::step(const String &p_state, int p_step, bool p_force_refresh) {
	if (!force_background && Thread::is_main_thread()) {
		return EditorNode::progress_task_step(task, p_state, p_step, p_force_refresh);
	} else {
		EditorNode::progress_task_step_bg(task, p_step);
		return false;
	}
}

EditorProgress::EditorProgress(const String &p_task, const String &p_label, int p_amount, bool p_can_cancel, bool p_force_background) {
	if (!p_force_background && Thread::is_main_thread()) {
		EditorNode::progress_add_task(p_task, p_label, p_amount, p_can_cancel);
	} else {
		EditorNode::progress_add_task_bg(p_task, p_label, p_amount);
	}
	task = p_task;
	force_background = p_force_background;
}

EditorProgress::~EditorProgress() {
	if (!force_background && Thread::is_main_thread()) {
		EditorNode::progress_end_task(task);
	} else {
		EditorNode::progress_end_task_bg(task);
	}
}

void EditorNode::disambiguate_filenames(const Vector<String> p_full_paths, Vector<String> &r_filenames) {
	ERR_FAIL_COND_MSG(p_full_paths.size() != r_filenames.size(), vformat("disambiguate_filenames requires two string vectors of same length (%d != %d).", p_full_paths.size(), r_filenames.size()));

	// Keep track of a list of "index sets," i.e. sets of indices
	// within disambiguated_scene_names which contain the same name.
	Vector<RBSet<int>> index_sets;
	HashMap<String, int> scene_name_to_set_index;
	for (int i = 0; i < r_filenames.size(); i++) {
		const String &scene_name = r_filenames[i];
		if (!scene_name_to_set_index.has(scene_name)) {
			index_sets.append(RBSet<int>());
			scene_name_to_set_index.insert(r_filenames[i], index_sets.size() - 1);
		}
		index_sets.write[scene_name_to_set_index[scene_name]].insert(i);
	}

	// For each index set with a size > 1, we need to disambiguate.
	for (int i = 0; i < index_sets.size(); i++) {
		RBSet<int> iset = index_sets[i];
		while (iset.size() > 1) {
			// Append the parent folder to each scene name.
			for (const int &E : iset) {
				int set_idx = E;
				String scene_name = r_filenames[set_idx];
				String full_path = p_full_paths[set_idx];

				// Get rid of file extensions and res:// prefixes.
				scene_name = scene_name.get_basename();
				if (full_path.begins_with("res://")) {
					full_path = full_path.substr(6);
				}
				full_path = full_path.get_basename();

				// Normalize trailing slashes when normalizing directory names.
				scene_name = scene_name.trim_suffix("/");
				full_path = full_path.trim_suffix("/");

				int scene_name_size = scene_name.size();
				int full_path_size = full_path.size();
				int difference = full_path_size - scene_name_size;

				// Find just the parent folder of the current path and append it.
				// If the current name is foo.tscn, and the full path is /some/folder/foo.tscn
				// then slash_idx is the second '/', so that we select just "folder", and
				// append that to yield "folder/foo.tscn".
				if (difference > 0) {
					String parent = full_path.substr(0, difference);
					int slash_idx = parent.rfind_char('/');
					slash_idx = parent.rfind_char('/', slash_idx - 1);
					parent = (slash_idx >= 0 && parent.length() > 1) ? parent.substr(slash_idx + 1) : parent;
					r_filenames.write[set_idx] = parent + r_filenames[set_idx];
				}
			}

			// Loop back through scene names and remove non-ambiguous names.
			bool can_proceed = false;
			RBSet<int>::Element *E = iset.front();
			while (E) {
				String scene_name = r_filenames[E->get()];
				bool duplicate_found = false;
				for (const int &F : iset) {
					if (E->get() == F) {
						continue;
					}
					const String &other_scene_name = r_filenames[F];
					if (other_scene_name == scene_name) {
						duplicate_found = true;
						break;
					}
				}

				RBSet<int>::Element *to_erase = duplicate_found ? nullptr : E;

				// We need to check that we could actually append anymore names
				// if we wanted to for disambiguation. If we can't, then we have
				// to abort even with ambiguous names. We clean the full path
				// and the scene name first to remove extensions so that this
				// comparison actually works.
				String path = p_full_paths[E->get()];

				// Get rid of file extensions and res:// prefixes.
				scene_name = scene_name.get_basename();
				if (path.begins_with("res://")) {
					path = path.substr(6);
				}
				path = path.get_basename();

				// Normalize trailing slashes when normalizing directory names.
				scene_name = scene_name.trim_suffix("/");
				path = path.trim_suffix("/");

				// We can proceed if the full path is longer than the scene name,
				// meaning that there is at least one more parent folder we can
				// tack onto the name.
				can_proceed = can_proceed || (path.size() - scene_name.size()) >= 1;

				E = E->next();
				if (to_erase) {
					iset.erase(to_erase);
				}
			}

			if (!can_proceed) {
				break;
			}
		}
	}
}

void EditorNode::_version_control_menu_option(int p_idx) {
	switch (vcs_actions_menu->get_item_id(p_idx)) {
		case VCS_METADATA: {
			VersionControlEditorPlugin::get_singleton()->popup_vcs_metadata_dialog();
		} break;
		case VCS_SETTINGS: {
			VersionControlEditorPlugin::get_singleton()->popup_vcs_set_up_dialog(gui_base);
		} break;
	}
}

void EditorNode::_update_title() {
	const String appname = GLOBAL_GET("application/config/name");
	String title = (appname.is_empty() ? TTR("Unnamed Project") : appname);
	const String edited = editor_data.get_edited_scene_root() ? editor_data.get_edited_scene_root()->get_scene_file_path() : String();
	if (!edited.is_empty()) {
		// Display the edited scene name before the program name so that it can be seen in the OS task bar.
		title = vformat("%s - %s", edited.get_file(), title);
	}
	if (unsaved_cache) {
		// Display the "modified" mark before anything else so that it can always be seen in the OS task bar.
		title = vformat("(*) %s", title);
	}
	DisplayServer::get_singleton()->window_set_title(title + String(" - ") + GODOT_VERSION_NAME);
	if (project_title) {
		project_title->set_text(title);
	}
}

void EditorNode::_update_unsaved_cache() {
	bool is_unsaved = EditorUndoRedoManager::get_singleton()->is_history_unsaved(EditorUndoRedoManager::GLOBAL_HISTORY) ||
			EditorUndoRedoManager::get_singleton()->is_history_unsaved(editor_data.get_current_edited_scene_history_id());

	if (unsaved_cache != is_unsaved) {
		unsaved_cache = is_unsaved;
		_update_title();
	}
}

void EditorNode::input(const Ref<InputEvent> &p_event) {
	// EditorNode::get_singleton()->set_process_input is set to true in ProgressDialog
	// only when the progress dialog is visible.
	// We need to discard all key events to disable all shortcuts while the progress
	// dialog is displayed, simulating an exclusive popup. Mouse events are
	// captured by a full-screen container in front of the EditorNode in ProgressDialog,
	// allowing interaction with the actual dialog where a Cancel button may be visible.
	Ref<InputEventKey> k = p_event;
	if (k.is_valid()) {
		get_tree()->get_root()->set_input_as_handled();
	}
}

void EditorNode::shortcut_input(const Ref<InputEvent> &p_event) {
	ERR_FAIL_COND(p_event.is_null());

	Ref<InputEventKey> k = p_event;
	if ((k.is_valid() && k->is_pressed() && !k->is_echo()) || Object::cast_to<InputEventShortcut>(*p_event)) {
		bool is_handled = true;
		if (ED_IS_SHORTCUT("editor/filter_files", p_event)) {
			FileSystemDock::get_singleton()->focus_on_filter();
		} else if (ED_IS_SHORTCUT("editor/editor_2d", p_event)) {
			editor_main_screen->select(EditorMainScreen::EDITOR_2D);
		} else if (ED_IS_SHORTCUT("editor/editor_3d", p_event)) {
			editor_main_screen->select(EditorMainScreen::EDITOR_3D);
		} else if (ED_IS_SHORTCUT("editor/editor_script", p_event)) {
			editor_main_screen->select(EditorMainScreen::EDITOR_SCRIPT);
		} else if (ED_IS_SHORTCUT("editor/editor_game", p_event)) {
			editor_main_screen->select(EditorMainScreen::EDITOR_GAME);
		} else if (ED_IS_SHORTCUT("editor/editor_help", p_event)) {
			emit_signal(SNAME("request_help_search"), "");
		} else if (ED_IS_SHORTCUT("editor/editor_assetlib", p_event) && AssetLibraryEditorPlugin::is_available()) {
			editor_main_screen->select(EditorMainScreen::EDITOR_ASSETLIB);
		} else if (ED_IS_SHORTCUT("editor/editor_next", p_event)) {
			editor_main_screen->select_next();
		} else if (ED_IS_SHORTCUT("editor/editor_prev", p_event)) {
			editor_main_screen->select_prev();
		} else if (ED_IS_SHORTCUT("editor/command_palette", p_event)) {
			_open_command_palette();
		} else if (ED_IS_SHORTCUT("editor/toggle_last_opened_bottom_panel", p_event)) {
			bottom_panel->toggle_last_opened_bottom_panel();
		} else {
			is_handled = false;
		}

		if (is_handled) {
			get_tree()->get_root()->set_input_as_handled();
		}
	}
}

void EditorNode::_update_vsync_mode() {
	const DisplayServer::VSyncMode window_vsync_mode = DisplayServer::VSyncMode(int(EDITOR_GET("interface/editor/vsync_mode")));
	DisplayServer::get_singleton()->window_set_vsync_mode(window_vsync_mode);
}

void EditorNode::_update_from_settings() {
	if (!is_inside_tree()) {
		return;
	}
	_update_title();

	int current_filter = GLOBAL_GET("rendering/textures/canvas_textures/default_texture_filter");
	if (current_filter != scene_root->get_default_canvas_item_texture_filter()) {
		Viewport::DefaultCanvasItemTextureFilter tf = (Viewport::DefaultCanvasItemTextureFilter)current_filter;
		scene_root->set_default_canvas_item_texture_filter(tf);
	}
	int current_repeat = GLOBAL_GET("rendering/textures/canvas_textures/default_texture_repeat");
	if (current_repeat != scene_root->get_default_canvas_
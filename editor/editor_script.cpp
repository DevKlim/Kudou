#include "editor_script.h"

#include "editor/editor_interface.h"
#include "editor/editor_node.h"
#include "editor/editor_undo_redo_manager.h"
#include "editor/gui/editor_scene_tabs.h"
#include "scene/main/node.h"
#include "scene/resources/packed_scene.h"

void EditorScript::add_root_node(Node *p_node) {
	if (!EditorNode::get_singleton()) {
		EditorNode::add_io_error("EditorScript::add_root_node: " + TTR("Write your logic in the _run() method."));
		return;
	}

	if (EditorNode::get_singleton()->get_edited_scene()) {
		EditorNode::add_io_error("EditorScript::add_root_node: " + TTR("The current scene already has a root node."));
		return;
	}

	const String &scene_path = p_node->get_scene_file_path();
	if (!scene_path.is_empty()) {
		Ref<PackedScene> scene = ResourceLoader::load(scene_path);
		if (scene.is_valid()) {
			memfree(scene->instantiate(PackedScene::GEN_EDIT_STATE_INSTANCE)); // Ensure node cache.

			p_node->set_scene_inherited_state(scene->get_state());
			p_node->set_scene_file_path(String());
		}
	}

	EditorNode::get_singleton()->set_edited_scene(p_node);
	EditorUndoRedoManager::get_singleton()->set_history_as_unsaved(EditorNode::get_editor_data().get_current_edited_scene_history_id());
	EditorSceneTabs::get_singleton()->update_scene_tabs();
}

Node *EditorScript::get_scene() const {
	if (!EditorNode::get_singleton()) {
		EditorNode::add_io_error("EditorScript::get_scene: " + TTR("Write your logic in the _run() method."));
		return nullptr;
	}

	return EditorNode::get_singleton()->get_edited_scene();
}

EditorInterface *EditorScript::get_editor_interface() const {
	return EditorInterface::get_singleton();
}

void EditorScript::run() {
	GDVIRTUAL_CALL(_run);
}

void EditorScript::_bind_methods() {
	ClassDB::bind_method(D_METHOD("add_root_node", "node"), &EditorScript::add_root_node);
	ClassDB::bind_method(D_METHOD("get_scene"), &EditorScript::get_scene);
	ClassDB::bind_method(D_METHOD("get_editor_interface"), &EditorScript::get_editor_interface);

	GDVIRTUAL_BIND(_run);
}

#include "packed_scene_editor_plugin.h"

#include "editor/editor_node.h"
#include "scene/gui/button.h"
#include "scene/resources/packed_scene.h"

void PackedSceneEditor::_on_open_scene_pressed() {
	// Using deferred call because changing scene updates the Inspector and thus destroys this plugin.
	callable_mp(EditorNode::get_singleton(), &EditorNode::load_scene).call_deferred(packed_scene->get_path(), false, false, false, false);
}

void PackedSceneEditor::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_THEME_CHANGED: {
			open_scene_button->set_button_icon(get_editor_theme_icon(SNAME("PackedScene")));
		} break;
	}
}

PackedSceneEditor::PackedSceneEditor(Ref<PackedScene> &p_packed_scene) {
	packed_scene = p_packed_scene;

	open_scene_button = EditorInspector::create_inspector_action_button(TTR("Open Scene"));
	open_scene_button->connect(SceneStringName(pressed), callable_mp(this, &PackedSceneEditor::_on_open_scene_pressed));
	open_scene_button->set_disabled(!packed_scene->get_path().get_file().is_valid_filename());
	add_child(open_scene_button);

	add_child(memnew(Control)); // Add padding before the regular properties.
}

///////////////////////

bool EditorInspectorPluginPackedScene::can_handle(Object *p_object) {
	return Object::cast_to<PackedScene>(p_object) != nullptr;
}

void EditorInspectorPluginPackedScene::parse_begin(Object *p_object) {
	Ref<PackedScene> packed_scene(p_object);
	PackedSceneEditor *editor = memnew(PackedSceneEditor(packed_scene));
	add_custom_control(editor);
}

///////////////////////

PackedSceneEditorPlugin::PackedSceneEditorPlugin() {
	Ref<EditorInspectorPluginPackedScene> plugin;
	plugin.instantiate();
	add_inspector_plugin(plugin);
}

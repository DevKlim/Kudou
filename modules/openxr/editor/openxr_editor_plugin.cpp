#include "openxr_editor_plugin.h"

#include "../action_map/openxr_action_map.h"

#include "editor/editor_command_palette.h"
#include "editor/editor_node.h"
#include "editor/gui/editor_bottom_panel.h"

void OpenXREditorPlugin::edit(Object *p_node) {
	if (Object::cast_to<OpenXRActionMap>(p_node)) {
		String path = Object::cast_to<OpenXRActionMap>(p_node)->get_path();
		if (path.is_resource_file()) {
			action_map_editor->open_action_map(path);
		}
	}
}

bool OpenXREditorPlugin::handles(Object *p_node) const {
	return (Object::cast_to<OpenXRActionMap>(p_node) != nullptr);
}

void OpenXREditorPlugin::make_visible(bool p_visible) {
}

OpenXREditorPlugin::OpenXREditorPlugin() {
	action_map_editor = memnew(OpenXRActionMapEditor);
	EditorNode::get_bottom_panel()->add_item(TTRC("OpenXR Action Map"), action_map_editor, ED_SHORTCUT_AND_COMMAND("bottom_panels/toggle_openxr_action_map_bottom_panel", TTRC("Toggle OpenXR Action Map Bottom Panel")));

	binding_modifier_inspector_plugin = Ref<EditorInspectorPluginBindingModifier>(memnew(EditorInspectorPluginBindingModifier));
	EditorInspector::add_inspector_plugin(binding_modifier_inspector_plugin);

#ifndef ANDROID_ENABLED
	select_runtime = memnew(OpenXRSelectRuntime);
	add_control_to_container(CONTAINER_TOOLBAR, select_runtime);
#endif
}

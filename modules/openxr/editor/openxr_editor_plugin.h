#pragma once

#include "openxr_action_map_editor.h"
#include "openxr_binding_modifier_editor.h"
#include "openxr_select_runtime.h"

#include "editor/plugins/editor_plugin.h"

class OpenXREditorPlugin : public EditorPlugin {
	GDCLASS(OpenXREditorPlugin, EditorPlugin);

	OpenXRActionMapEditor *action_map_editor = nullptr;
	Ref<EditorInspectorPluginBindingModifier> binding_modifier_inspector_plugin = nullptr;
#ifndef ANDROID_ENABLED
	OpenXRSelectRuntime *select_runtime = nullptr;
#endif

public:
	virtual String get_plugin_name() const override { return "OpenXRPlugin"; }
	bool has_main_screen() const override { return false; }
	virtual void edit(Object *p_node) override;
	virtual bool handles(Object *p_node) const override;
	virtual void make_visible(bool p_visible) override;

	OpenXREditorPlugin();
};

#pragma once

#include "editor/editor_inspector.h"
#include "editor/plugins/editor_plugin.h"

class EditorInspectorToolButtonPlugin : public EditorInspectorPlugin {
	GDCLASS(EditorInspectorToolButtonPlugin, EditorInspectorPlugin);

	void _update_action_icon(Button *p_action_button, const String &p_action_icon);
	void _call_action(const Variant &p_object, const StringName &p_property);

public:
	virtual bool can_handle(Object *p_object) override;
	virtual bool parse_property(Object *p_object, const Variant::Type p_type, const String &p_path, const PropertyHint p_hint, const String &p_hint_text, const BitField<PropertyUsageFlags> p_usage, const bool p_wide = false) override;
};

class ToolButtonEditorPlugin : public EditorPlugin {
	GDCLASS(ToolButtonEditorPlugin, EditorPlugin);

public:
	virtual String get_plugin_name() const override { return "ToolButtonEditorPlugin"; }

	ToolButtonEditorPlugin();
};

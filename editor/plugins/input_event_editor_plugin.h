#pragma once

#include "editor/action_map_editor.h"
#include "editor/editor_inspector.h"
#include "editor/plugins/editor_plugin.h"

class InputEventConfigContainer : public VBoxContainer {
	GDCLASS(InputEventConfigContainer, VBoxContainer);

	Label *input_event_text = nullptr;
	Button *open_config_button = nullptr;

	Ref<InputEvent> input_event;
	InputEventConfigurationDialog *config_dialog = nullptr;

	void _config_dialog_confirmed();
	void _configure_pressed();

	void _event_changed();

protected:
	void _notification(int p_what);

public:
	void set_event(const Ref<InputEvent> &p_event);

	InputEventConfigContainer();
};

class EditorInspectorPluginInputEvent : public EditorInspectorPlugin {
	GDCLASS(EditorInspectorPluginInputEvent, EditorInspectorPlugin);

public:
	virtual bool can_handle(Object *p_object) override;
	virtual void parse_begin(Object *p_object) override;
};

class InputEventEditorPlugin : public EditorPlugin {
	GDCLASS(InputEventEditorPlugin, EditorPlugin);

public:
	virtual String get_plugin_name() const override { return "InputEvent"; }

	InputEventEditorPlugin();
};

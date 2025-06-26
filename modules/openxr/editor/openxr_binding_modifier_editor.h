#pragma once

#include "../action_map/openxr_action_map.h"
#include "../action_map/openxr_action_set.h"
#include "../action_map/openxr_binding_modifier.h"
#include "editor/editor_inspector.h"
#include "editor/editor_undo_redo_manager.h"
#include "scene/gui/box_container.h"
#include "scene/gui/button.h"
#include "scene/gui/label.h"
#include "scene/gui/panel_container.h"

class EditorPropertyActionSet : public EditorProperty {
	GDCLASS(EditorPropertyActionSet, EditorProperty);
	OptionButton *options = nullptr;

	void _option_selected(int p_which);

protected:
	virtual void _set_read_only(bool p_read_only) override;

public:
	void setup(const Ref<OpenXRActionMap> &p_action_map);
	virtual void update_property() override;
	void set_option_button_clip(bool p_enable);
	EditorPropertyActionSet();
};

class EditorPropertyBindingPath : public EditorProperty {
	GDCLASS(EditorPropertyBindingPath, EditorProperty);
	OptionButton *options = nullptr;

	void _option_selected(int p_which);

protected:
	virtual void _set_read_only(bool p_read_only) override;

public:
	void setup(const String &p_interaction_profile_path, Vector<OpenXRAction::ActionType> p_include_action_types);
	virtual void update_property() override;
	void set_option_button_clip(bool p_enable);
	EditorPropertyBindingPath();
};

class EditorInspectorPluginBindingModifier : public EditorInspectorPlugin {
	GDCLASS(EditorInspectorPluginBindingModifier, EditorInspectorPlugin);

public:
	virtual bool can_handle(Object *p_object) override;
	virtual bool parse_property(Object *p_object, const Variant::Type p_type, const String &p_path, const PropertyHint p_hint, const String &p_hint_text, const BitField<PropertyUsageFlags> p_usage, const bool p_wide) override;
};

class OpenXRBindingModifierEditor : public PanelContainer {
	GDCLASS(OpenXRBindingModifierEditor, PanelContainer);

private:
	HBoxContainer *header_hb = nullptr;
	Label *binding_modifier_title = nullptr;
	Button *rem_binding_modifier_btn = nullptr;
	EditorInspector *editor_inspector = nullptr;

protected:
	VBoxContainer *main_vb = nullptr;

	EditorUndoRedoManager *undo_redo;
	Ref<OpenXRBindingModifier> binding_modifier;
	Ref<OpenXRActionMap> action_map;

	static void _bind_methods();
	void _notification(int p_what);

	void _on_remove_binding_modifier();

public:
	Ref<OpenXRBindingModifier> get_binding_modifier() const { return binding_modifier; }

	virtual void setup(Ref<OpenXRActionMap> p_action_map, Ref<OpenXRBindingModifier> p_binding_modifier);

	OpenXRBindingModifierEditor();
};

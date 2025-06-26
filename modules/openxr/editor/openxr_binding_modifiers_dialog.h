#pragma once

#include "../action_map/openxr_action_map.h"
#include "../action_map/openxr_interaction_profile.h"
#include "../editor/openxr_binding_modifier_editor.h"
#include "editor/create_dialog.h"
#include "editor/editor_undo_redo_manager.h"
#include "scene/gui/box_container.h"
#include "scene/gui/button.h"
#include "scene/gui/dialogs.h"
#include "scene/gui/label.h"
#include "scene/gui/scroll_container.h"

class OpenXRBindingModifiersDialog : public AcceptDialog {
	GDCLASS(OpenXRBindingModifiersDialog, AcceptDialog);

private:
	ScrollContainer *binding_modifier_sc = nullptr;
	VBoxContainer *binding_modifiers_vb = nullptr;
	Label *binding_warning_label = nullptr;
	Button *add_binding_modifier_btn = nullptr;
	CreateDialog *create_dialog = nullptr;

	OpenXRBindingModifierEditor *_add_binding_modifier_editor(Ref<OpenXRBindingModifier> p_binding_modifier);
	void _create_binding_modifiers();

	void _on_add_binding_modifier();
	void _on_remove_binding_modifier(Object *p_binding_modifier_editor);
	void _on_dialog_created();

protected:
	EditorUndoRedoManager *undo_redo;
	Ref<OpenXRActionMap> action_map;
	Ref<OpenXRInteractionProfile> interaction_profile;
	Ref<OpenXRIPBinding> ip_binding;

	static void _bind_methods();
	void _notification(int p_what);

	// used for undo/redo
	void _do_add_binding_modifier_editor(OpenXRBindingModifierEditor *p_binding_modifier_editor);
	void _do_remove_binding_modifier_editor(OpenXRBindingModifierEditor *p_binding_modifier_editor);

public:
	OpenXRBindingModifiersDialog();

	void setup(Ref<OpenXRActionMap> p_action_map, Ref<OpenXRInteractionProfile> p_interaction_profile, Ref<OpenXRIPBinding> p_ip_binding = Ref<OpenXRIPBinding>());
};

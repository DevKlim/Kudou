#pragma once

#include "scene/gui/dialogs.h"

class LineEdit;
class ItemList;

class EditorLayoutsDialog : public ConfirmationDialog {
	GDCLASS(EditorLayoutsDialog, ConfirmationDialog);

	LineEdit *name = nullptr;
	ItemList *layout_names = nullptr;
	VBoxContainer *makevb = nullptr;

	void _line_gui_input(const Ref<InputEvent> &p_event);
	void _update_ok_disable_state();
	void _deselect_layout_names();

protected:
	static void _bind_methods();
	virtual void ok_pressed() override;
	virtual void _post_popup() override;

public:
	EditorLayoutsDialog();

	void set_name_line_enabled(bool p_enabled);
};

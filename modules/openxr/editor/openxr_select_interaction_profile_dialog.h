#pragma once

#include "scene/gui/box_container.h"
#include "scene/gui/dialogs.h"
#include "scene/gui/label.h"
#include "scene/gui/scroll_container.h"

class OpenXRSelectInteractionProfileDialog : public ConfirmationDialog {
	GDCLASS(OpenXRSelectInteractionProfileDialog, ConfirmationDialog);

private:
	String selected_interaction_profile;
	Dictionary ip_buttons;

	VBoxContainer *main_vb = nullptr;
	ScrollContainer *scroll = nullptr;
	Label *all_selected = nullptr;

protected:
	static void _bind_methods();
	void _notification(int p_what);

public:
	void _on_select_interaction_profile(const String p_interaction_profile);
	void open(PackedStringArray p_do_not_include);
	virtual void ok_pressed() override;

	OpenXRSelectInteractionProfileDialog();
};

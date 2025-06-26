#pragma once

#include "scene/gui/box_container.h"
#include "scene/gui/dialogs.h"
#include "scene/gui/menu_button.h"
#include "scene/gui/rich_text_label.h"

class EditorRunNative : public HBoxContainer {
	GDCLASS(EditorRunNative, HBoxContainer);

	RichTextLabel *result_dialog_log = nullptr;
	AcceptDialog *result_dialog = nullptr;
	ConfirmationDialog *run_native_confirm = nullptr;
	bool run_confirmed = false;

	MenuButton *remote_debug = nullptr;
	bool first = true;

	int resume_id = -1;

	void _confirm_run_native();

protected:
	static void _bind_methods();
	void _notification(int p_what);

public:
	Error start_run_native(int p_id);
	void resume_run_native();

	bool is_deploy_debug_remote_enabled() const;

	EditorRunNative();
};

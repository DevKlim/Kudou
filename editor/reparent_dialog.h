#pragma once

#include "scene/gui/dialogs.h"

class CheckBox;
class SceneTreeEditor;

class ReparentDialog : public ConfirmationDialog {
	GDCLASS(ReparentDialog, ConfirmationDialog);

	SceneTreeEditor *tree = nullptr;
	CheckBox *keep_transform = nullptr;

	void _reparent();
	void _cancel();

protected:
	void _notification(int p_what);
	static void _bind_methods();

public:
	void set_current(const HashSet<Node *> &p_selection);

	ReparentDialog();
};

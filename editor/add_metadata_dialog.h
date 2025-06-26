#pragma once

#include "scene/gui/dialogs.h"

class EditorValidationPanel;
class EditorVariantTypeOptionButton;
class LineEdit;

class AddMetadataDialog : public ConfirmationDialog {
	GDCLASS(AddMetadataDialog, ConfirmationDialog);

public:
	AddMetadataDialog();
	void open(const StringName p_title, List<StringName> &p_existing_metas);

	StringName get_meta_name();
	Variant get_meta_defval();

private:
	List<StringName> _existing_metas;

	void _check_meta_name();
	void _complete_init(const StringName &p_label);

	LineEdit *add_meta_name = nullptr;
	EditorVariantTypeOptionButton *add_meta_type = nullptr;
	EditorValidationPanel *validation_panel = nullptr;
};

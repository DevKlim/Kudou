#pragma once

#include "scene/gui/box_container.h"
#include "scene/gui/button.h"
#include "scene/gui/option_button.h"

class ImportDefaultsEditorSettings;
class EditorInspector;

class ImportDefaultsEditor : public VBoxContainer {
	GDCLASS(ImportDefaultsEditor, VBoxContainer)

	OptionButton *importers = nullptr;
	Button *save_defaults = nullptr;
	Button *reset_defaults = nullptr;

	EditorInspector *inspector = nullptr;

	ImportDefaultsEditorSettings *settings = nullptr;

	void _update_importer();
	void _importer_selected(int p_index);

	void _reset();
	void _save();

protected:
	void _notification(int p_what);

public:
	void clear();

	ImportDefaultsEditor();
	~ImportDefaultsEditor();
};

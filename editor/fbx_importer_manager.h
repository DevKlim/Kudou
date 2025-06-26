#pragma once

#include "editor/gui/editor_file_dialog.h"
#include "scene/gui/dialogs.h"
#include "scene/gui/line_edit.h"

class FBXImporterManager : public ConfirmationDialog {
	GDCLASS(FBXImporterManager, ConfirmationDialog)

	bool is_importing = false;

	Label *message = nullptr;
	LineEdit *fbx_path = nullptr;
	Button *fbx_path_browse = nullptr;
	EditorFileDialog *browse_dialog = nullptr;
	Label *path_status = nullptr;

	void _validate_path(const String &p_path);
	void _select_file(const String &p_path);
	void _path_confirmed();
	void _cancel_setup();
	void _browse_install();
	void _link_clicked();

	static FBXImporterManager *singleton;

protected:
	void _notification(int p_what);

public:
	static FBXImporterManager *get_singleton() { return singleton; }

	void show_dialog(bool p_exclusive = false);

	FBXImporterManager();
};

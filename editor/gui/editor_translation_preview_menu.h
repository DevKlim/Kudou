#pragma once

#include "scene/gui/popup_menu.h"

class EditorTranslationPreviewMenu : public PopupMenu {
	GDCLASS(EditorTranslationPreviewMenu, PopupMenu);

	void _prepare();
	void _pressed(int p_index);

protected:
	void _notification(int p_what);

public:
	EditorTranslationPreviewMenu();
};

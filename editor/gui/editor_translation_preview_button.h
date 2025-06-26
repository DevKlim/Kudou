#pragma once

#include "scene/gui/button.h"

class EditorTranslationPreviewButton : public Button {
	GDCLASS(EditorTranslationPreviewButton, Button);

	void _update();

protected:
	virtual void pressed() override;

	void _notification(int p_what);

public:
	EditorTranslationPreviewButton();
};

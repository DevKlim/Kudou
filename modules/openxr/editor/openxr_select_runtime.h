#pragma once

#include "scene/gui/option_button.h"

class OpenXRSelectRuntime : public OptionButton {
	GDCLASS(OpenXRSelectRuntime, OptionButton);

public:
	OpenXRSelectRuntime();

protected:
	void _notification(int p_notification);

private:
	void _update_items();
	void _on_item_selected(int p_which);
};

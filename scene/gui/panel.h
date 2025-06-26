#pragma once

#include "scene/gui/control.h"

class Panel : public Control {
	GDCLASS(Panel, Control);

	struct ThemeCache {
		Ref<StyleBox> panel_style;
	} theme_cache;

protected:
	void _notification(int p_what);
	static void _bind_methods();

public:
	Panel();
};

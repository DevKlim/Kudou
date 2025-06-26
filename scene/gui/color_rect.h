#pragma once

#include "scene/gui/control.h"

class ColorRect : public Control {
	GDCLASS(ColorRect, Control);

	Color color = Color(1, 1, 1);

protected:
	void _notification(int p_what);
	static void _bind_methods();

public:
	void set_color(const Color &p_color);
	Color get_color() const;
};

#pragma once

#include "scene/gui/control.h"
class Separator : public Control {
	GDCLASS(Separator, Control);

	struct ThemeCache {
		int separation = 0;
		Ref<StyleBox> separator_style;
	} theme_cache;

protected:
	Orientation orientation = Orientation::HORIZONTAL;

	void _notification(int p_what);
	static void _bind_methods();

public:
	virtual Size2 get_minimum_size() const override;

	Separator();
	~Separator();
};

class VSeparator : public Separator {
	GDCLASS(VSeparator, Separator);

public:
	VSeparator();
};

class HSeparator : public Separator {
	GDCLASS(HSeparator, Separator);

public:
	HSeparator();
};

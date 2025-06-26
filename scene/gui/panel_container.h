#pragma once

#include "scene/gui/container.h"

class PanelContainer : public Container {
	GDCLASS(PanelContainer, Container);

	struct ThemeCache {
		Ref<StyleBox> panel_style;
	} theme_cache;

protected:
	void _notification(int p_what);
	static void _bind_methods();

public:
	virtual Size2 get_minimum_size() const override;

	virtual Vector<int> get_allowed_size_flags_horizontal() const override;
	virtual Vector<int> get_allowed_size_flags_vertical() const override;

	PanelContainer();
};

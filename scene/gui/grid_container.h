#pragma once

#include "scene/gui/container.h"

class GridContainer : public Container {
	GDCLASS(GridContainer, Container);

	int columns = 1;

	struct ThemeCache {
		int h_separation = 0;
		int v_separation = 0;
	} theme_cache;

protected:
	void _notification(int p_what);
	static void _bind_methods();

public:
	void set_columns(int p_columns);
	int get_columns() const;
	virtual Size2 get_minimum_size() const override;

	int get_h_separation() const;
};

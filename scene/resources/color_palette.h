#pragma once

#include "core/io/resource.h"

class ColorPalette : public Resource {
	GDCLASS(ColorPalette, Resource)

private:
	PackedColorArray colors;

protected:
	static void _bind_methods();

public:
	void set_colors(const PackedColorArray &p_colors);
	PackedColorArray get_colors() const;
};

#include "color_palette.h"

void ColorPalette::set_colors(const PackedColorArray &p_colors) {
	colors = p_colors;
}

PackedColorArray ColorPalette::get_colors() const {
	return colors;
}

void ColorPalette::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_colors", "colors"), &ColorPalette::set_colors);
	ClassDB::bind_method(D_METHOD("get_colors"), &ColorPalette::get_colors);

	ADD_PROPERTY(PropertyInfo(Variant::PACKED_COLOR_ARRAY, "colors"), "set_colors", "get_colors");
}

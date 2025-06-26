#pragma once

#include "core/io/resource.h"
#include "core/object/gdvirtual.gen.inc"

class CanvasItem;

class StyleBox : public Resource {
	GDCLASS(StyleBox, Resource);
	RES_BASE_EXTENSION("stylebox");
	OBJ_SAVE_TYPE(StyleBox);

	float content_margin[4];

protected:
	static void _bind_methods();
	virtual float get_style_margin(Side p_side) const { return 0; }

	GDVIRTUAL2C_REQUIRED(_draw, RID, Rect2)
	GDVIRTUAL1RC(Rect2, _get_draw_rect, Rect2)
	GDVIRTUAL0RC(Size2, _get_minimum_size)
	GDVIRTUAL2RC(bool, _test_mask, Point2, Rect2)

public:
	virtual Size2 get_minimum_size() const;

	void set_content_margin(Side p_side, float p_value);
	void set_content_margin_all(float p_value);
	void set_content_margin_individual(float p_left, float p_top, float p_right, float p_bottom);
	float get_content_margin(Side p_side) const;

	float get_margin(Side p_side) const;
	Point2 get_offset() const;

	virtual void draw(RID p_canvas_item, const Rect2 &p_rect) const;
	virtual Rect2 get_draw_rect(const Rect2 &p_rect) const;

	CanvasItem *get_current_item_drawn() const;

	virtual bool test_mask(const Point2 &p_point, const Rect2 &p_rect) const;

	StyleBox();
};

class StyleBoxEmpty : public StyleBox {
	GDCLASS(StyleBoxEmpty, StyleBox);
	virtual float get_style_margin(Side p_side) const override { return 0; }

public:
	virtual void draw(RID p_canvas_item, const Rect2 &p_rect) const override {}
};

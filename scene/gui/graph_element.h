#pragma once

#include "scene/gui/container.h"

class GraphElement : public Container {
	GDCLASS(GraphElement, Container);

protected:
	bool selected = false;
	bool resizable = false;
	bool resizing = false;
	bool draggable = true;
	bool selectable = true;

	Vector2 drag_from;
	Vector2 resizing_from;
	Vector2 resizing_from_size;

	Vector2 position_offset;

	struct ThemeCache {
		Ref<Texture2D> resizer;
	} theme_cache;

#ifdef TOOLS_ENABLED
	void _edit_set_position(const Point2 &p_position) override;
#endif

protected:
	virtual void gui_input(const Ref<InputEvent> &p_ev) override;
	void _notification(int p_what);
	static void _bind_methods();

	virtual void _resort();

	void _validate_property(PropertyInfo &p_property) const;

public:
	void set_position_offset(const Vector2 &p_offset);
	Vector2 get_position_offset() const;

	void set_selected(bool p_selected);
	bool is_selected();

	void set_drag(bool p_drag);
	Vector2 get_drag_from();

	void set_resizable(bool p_enable);
	bool is_resizable() const;

	void set_draggable(bool p_draggable);
	bool is_draggable();

	void set_selectable(bool p_selectable);
	bool is_selectable();

	virtual Size2 get_minimum_size() const override;

	bool is_resizing() const {
		return resizing;
	}
};

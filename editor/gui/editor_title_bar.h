#pragma once

#include "scene/gui/box_container.h"
#include "scene/main/window.h"

class EditorTitleBar : public HBoxContainer {
	GDCLASS(EditorTitleBar, HBoxContainer);

	Point2i click_pos;
	bool moving = false;
	bool can_move = false;
	Control *center_control = nullptr;

protected:
	void _notification(int p_what);

	virtual void gui_input(const Ref<InputEvent> &p_event) override;
	static void _bind_methods() {}

public:
	void set_center_control(Control *p_center_control);
	Control *get_center_control() const;

	void set_can_move_window(bool p_enabled);
	bool get_can_move_window() const;
};

#pragma once

#include "scene/gui/control.h"
#include "scene/main/window.h"

#include "tests/test_macros.h"

namespace TestWindow {

class NotificationControlWindow : public Control {
	GDCLASS(NotificationControlWindow, Control);

protected:
	void _notification(int p_what) {
		switch (p_what) {
			case NOTIFICATION_MOUSE_ENTER: {
				mouse_over = true;
			} break;

			case NOTIFICATION_MOUSE_EXIT: {
				mouse_over = false;
			} break;
		}
	}

public:
	bool mouse_over = false;
};

TEST_CASE("[SceneTree][Window]") {
	Window *root = SceneTree::get_singleton()->get_root();

	SUBCASE("Control-mouse-over within Window-black bars should not happen") {
		Window *w = memnew(Window);
		root->add_child(w);
		w->set_size(Size2i(400, 200));
		w->set_position(Size2i(0, 0));
		w->set_content_scale_size(Size2i(200, 200));
		w->set_content_scale_mode(Window::CONTENT_SCALE_MODE_CANVAS_ITEMS);
		w->set_content_scale_aspect(Window::CONTENT_SCALE_ASPECT_KEEP);
		NotificationControlWindow *c = memnew(NotificationControlWindow);
		w->add_child(c);
		c->set_size(Size2i(100, 100));
		c->set_position(Size2i(-50, -50));

		CHECK_FALSE(c->mouse_over);
		SEND_GUI_MOUSE_MOTION_EVENT(Point2i(110, 10), MouseButtonMask::NONE, Key::NONE);
		CHECK(c->mouse_over);
		SEND_GUI_MOUSE_MOTION_EVENT(Point2i(90, 10), MouseButtonMask::NONE, Key::NONE);
		CHECK_FALSE(c->mouse_over); // GH-80011

		/* TODO:
		SEND_GUI_MOUSE_BUTTON_EVENT(Point2i(90, 10), MouseButton::LEFT, MouseButtonMask::LEFT, Key::NONE);
		SEND_GUI_MOUSE_BUTTON_RELEASED_EVENT(Point2i(90, 10), MouseButton::LEFT, MouseButtonMask::NONE, Key::NONE);
		CHECK(Control was not pressed);
		*/

		memdelete(c);
		memdelete(w);
	}
}

} // namespace TestWindow

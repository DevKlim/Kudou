#include "core/error/error_macros.h"
#include "core/math/math_funcs.h"
#include "core/os/keyboard.h"
#include "core/typedefs.h"
#include "helper_files.hpp"

// Guards for CEF conflicts are now in gdbrowser.hpp (via ad_blocker.hpp)

#include "gdbrowser.hpp"

//------------------------------------------------------------------------------
void GDBrowserView::leftClick() {
	leftMouseDown();
	leftMouseUp();
}

//------------------------------------------------------------------------------
void GDBrowserView::rightClick() {
	rightMouseDown();
	rightMouseUp();
}

//------------------------------------------------------------------------------
void GDBrowserView::middleClick() {
	middleMouseDown();
	middleMouseUp();
}

//------------------------------------------------------------------------------
void GDBrowserView::leftMouseDown() {
	if (!m_browser) {
		return;
	}

	m_left_click_count = CLAMP(m_left_click_count + 1, 1, 3);

	using namespace std::chrono;
	system_clock::time_point now = system_clock::now();
	int64_t click_interval_ms =
			duration_cast<milliseconds>(now - m_last_left_down).count();
	m_last_left_down = now;
	if (click_interval_ms > 500) {
		m_left_click_count = 1;
	}

	m_mouse_event_modifiers |= EVENTFLAG_LEFT_MOUSE_BUTTON;

	CefBrowserHost::MouseButtonType btn =
			CefBrowserHost::MouseButtonType::MBT_LEFT;
	CefMouseEvent evt;
	evt.x = m_mouse_x;
	evt.y = m_mouse_y;
	evt.modifiers = m_mouse_event_modifiers;

	m_browser->GetHost()->SendMouseClickEvent(
			evt, btn, false, m_left_click_count);
}

//------------------------------------------------------------------------------
void GDBrowserView::rightMouseDown() {
	if (!m_browser) {
		return;
	}

	m_mouse_event_modifiers |= EVENTFLAG_RIGHT_MOUSE_BUTTON;

	CefBrowserHost::MouseButtonType btn =
			CefBrowserHost::MouseButtonType::MBT_RIGHT;
	CefMouseEvent evt;
	evt.x = m_mouse_x;
	evt.y = m_mouse_y;
	evt.modifiers = m_mouse_event_modifiers;

	m_browser->GetHost()->SendMouseClickEvent(evt, btn, false, 1);
}

//------------------------------------------------------------------------------
void GDBrowserView::leftMouseUp() {
	if (!m_browser) {
		return;
	}

	m_mouse_event_modifiers &= ~EVENTFLAG_LEFT_MOUSE_BUTTON;

	CefBrowserHost::MouseButtonType btn =
			CefBrowserHost::MouseButtonType::MBT_LEFT;
	CefMouseEvent evt;
	evt.x = m_mouse_x;
	evt.y = m_mouse_y;
	evt.modifiers = m_mouse_event_modifiers;

	m_browser->GetHost()->SendMouseClickEvent(evt, btn, true, 1);
}

//------------------------------------------------------------------------------
void GDBrowserView::rightMouseUp() {
	if (!m_browser) {
		return;
	}

	m_mouse_event_modifiers &= ~EVENTFLAG_RIGHT_MOUSE_BUTTON;

	CefBrowserHost::MouseButtonType btn =
			CefBrowserHost::MouseButtonType::MBT_RIGHT;
	CefMouseEvent evt;
	evt.x = m_mouse_x;
	evt.y = m_mouse_y;
	evt.modifiers = m_mouse_event_modifiers;

	m_browser->GetHost()->SendMouseClickEvent(evt, btn, true, 1);
}

//------------------------------------------------------------------------------
void GDBrowserView::middleMouseDown() {
	if (!m_browser) {
		return;
	}

	m_mouse_event_modifiers |= EVENTFLAG_MIDDLE_MOUSE_BUTTON;

	CefBrowserHost::MouseButtonType btn =
			CefBrowserHost::MouseButtonType::MBT_MIDDLE;
	CefMouseEvent evt;
	evt.x = m_mouse_x;
	evt.y = m_mouse_y;
	evt.modifiers = m_mouse_event_modifiers;

	m_browser->GetHost()->SendMouseClickEvent(evt, btn, false, 1);
}

//------------------------------------------------------------------------------
void GDBrowserView::middleMouseUp() {
	if (!m_browser) {
		return;
	}

	m_mouse_event_modifiers &= ~EVENTFLAG_MIDDLE_MOUSE_BUTTON;

	CefBrowserHost::MouseButtonType btn =
			CefBrowserHost::MouseButtonType::MBT_MIDDLE;
	CefMouseEvent evt;
	evt.x = m_mouse_x;
	evt.y = m_mouse_y;
	evt.modifiers = m_mouse_event_modifiers;

	m_browser->GetHost()->SendMouseClickEvent(evt, btn, true, 1);
}

//------------------------------------------------------------------------------
void GDBrowserView::mouseMove(int x, int y) {
	if (!m_browser) {
		return;
	}

	m_mouse_x = x;
	m_mouse_y = y;

	CefMouseEvent evt;
	evt.x = x;
	evt.y = y;
	evt.modifiers = m_mouse_event_modifiers;

	bool mouse_leave = false;
	auto host = m_browser->GetHost();
	host->SetFocus(true);
	host->SendMouseMoveEvent(evt, mouse_leave);
}

//------------------------------------------------------------------------------
void GDBrowserView::mouseWheelVertical(const int wDelta) {
	if (m_browser == nullptr) {
		return;
	}

	CefMouseEvent evt;
	evt.x = m_mouse_x;
	evt.y = m_mouse_y;
	evt.modifiers = m_mouse_event_modifiers;

	m_browser->GetHost()->SendMouseWheelEvent(evt, 0, wDelta * 10);
}

//------------------------------------------------------------------------------
void GDBrowserView::mouseWheelHorizontal(const int wDelta) {
	if (m_browser == nullptr) {
		return;
	}

	CefMouseEvent evt;
	evt.x = m_mouse_x;
	evt.y = m_mouse_y;
	evt.modifiers = m_mouse_event_modifiers;

	m_browser->GetHost()->SendMouseWheelEvent(evt, wDelta * 10, 0);
}

//------------------------------------------------------------------------------
static uint32_t getKeyboardModifiers(bool shift, bool alt, bool ctrl) {
	uint32_t modifiers = 0;

	if (shift == true) {
		modifiers |= EVENTFLAG_SHIFT_DOWN;
	}
	if (ctrl == true) {
		modifiers |= EVENTFLAG_CONTROL_DOWN;
	}
	if (alt == true) {
		modifiers |= EVENTFLAG_ALT_DOWN;
	}

	return modifiers;
}

//------------------------------------------------------------------------------
void GDBrowserView::keyPress(int key,
		bool pressed,
		bool shift,
		bool alt,
		bool ctrl) {
	if (!m_browser) {
		return;
	}

	CefKeyEvent event;
	char16_t key16b = char16_t(key);
	if (pressed == true) {
		event.modifiers = getKeyboardModifiers(shift, alt, ctrl);

		if ((key >= 32) && (key <= 126)) {
			event.windows_key_code = key;
			event.character = key16b;
			event.unmodified_character = key16b;
			event.type = KEYEVENT_CHAR;
			m_browser->GetHost()->SendKeyEvent(event);
		} else if (key == (int)Key::BACKSPACE || key == (int)Key::ENTER ||
				key == (int)Key::KP_ENTER) {
			if (key == (int)Key::BACKSPACE) {
				event.windows_key_code = 8;
				event.character = 8;
				event.unmodified_character = 8;
			} else if (key == (int)Key::ENTER) {
				event.windows_key_code = 13;
				event.character = 13;
				event.unmodified_character = 13;
			} else if (key == (int)Key::KP_ENTER) {
				event.windows_key_code = 13;
				event.character = 13;
				event.unmodified_character = 13;
			}

			event.character = char16_t(event.windows_key_code);
			event.native_key_code = event.windows_key_code;
			event.type = KEYEVENT_KEYDOWN;
			m_browser->GetHost()->SendKeyEvent(event);
			event.type = KEYEVENT_CHAR;
			m_browser->GetHost()->SendKeyEvent(event);
		} else if (key >= 320 && key <= 329) {
			event.windows_key_code = key;
			event.character = key16b;
			event.native_key_code = key;

			event.type = KEYEVENT_KEYDOWN;
			m_browser->GetHost()->SendKeyEvent(event);
			event.type = KEYEVENT_CHAR;
			m_browser->GetHost()->SendKeyEvent(event);
		} else if (key == (int)Key::RIGHT || key == (int)Key::LEFT ||
				key == (int)Key::UP || key == (int)Key::DOWN ||
				key == (int)Key::PAGEUP || key == (int)Key::PAGEDOWN ||
				key == (int)Key::HOME || key == (int)Key::END ||
				key == (int)Key::INSERT || key == (int)Key::KEY_DELETE) {
			if (key == (int)Key::RIGHT) {
				event.windows_key_code = 39;
			} else if (key == (int)Key::LEFT) {
				event.windows_key_code = 37;
			} else if (key == (int)Key::UP) {
				event.windows_key_code = 38;
			} else if (key == (int)Key::DOWN) {
				event.windows_key_code = 40;
			} else if (key == (int)Key::PAGEUP) {
				event.windows_key_code = 33;
			} else if (key == (int)Key::PAGEDOWN) {
				event.windows_key_code = 34;
			} else if (key == (int)Key::HOME) {
				event.windows_key_code = 36;
			} else if (key == (int)Key::END) {
				event.windows_key_code = 35;
			} else if (key == (int)Key::INSERT) {
				event.windows_key_code = 45;
			} else if (key == (int)Key::KEY_DELETE) {
				event.windows_key_code = 46;
			}

			event.type = KEYEVENT_KEYDOWN;
			event.character = char16_t(event.windows_key_code);
			event.native_key_code = event.windows_key_code;
			m_browser->GetHost()->SendKeyEvent(event);
		} else {
			event.windows_key_code = key;
			event.character = key16b;
			event.native_key_code = key;
			event.unmodified_character = key16b;

			event.type = KEYEVENT_KEYDOWN;
			m_browser->GetHost()->SendKeyEvent(event);
			event.type = pressed ? KEYEVENT_CHAR : KEYEVENT_KEYUP;
			m_browser->GetHost()->SendKeyEvent(event);
		}
	} else {
		event.native_key_code |= int(0xC0000000);
		event.type = KEYEVENT_KEYUP;
		m_browser->GetHost()->SendKeyEvent(event);
	}
}
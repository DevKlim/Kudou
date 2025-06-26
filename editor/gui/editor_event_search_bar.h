#pragma once

#include "scene/gui/box_container.h"

class Button;
class EventListenerLineEdit;
class LineEdit;

class EditorEventSearchBar : public HBoxContainer {
	GDCLASS(EditorEventSearchBar, HBoxContainer);

	LineEdit *search_by_name = nullptr;
	EventListenerLineEdit *search_by_event = nullptr;
	Button *clear_all = nullptr;

	void _on_event_changed(const Ref<InputEvent> &p_event);
	void _on_clear_all();

	void _value_changed();

protected:
	void _notification(int p_what);
	static void _bind_methods();

public:
	LineEdit *get_name_search_box() const { return search_by_name; }

	bool is_searching() const;

	String get_name() const;
	Ref<InputEvent> get_event() const;

	EditorEventSearchBar();
};

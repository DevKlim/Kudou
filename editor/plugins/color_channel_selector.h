#pragma once

#include "scene/gui/box_container.h"

class PanelContainer;
class Button;

class ColorChannelSelector : public HBoxContainer {
	GDCLASS(ColorChannelSelector, HBoxContainer);

	static const unsigned int CHANNEL_COUNT = 4;

public:
	ColorChannelSelector();

	void set_available_channels_mask(uint32_t p_mask);
	uint32_t get_selected_channels_mask() const;
	Vector4 get_selected_channel_factors() const;

private:
	void _notification(int p_what);

	void on_channel_button_toggled(bool p_unused_pressed);
	void create_button(unsigned int p_channel_index, const String &p_text, Control *p_parent);
	void on_toggled(bool p_pressed);

	static void _bind_methods();

	Button *channel_buttons[CHANNEL_COUNT] = {};
	PanelContainer *panel = nullptr;
	Button *toggle_button = nullptr;
};

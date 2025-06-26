#pragma once

#include "scene/gui/popup.h"

class Label;
class VBoxContainer;
class Font;

class CreditsRoll : public Popup {
	GDCLASS(CreditsRoll, Popup);

	enum class LabelSize {
		NORMAL,
		HEADER,
		BIG_HEADER,
	};

	int font_size_normal = 0;
	int font_size_header = 0;
	int font_size_big_header = 0;
	Ref<Font> bold_font;

	bool mouse_enabled = false;
	VBoxContainer *content = nullptr;
	Label *project_manager = nullptr;

	Label *_create_label(const String &p_with_text, LabelSize p_size = LabelSize::NORMAL);
	void _create_nothing(int p_size = -1);
	String _build_string(const char *const *p_from) const;

protected:
	void _notification(int p_what);

public:
	void roll_credits();

	CreditsRoll();
};

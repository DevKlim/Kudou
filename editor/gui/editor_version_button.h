#pragma once

#include "scene/gui/link_button.h"

class EditorVersionButton : public LinkButton {
	GDCLASS(EditorVersionButton, LinkButton);

public:
	enum VersionFormat {
		// 4.3.2.stable
		FORMAT_BASIC,
		// v4.3.2.stable.mono [HASH]
		FORMAT_WITH_BUILD,
		// Godot Engine v4.3.2.stable.mono.official [HASH]
		FORMAT_WITH_NAME_AND_BUILD,
	};

private:
	VersionFormat format = FORMAT_WITH_NAME_AND_BUILD;

protected:
	void _notification(int p_what);

	virtual void pressed() override;

public:
	EditorVersionButton(VersionFormat p_format);
};

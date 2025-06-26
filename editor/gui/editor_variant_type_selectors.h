#pragma once

#include "core/variant/variant.h"
#include "scene/gui/option_button.h"
#include "scene/gui/popup_menu.h"

class EditorVariantTypeOptionButton : public OptionButton {
	GDCLASS(EditorVariantTypeOptionButton, OptionButton);

	void _update_menu_icons();

protected:
	void _notification(int p_what);

public:
	Variant::Type get_selected_type() const;

	void populate(const LocalVector<Variant::Type> &p_disabled_types, const HashMap<Variant::Type, String> &p_renames = {});
};

class EditorVariantTypePopupMenu : public PopupMenu {
	GDCLASS(EditorVariantTypePopupMenu, PopupMenu);

	bool remove_item = false;
	bool icons_dirty = true;

	void _populate();
	void _update_menu_icons();

protected:
	void _notification(int p_what);

public:
	virtual void popup(const Rect2i &p_bounds = Rect2i()) override;

	EditorVariantTypePopupMenu(bool p_remove_item);
};

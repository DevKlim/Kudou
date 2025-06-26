#pragma once

#include "scene/gui/box_container.h"
#include "scene/gui/button.h"
#include "scene/gui/label.h"
#include "scene/gui/popup_menu.h"
#include "scene/gui/texture_rect.h"

class EditorSelectionHistory;

class EditorObjectSelector : public Button {
	GDCLASS(EditorObjectSelector, Button);

	EditorSelectionHistory *history = nullptr;

	TextureRect *current_object_icon = nullptr;
	Label *current_object_label = nullptr;
	TextureRect *sub_objects_icon = nullptr;
	PopupMenu *sub_objects_menu = nullptr;

	Vector<ObjectID> objects;

	void _show_popup();
	void _id_pressed(int p_idx);
	void _about_to_show();
	void _add_children_to_popup(Object *p_obj, int p_depth = 0);

protected:
	void _notification(int p_what);

public:
	virtual Size2 get_minimum_size() const override;

	void update_path();
	void clear_path();
	void enable_path();

	EditorObjectSelector(EditorSelectionHistory *p_history);
};

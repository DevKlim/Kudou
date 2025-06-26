#pragma once

#include "scene/gui/dialogs.h"

class CreditsRoll;
class ItemList;
class Label;
class RichTextLabel;
class TextureRect;
class Tree;

/**
 * NOTE: Do not assume the EditorNode singleton to be available in this class' methods.
 * EditorAbout is also used from the project manager where EditorNode isn't initialized.
 */
class EditorAbout : public AcceptDialog {
	GDCLASS(EditorAbout, AcceptDialog);

private:
	enum SectionFlags {
		FLAG_SINGLE_COLUMN = 1 << 0,
		FLAG_ALLOW_WEBSITE = 1 << 1,
		FLAG_EASTER_EGG = 1 << 2,
	};

	void _license_tree_selected();
	void _item_activated(int p_idx, ItemList *p_il);
	void _item_list_resized(ItemList *p_il);
	Label *_create_section(Control *p_parent, const String &p_name, const char *const *p_src, BitField<SectionFlags> p_flags = 0);

	Label *_about_text_label = nullptr;
	Label *_project_manager_label = nullptr;
	Tree *_tpl_tree = nullptr;
	RichTextLabel *license_text_label = nullptr;
	RichTextLabel *_tpl_text = nullptr;
	TextureRect *_logo = nullptr;
	Vector<ItemList *> name_lists;
	CreditsRoll *credits_roll = nullptr;

protected:
	void _notification(int p_what);

public:
	EditorAbout();
};

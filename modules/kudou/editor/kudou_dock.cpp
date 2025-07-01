#include "kudou_dock.h"

#include "editor/themes/editor_scale.h"
#include "scene/gui/box_container.h"
#include "scene/gui/rich_text_label.h"
#include "scene/gui/text_edit.h"

KudouDock::KudouDock() {
    VBoxContainer *main_vbox = memnew(VBoxContainer);
    add_child(main_vbox);

    RichTextLabel *chat_box = memnew(RichTextLabel);
    chat_box->set_v_size_flags(Control::SIZE_EXPAND_FILL);
    main_vbox->add_child(chat_box);

    TextEdit *input_field = memnew(TextEdit);
    input_field->set_custom_minimum_size(Vector2(0, 100 * EDSCALE));
    main_vbox->add_child(input_field);
}

void KudouDock::_notification(int p_what) {}

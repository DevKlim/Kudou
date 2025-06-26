#include "editor/gui/editor_translation_preview_menu.h"

#include "core/string/translation_server.h"
#include "editor/editor_node.h"

void EditorTranslationPreviewMenu::_prepare() {
	const String current_preview_locale = EditorNode::get_singleton()->get_preview_locale();

	clear();
	reset_size();

	add_radio_check_item(TTRC("None"));
	set_item_metadata(-1, "");
	if (current_preview_locale.is_empty()) {
		set_item_checked(-1, true);
	}

	const Vector<String> locales = TranslationServer::get_singleton()->get_loaded_locales();
	if (!locales.is_empty()) {
		add_separator();
	}
	for (const String &locale : locales) {
		const String name = TranslationServer::get_singleton()->get_locale_name(locale);
		add_radio_check_item(name == locale ? name : name + " [" + locale + "]");
		set_item_auto_translate_mode(-1, AUTO_TRANSLATE_MODE_DISABLED);
		set_item_metadata(-1, locale);
		if (locale == current_preview_locale) {
			set_item_checked(-1, true);
		}
	}
}

void EditorTranslationPreviewMenu::_pressed(int p_index) {
	for (int i = 0; i < get_item_count(); i++) {
		set_item_checked(i, i == p_index);
	}
	EditorNode::get_singleton()->set_preview_locale(get_item_metadata(p_index));
}

void EditorTranslationPreviewMenu::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_READY: {
			connect("about_to_popup", callable_mp(this, &EditorTranslationPreviewMenu::_prepare));
			connect("index_pressed", callable_mp(this, &EditorTranslationPreviewMenu::_pressed));
		} break;
	}
}

EditorTranslationPreviewMenu::EditorTranslationPreviewMenu() {
	set_hide_on_checkable_item_selection(false);
}

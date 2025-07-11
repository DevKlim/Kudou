
#include "web_browser.h"
#include "scene/gui/rich_text_label.h"
#include "core/string/print_string.h"

void WebBrowser::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_ENTER_TREE:
			print_line("WebBrowser: _notification(NOTIFICATION_ENTER_TREE)");
			break;
		case NOTIFICATION_EXIT_TREE:
			print_line("WebBrowser: _notification(NOTIFICATION_EXIT_TREE)");
			break;
		case NOTIFICATION_READY:
			print_line("WebBrowser: _notification(NOTIFICATION_READY)");
			break;
		default:
			break;
	}
}

void WebBrowser::_bind_methods() {
    ClassDB::bind_method(D_METHOD("load_url", "url"), &WebBrowser::load_url);
}

void WebBrowser::load_url(const String &p_url) {
    RichTextLabel *label = Object::cast_to<RichTextLabel>(get_child(0));
    label->set_text(vformat("Web view not available. URL: %s", p_url));
}

WebBrowser::WebBrowser() {
    print_line("WebBrowser: Constructor");
    set_name("Web Browser");
    RichTextLabel *label = memnew(RichTextLabel);
    label->set_anchors_and_offsets_preset(Control::PRESET_FULL_RECT);
    add_child(label);
}

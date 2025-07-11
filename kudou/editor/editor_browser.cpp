#include "editor_browser.h"

#include "../gdbrowser.hpp"
#include "../gdcef.hpp"

#include "core/string/print_string.h"

void EditorBrowser::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_READY: {
			call_deferred("initialize_cef_deferred");
			break;
		}
		default:
			break;
	}
}

void EditorBrowser::_bind_methods() {
	ClassDB::bind_method("initialize_cef_deferred", &EditorBrowser::initialize_cef_deferred);
}

void EditorBrowser::initialize_cef_deferred() {
	print_line("EditorBrowser: Initializing CEF");
	cef = memnew(GDCef);
	add_child(cef);

	Dictionary config;
	if (cef->initialize(config)) {
		print_line("EditorBrowser: CEF initialized successfully");
		browser_view = cef->createBrowser("https://www.google.com", texture_rect, config);
		if (browser_view) {
			print_line("EditorBrowser: Browser created successfully");
		} else {
			print_error("EditorBrowser: Failed to create browser");
		}
	} else {
			print_error("EditorBrowser: Failed to initialize CEF");
	}
}

void EditorBrowser::load_url(const String &p_url) {
	if (browser_view) {
		browser_view->loadURL(p_url);
	}
}

EditorBrowser::EditorBrowser() {
	print_line("--- EDITOR BROWSER CONSTRUCTOR ---");
	texture_rect = memnew(TextureRect);
	texture_rect->set_anchors_and_offsets_preset(Control::PRESET_FULL_RECT);
	add_child(texture_rect);
}

EditorBrowser::~EditorBrowser() {
}

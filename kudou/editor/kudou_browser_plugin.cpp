#include "kudou_browser_plugin.h"

#include "editor/editor_interface.h"
#include "editor/editor_node.h"
#include "editor/editor_string_names.h"

#include "editor_browser.h"

const Ref<Texture2D> KudouBrowserPlugin::get_plugin_icon() const {
	return EditorInterface::get_singleton()->get_base_control()->get_theme_icon(SNAME("Window"), EditorStringName(EditorIcons));
}

void KudouBrowserPlugin::_notification(int p_what) {
	if (p_what == NOTIFICATION_ENTER_TREE) {
		// Initialization logic can go here if needed.
	}
}

void KudouBrowserPlugin::make_visible(bool p_visible) {
	if (browser_view) {
		browser_view->set_visible(p_visible);
	}
}

// **THE FIX for C3668**: Removed 'const' from the definition.
Control *KudouBrowserPlugin::get_editor_main_screen() {
	return browser_view;
}

KudouBrowserPlugin::KudouBrowserPlugin() {
	print_line("--- KUDOU BROWSER PLUGIN CONSTRUCTOR ---");
	browser_view = memnew(EditorBrowser);
	browser_view->set_name("KudouWebView");
	// DO NOT call add_child or add_main_plugin here. The editor will do this
	// by calling get_editor_main_screen() when the plugin is enabled.
}

KudouBrowserPlugin::~KudouBrowserPlugin() {
	// The plugin owns the main screen control, so it should delete it.
	if (browser_view) {
		memdelete(browser_view);
		browser_view = nullptr;
	}
}
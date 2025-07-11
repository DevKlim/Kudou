#ifndef KUDOU_BROWSER_PLUGIN_H
#define KUDOU_BROWSER_PLUGIN_H

#include "editor/plugins/editor_plugin.h"

class EditorBrowser;

class KudouBrowserPlugin : public EditorPlugin {
	GDCLASS(KudouBrowserPlugin, EditorPlugin);

private:
	EditorBrowser *browser_view = nullptr;

protected:
	void _notification(int p_what);

public:
	virtual String get_plugin_name() const override { return TTR("Web"); }
	virtual const Ref<Texture2D> get_plugin_icon() const override;
	virtual bool has_main_screen() const override { return true; }
	virtual void make_visible(bool p_visible) override;
	// **THE FIX for C3668**: Removed 'const' to match the base class method signature.
	virtual Control *get_editor_main_screen();

	EditorBrowser *get_browser_view() const { return browser_view; }

	KudouBrowserPlugin();
	~KudouBrowserPlugin();
};

#endif // KUDOU_BROWSER_PLUGIN_H
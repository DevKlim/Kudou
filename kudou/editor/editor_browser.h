#ifndef EDITOR_BROWSER_H
#define EDITOR_BROWSER_H

#include "scene/gui/control.h"
#include "scene/gui/texture_rect.h"



class GDCef;
class GDBrowserView;

class EditorBrowser : public Control {
	GDCLASS(EditorBrowser, Control);

private:
	GDCef *cef = nullptr;
	GDBrowserView *browser_view = nullptr;
	TextureRect *texture_rect = nullptr;

protected:
	void _notification(int p_what);
	static void _bind_methods();

public:
	void initialize_cef_deferred();
	void load_url(const String &p_url);
	EditorBrowser();
	~EditorBrowser();
};

#endif // EDITOR_BROWSER_H
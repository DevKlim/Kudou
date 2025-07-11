
#ifndef WEB_BROWSER_H
#define WEB_BROWSER_H

#include "scene/gui/control.h"

class WebBrowser : public Control {
    GDCLASS(WebBrowser, Control);

protected:
    void _notification(int p_what);
    static void _bind_methods();

public:
    void load_url(const String &p_url);

    WebBrowser();
};

#endif // WEB_BROWSER_H

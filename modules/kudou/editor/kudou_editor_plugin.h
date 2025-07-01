#pragma once

#include "editor/plugins/editor_plugin.h"

class KudouEditorPlugin : public EditorPlugin {
    GDCLASS(KudouEditorPlugin, EditorPlugin);

public:
    KudouEditorPlugin();

    virtual String get_plugin_name() const override { return "Kudou"; }

protected:
    void _notification(int p_what);
};

#include "kudou_editor_plugin.h"

#include "editor/editor_node.h"
#include "editor/themes/editor_scale.h"
#include "kudou_dock.h"
#include "core/input/shortcut.h"

KudouEditorPlugin::KudouEditorPlugin() {}

void KudouEditorPlugin::_notification(int p_what) {
    if (p_what == NOTIFICATION_ENTER_TREE) {
        KudouDock *dock = memnew(KudouDock);
        add_control_to_dock(DOCK_SLOT_RIGHT_UL, dock);
    }
}

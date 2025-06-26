#pragma once

#include "editor/plugins/editor_plugin.h"

#include "navigation_link_3d_gizmo_plugin.h"

class NavigationLink3DEditorPlugin : public EditorPlugin {
	GDCLASS(NavigationLink3DEditorPlugin, EditorPlugin);

	Ref<NavigationLink3DGizmoPlugin> gizmo_plugin;

public:
	virtual String get_plugin_name() const override { return "NavigationLink3D"; }
	bool has_main_screen() const override { return false; }
	virtual void edit(Object *p_object) override;
	virtual bool handles(Object *p_object) const override;

	NavigationLink3DEditorPlugin();
};

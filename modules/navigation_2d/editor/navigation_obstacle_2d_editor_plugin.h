#pragma once

#include "editor/plugins/abstract_polygon_2d_editor.h"
#include "scene/2d/navigation/navigation_obstacle_2d.h"

class NavigationObstacle2DEditor : public AbstractPolygon2DEditor {
	GDCLASS(NavigationObstacle2DEditor, AbstractPolygon2DEditor);

	NavigationObstacle2D *node = nullptr;

protected:
	virtual Node2D *_get_node() const override;
	virtual void _set_node(Node *p_polygon) override;

	virtual Variant _get_polygon(int p_idx) const override;
	virtual void _set_polygon(int p_idx, const Variant &p_polygon) const override;

	virtual void _action_add_polygon(const Variant &p_polygon) override;
	virtual void _action_remove_polygon(int p_idx) override;
	virtual void _action_set_polygon(int p_idx, const Variant &p_previous, const Variant &p_polygon) override;
};

class NavigationObstacle2DEditorPlugin : public AbstractPolygon2DEditorPlugin {
	GDCLASS(NavigationObstacle2DEditorPlugin, AbstractPolygon2DEditorPlugin);

public:
	NavigationObstacle2DEditorPlugin();
};

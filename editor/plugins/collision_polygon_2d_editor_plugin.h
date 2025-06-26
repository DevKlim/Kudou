#pragma once

#include "editor/plugins/abstract_polygon_2d_editor.h"
#include "scene/2d/physics/collision_polygon_2d.h"

class CollisionPolygon2DEditor : public AbstractPolygon2DEditor {
	GDCLASS(CollisionPolygon2DEditor, AbstractPolygon2DEditor);

	CollisionPolygon2D *node = nullptr;

protected:
	virtual Node2D *_get_node() const override;
	virtual void _set_node(Node *p_polygon) override;

public:
	CollisionPolygon2DEditor();
};

class CollisionPolygon2DEditorPlugin : public AbstractPolygon2DEditorPlugin {
	GDCLASS(CollisionPolygon2DEditorPlugin, AbstractPolygon2DEditorPlugin);

public:
	CollisionPolygon2DEditorPlugin();
};

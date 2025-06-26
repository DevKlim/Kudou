#include "collision_polygon_2d_editor_plugin.h"

Node2D *CollisionPolygon2DEditor::_get_node() const {
	return node;
}

void CollisionPolygon2DEditor::_set_node(Node *p_polygon) {
	node = Object::cast_to<CollisionPolygon2D>(p_polygon);
}

CollisionPolygon2DEditor::CollisionPolygon2DEditor() {
	set_edit_origin_and_center(true);
}

CollisionPolygon2DEditorPlugin::CollisionPolygon2DEditorPlugin() :
		AbstractPolygon2DEditorPlugin(memnew(CollisionPolygon2DEditor), "CollisionPolygon2D") {
}

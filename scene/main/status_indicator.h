#pragma once

#include "scene/main/node.h"
#include "servers/display_server.h"

class StatusIndicator : public Node {
	GDCLASS(StatusIndicator, Node);

	Ref<Texture2D> icon;
	String tooltip;
	bool visible = true;
	DisplayServer::IndicatorID iid = DisplayServer::INVALID_INDICATOR_ID;
	NodePath menu;

protected:
	void _notification(int p_what);
	static void _bind_methods();

	void _callback(MouseButton p_index, const Point2i &p_pos);

public:
	void set_icon(const Ref<Texture2D> &p_icon);
	Ref<Texture2D> get_icon() const;

	void set_tooltip(const String &p_tooltip);
	String get_tooltip() const;

	void set_menu(const NodePath &p_menu);
	NodePath get_menu() const;

	void set_visible(bool p_visible);
	bool is_visible() const;

	Rect2 get_rect() const;
};

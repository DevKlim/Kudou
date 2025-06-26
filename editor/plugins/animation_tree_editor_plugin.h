#pragma once

#include "editor/plugins/editor_plugin.h"
#include "scene/animation/animation_tree.h"
#include "scene/gui/graph_edit.h"

class Button;
class EditorFileDialog;
class ScrollContainer;

class AnimationTreeNodeEditorPlugin : public VBoxContainer {
	GDCLASS(AnimationTreeNodeEditorPlugin, VBoxContainer);

public:
	virtual bool can_edit(const Ref<AnimationNode> &p_node) = 0;
	virtual void edit(const Ref<AnimationNode> &p_node) = 0;
};

class AnimationTreeEditor : public VBoxContainer {
	GDCLASS(AnimationTreeEditor, VBoxContainer);

	ScrollContainer *path_edit = nullptr;
	HBoxContainer *path_hb = nullptr;

	AnimationTree *tree = nullptr;
	MarginContainer *editor_base = nullptr;

	Vector<String> button_path;
	Vector<String> edited_path;
	Vector<AnimationTreeNodeEditorPlugin *> editors;

	void _update_path();
	void _clear_editors();
	ObjectID current_root;

	void _path_button_pressed(int p_path);
	void _animation_list_changed();

	static Vector<String> get_animation_list();

protected:
	void _notification(int p_what);
	void _node_removed(Node *p_node);

	static AnimationTreeEditor *singleton;

public:
	AnimationTree *get_animation_tree() { return tree; }
	void add_plugin(AnimationTreeNodeEditorPlugin *p_editor);
	void remove_plugin(AnimationTreeNodeEditorPlugin *p_editor);

	String get_base_path();

	bool can_edit(const Ref<AnimationNode> &p_node) const;

	void edit_path(const Vector<String> &p_path);
	Vector<String> get_edited_path() const;

	void enter_editor(const String &p_path = "");
	static AnimationTreeEditor *get_singleton() { return singleton; }
	void edit(AnimationTree *p_tree);
	AnimationTreeEditor();
};

class AnimationTreeEditorPlugin : public EditorPlugin {
	GDCLASS(AnimationTreeEditorPlugin, EditorPlugin);

	AnimationTreeEditor *anim_tree_editor = nullptr;
	Button *button = nullptr;

public:
	virtual String get_plugin_name() const override { return "AnimationTree"; }
	bool has_main_screen() const override { return false; }
	virtual void edit(Object *p_object) override;
	virtual bool handles(Object *p_object) const override;
	virtual void make_visible(bool p_visible) override;

	AnimationTreeEditorPlugin();
};

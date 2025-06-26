#pragma once

#include "editor/plugins/editor_plugin.h"
#include "scene/resources/3d/mesh_library.h"

class EditorFileDialog;
class ConfirmationDialog;
class MenuButton;
class MeshInstance3D;

class MeshLibraryEditor : public Control {
	GDCLASS(MeshLibraryEditor, Control);

	Ref<MeshLibrary> mesh_library;

	MenuButton *menu = nullptr;
	ConfirmationDialog *cd_remove = nullptr;
	ConfirmationDialog *cd_update = nullptr;
	EditorFileDialog *file = nullptr;
	bool apply_xforms = false;
	int to_erase = 0;

	enum {
		MENU_OPTION_ADD_ITEM,
		MENU_OPTION_REMOVE_ITEM,
		MENU_OPTION_UPDATE_FROM_SCENE,
		MENU_OPTION_IMPORT_FROM_SCENE,
		MENU_OPTION_IMPORT_FROM_SCENE_APPLY_XFORMS
	};

	int option = 0;
	void _import_scene_cbk(const String &p_str);
	void _menu_cbk(int p_option);
	void _menu_remove_confirm();
	void _menu_update_confirm(bool p_apply_xforms);

	static void _import_scene(Node *p_scene, Ref<MeshLibrary> p_library, bool p_merge, bool p_apply_xforms);
	static void _import_scene_parse_node(Ref<MeshLibrary> p_library, HashMap<int, MeshInstance3D *> &p_mesh_instances, Node *p_node, bool p_merge, bool p_apply_xforms);

public:
	MenuButton *get_menu_button() const { return menu; }

	void edit(const Ref<MeshLibrary> &p_mesh_library);
	static Error update_library_file(Node *p_base_scene, Ref<MeshLibrary> ml, bool p_merge = true, bool p_apply_xforms = false);

	MeshLibraryEditor();
};

class MeshLibraryEditorPlugin : public EditorPlugin {
	GDCLASS(MeshLibraryEditorPlugin, EditorPlugin);

	MeshLibraryEditor *mesh_library_editor = nullptr;

public:
	virtual String get_plugin_name() const override { return "MeshLibrary"; }
	bool has_main_screen() const override { return false; }
	virtual void edit(Object *p_node) override;
	virtual bool handles(Object *p_node) const override;
	virtual void make_visible(bool p_visible) override;

	MeshLibraryEditorPlugin();
};

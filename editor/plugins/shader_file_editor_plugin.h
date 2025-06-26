#pragma once

#include "editor/plugins/editor_plugin.h"
#include "scene/gui/box_container.h"
#include "scene/gui/panel_container.h"
#include "scene/gui/rich_text_label.h"
#include "servers/rendering/rendering_device_binds.h"

class ItemList;

class ShaderFileEditor : public PanelContainer {
	GDCLASS(ShaderFileEditor, PanelContainer);

	Ref<RDShaderFile> shader_file;

	HBoxContainer *stage_hb = nullptr;
	ItemList *versions = nullptr;
	Button *stages[RD::SHADER_STAGE_MAX];
	RichTextLabel *error_text = nullptr;

	void _update_version(const StringName &p_version_txt, const RenderingDevice::ShaderStage p_stage);
	void _version_selected(int p_stage);
	void _editor_settings_changed();

	void _update_options();
	void _shader_changed();

protected:
	void _notification(int p_what);

public:
	static ShaderFileEditor *singleton;
	void edit(const Ref<RDShaderFile> &p_shader);

	ShaderFileEditor();
};

class ShaderFileEditorPlugin : public EditorPlugin {
	GDCLASS(ShaderFileEditorPlugin, EditorPlugin);

	ShaderFileEditor *shader_editor = nullptr;
	Button *button = nullptr;

public:
	virtual String get_plugin_name() const override { return "ShaderFile"; }
	bool has_main_screen() const override { return false; }
	virtual void edit(Object *p_object) override;
	virtual bool handles(Object *p_object) const override;
	virtual void make_visible(bool p_visible) override;

	ShaderFileEditor *get_shader_editor() const { return shader_editor; }

	ShaderFileEditorPlugin();
};

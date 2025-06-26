#pragma once

#include "scene/gui/control.h"
#include "scene/resources/shader.h"

class ShaderEditor : public Control {
	GDCLASS(ShaderEditor, Control);

public:
	virtual void edit_shader(const Ref<Shader> &p_shader) = 0;
	virtual void edit_shader_include(const Ref<ShaderInclude> &p_shader_inc) {}

	virtual void apply_shaders() = 0;
	virtual bool is_unsaved() const = 0;
	virtual void save_external_data(const String &p_str = "") = 0;
	virtual void validate_script() = 0;
	virtual Control *get_top_bar() = 0;
};

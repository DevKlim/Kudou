#pragma once

#include "scene/gui/dialogs.h"
#include "scene/gui/tab_container.h"
#include "scene/resources/syntax_highlighter.h"

class EditorNativeShaderSourceVisualizer : public AcceptDialog {
	GDCLASS(EditorNativeShaderSourceVisualizer, AcceptDialog)
	TabContainer *versions = nullptr;
	Ref<CodeHighlighter> syntax_highlighter;

	void _load_theme_settings();
	void _inspect_shader(RID p_shader);

protected:
	static void _bind_methods();

public:
	EditorNativeShaderSourceVisualizer();
};

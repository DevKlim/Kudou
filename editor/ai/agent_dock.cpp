#include "agent_dock.h"

#include "editor/editor_interface.h"
#include "editor/editor_node.h"
#include "editor/editor_string_names.h"
#include "scene/gui/box_container.h"
#include "scene/gui/button.h"
#include "scene/gui/line_edit.h"
#include "scene/gui/rich_text_label.h"

EditorPluginAgentDock::EditorPluginAgentDock(EditorNode *p_node) {
	// Initialization is done in _notification(NOTIFICATION_ENTER_TREE).
}

EditorPluginAgentDock::~EditorPluginAgentDock() {
}

void EditorPluginAgentDock::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_ENTER_TREE: {
			// Dock panel
			agent_dock = memnew(VBoxContainer);
			agent_dock->set_name(SNAME("KudouAgent"));

			chat_history = memnew(RichTextLabel);
			chat_history->set_v_size_flags(Control::SIZE_EXPAND_FILL);
			agent_dock->add_child(chat_history);

			HBoxContainer *input_container = memnew(HBoxContainer);
			agent_dock->add_child(input_container);

			user_input = memnew(LineEdit);
			user_input->set_h_size_flags(Control::SIZE_EXPAND_FILL);
			user_input->set_placeholder(TTR("Ask Kudou..."));
			input_container->add_child(user_input);

			send_button = memnew(Button);
			send_button->set_text(TTR("Send"));
			input_container->add_child(send_button);

			add_control_to_bottom_panel(agent_dock, TTR("Kudou Agent"));

		} break;

		case NOTIFICATION_EXIT_TREE: {
			if (agent_dock) {
				remove_control_from_bottom_panel(agent_dock);
				// The control is freed by remove_control_from_bottom_panel.
				agent_dock = nullptr;
			}
		} break;
	}
}

const Ref<Texture2D> EditorPluginAgentDock::get_plugin_icon() const {
	// For now, use a placeholder icon. A custom "Kudou" icon will be added later.
	return EditorNode::get_singleton()->get_editor_theme()->get_icon(SNAME("Comment"), EditorStringName(EditorIcons));
}
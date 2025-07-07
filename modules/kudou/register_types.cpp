/* register_types.cpp */

#include "register_types.h"

#include "core/object/class_db.h"

#ifdef TOOLS_ENABLED
#include "editor/editor_node.h"
#include "editor/plugins/editor_plugin.h"
#include "modules/kudou/editor/kudou_agent_plugin.h"

#endif

#include "modules/kudou/editor/kudou_chat_controller.h"

#ifdef TOOLS_ENABLED
static void _editor_init_callback() {
	EditorNode::add_init_callback([]() {
		EditorNode::get_singleton()->add_editor_plugin(memnew(KudouAgentPlugin));
	});
}
#endif

void initialize_kudou_module(ModuleInitializationLevel p_level) {
	if (p_level == MODULE_INITIALIZATION_LEVEL_SCENE) {
		ClassDB::register_class<KudouChatController>();
	}

#ifdef TOOLS_ENABLED
	if (p_level == MODULE_INITIALIZATION_LEVEL_EDITOR) {
		ClassDB::register_class<KudouAgentPlugin>();
		
		_editor_init_callback();
	}
#endif
}

void uninitialize_kudou_module(ModuleInitializationLevel p_level) {
	// No-op for now.
}
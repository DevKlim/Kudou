/* register_types.cpp */

#include "register_types.h"

#include "core/object/class_db.h"
#include "editor/editor_node.h"

#include "editor/kudou_browser_plugin.h"
#include "editor/kudou_agent_plugin.h"
#include "gdbrowser.hpp"
#include "gdcef.hpp"

static KudouAgentPlugin *kudou_agent_plugin = nullptr;
static KudouBrowserPlugin *kudou_browser_plugin = nullptr;

// This callback is deferred until the editor is fully initialized.
static void editor_init_callback() {
	// The Kudou Agent dock
	kudou_agent_plugin = memnew(KudouAgentPlugin);
	EditorNode::add_editor_plugin(kudou_agent_plugin);

	// The main "Browser" screen
	kudou_browser_plugin = memnew(KudouBrowserPlugin);
	EditorNode::add_editor_plugin(kudou_browser_plugin);
}

void initialize_kudou_module(ModuleInitializationLevel p_level) {
	if (p_level == MODULE_INITIALIZATION_LEVEL_SCENE) {
		// Register runtime-visible nodes here
		ClassDB::register_class<GDCef>();
		ClassDB::register_class<GDBrowserView>();
	}

	if (p_level == MODULE_INITIALIZATION_LEVEL_EDITOR) {
		// Register editor-only plugins and functionality here
		ClassDB::register_class<KudouAgentPlugin>();
		ClassDB::register_class<KudouBrowserPlugin>();
		EditorNode::add_init_callback(&editor_init_callback);
	}
}

void uninitialize_kudou_module(ModuleInitializationLevel p_level) {
	// Cleanup if needed, but EditorNode handles plugin deletion.
}
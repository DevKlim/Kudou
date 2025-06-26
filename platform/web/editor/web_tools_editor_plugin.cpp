#include "web_tools_editor_plugin.h"

#include "core/config/engine.h"
#include "core/config/project_settings.h"
#include "core/io/dir_access.h"
#include "core/io/file_access.h"
#include "core/os/time.h"
#include "editor/editor_node.h"
#include "editor/export/project_zip_packer.h"

#include <emscripten/emscripten.h>

// Web functions defined in library_godot_editor_tools.js
extern "C" {
extern void godot_js_os_download_buffer(const uint8_t *p_buf, int p_buf_size, const char *p_name, const char *p_mime);
}

static void _web_editor_init_callback() {
	EditorNode::get_singleton()->add_editor_plugin(memnew(WebToolsEditorPlugin));
}

void WebToolsEditorPlugin::initialize() {
	EditorNode::add_init_callback(_web_editor_init_callback);
}

WebToolsEditorPlugin::WebToolsEditorPlugin() {
	add_tool_menu_item("Download Project Source", callable_mp(this, &WebToolsEditorPlugin::_download_zip));
}

void WebToolsEditorPlugin::_download_zip() {
	if (!Engine::get_singleton() || !Engine::get_singleton()->is_editor_hint()) {
		ERR_PRINT("Downloading the project as a ZIP archive is only available in Editor mode.");
		return;
	}
	const String output_name = ProjectZIPPacker::get_project_zip_safe_name();
	const String output_path = String("/tmp").path_join(output_name);
	ProjectZIPPacker::pack_project_zip(output_path);

	{
		Ref<FileAccess> f = FileAccess::open(output_path, FileAccess::READ);
		ERR_FAIL_COND_MSG(f.is_null(), "Unable to create ZIP file.");
		Vector<uint8_t> buf;
		buf.resize(f->get_length());
		f->get_buffer(buf.ptrw(), buf.size());
		godot_js_os_download_buffer(buf.ptr(), buf.size(), output_name.utf8().get_data(), "application/zip");
	}

	// Remove the temporary file since it was sent to the user's native filesystem as a download.
	DirAccess::remove_file_or_error(output_path);
}

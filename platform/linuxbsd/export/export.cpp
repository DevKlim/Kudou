#include "export.h"

#include "export_plugin.h"

#include "editor/export/editor_export.h"

void register_linuxbsd_exporter_types() {
	GDREGISTER_VIRTUAL_CLASS(EditorExportPlatformLinuxBSD);
}

void register_linuxbsd_exporter() {
	Ref<EditorExportPlatformLinuxBSD> platform;
	platform.instantiate();
	platform->set_name("Linux");
	platform->set_os_name("Linux");
	platform->set_chmod_flags(0755);

	EditorExport::get_singleton()->add_export_platform(platform);
}

#include "export.h"

#include "export_plugin.h"

#include "editor/export/editor_export.h"

void register_visionos_exporter_types() {
	GDREGISTER_VIRTUAL_CLASS(EditorExportPlatformVisionOS);
}

void register_visionos_exporter() {
	Ref<EditorExportPlatformVisionOS> platform;
	platform.instantiate();

	EditorExport::get_singleton()->add_export_platform(platform);
}

#include "export_plugin.h"

#include "logo_svg.gen.h"
#include "run_icon_svg.gen.h"

EditorExportPlatformVisionOS::EditorExportPlatformVisionOS() :
		EditorExportPlatformAppleEmbedded(_visionos_logo_svg, _visionos_run_icon_svg) {
}

EditorExportPlatformVisionOS::~EditorExportPlatformVisionOS() {
}

void EditorExportPlatformVisionOS::get_export_options(List<ExportOption> *r_options) const {
	EditorExportPlatformAppleEmbedded::get_export_options(r_options);

	r_options->push_back(ExportOption(PropertyInfo(Variant::STRING, "application/min_visionos_version"), get_minimum_deployment_target()));
}

Vector<EditorExportPlatformAppleEmbedded::IconInfo> EditorExportPlatformVisionOS::get_icon_infos() const {
	return Vector<EditorExportPlatformAppleEmbedded::IconInfo>();
}

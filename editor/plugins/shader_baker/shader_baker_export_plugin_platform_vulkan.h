#pragma once

#include "editor/plugins/shader_baker_export_plugin.h"

class ShaderBakerExportPluginPlatformVulkan : public ShaderBakerExportPluginPlatform {
	GDCLASS(ShaderBakerExportPluginPlatformVulkan, ShaderBakerExportPluginPlatform);

public:
	virtual RenderingShaderContainerFormat *create_shader_container_format(const Ref<EditorExportPlatform> &p_platform) override;
	virtual bool matches_driver(const String &p_driver) override;
};

#pragma once

#include "editor/plugins/shader_baker_export_plugin.h"

class ShaderBakerExportPluginPlatformD3D12 : public ShaderBakerExportPluginPlatform {
	GDCLASS(ShaderBakerExportPluginPlatformD3D12, ShaderBakerExportPluginPlatform);

private:
	void *lib_d3d12 = nullptr;

public:
	virtual RenderingShaderContainerFormat *create_shader_container_format(const Ref<EditorExportPlatform> &p_platform) override;
	virtual bool matches_driver(const String &p_driver) override;
	virtual ~ShaderBakerExportPluginPlatformD3D12() override;
};

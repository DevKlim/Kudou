#pragma once

#include "servers/rendering/rendering_shader_container.h"

#define RENDERING_SHADER_CONTAINER_VULKAN_COMPRESSION 1
#define RENDERING_SHADER_CONTAINER_VULKAN_SMOLV 1

class RenderingShaderContainerVulkan : public RenderingShaderContainer {
	GDSOFTCLASS(RenderingShaderContainerVulkan, RenderingShaderContainer);

public:
	static const uint32_t FORMAT_VERSION;

protected:
	virtual uint32_t _format() const override;
	virtual uint32_t _format_version() const override;
	virtual bool _set_code_from_spirv(const Vector<RenderingDeviceCommons::ShaderStageSPIRVData> &p_spirv) override;
};

class RenderingShaderContainerFormatVulkan : public RenderingShaderContainerFormat {
public:
	virtual Ref<RenderingShaderContainer> create_container() const override;
	virtual ShaderLanguageVersion get_shader_language_version() const override;
	virtual ShaderSpirvVersion get_shader_spirv_version() const override;
	RenderingShaderContainerFormatVulkan();
	virtual ~RenderingShaderContainerFormatVulkan();
};

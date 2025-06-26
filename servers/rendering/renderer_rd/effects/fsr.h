#pragma once

#include "spatial_upscaler.h"

#include "../storage_rd/render_scene_buffers_rd.h"
#include "servers/rendering/renderer_rd/shaders/effects/fsr_upscale.glsl.gen.h"

namespace RendererRD {

class FSR : public SpatialUpscaler {
public:
	FSR();
	~FSR();

	virtual const Span<char> get_label() const final { return "FSR 1.0 Upscale"; }
	virtual void ensure_context(Ref<RenderSceneBuffersRD> p_render_buffers) final {}
	virtual void process(Ref<RenderSceneBuffersRD> p_render_buffers, RID p_source_rd_texture, RID p_destination_texture) final;

private:
	enum FSRShaderVariant {
		FSR_SHADER_VARIANT_NORMAL,
		FSR_SHADER_VARIANT_FALLBACK,
	};

	enum FSRUpscalePass {
		FSR_UPSCALE_PASS_EASU = 0,
		FSR_UPSCALE_PASS_RCAS = 1
	};

	struct FSRUpscalePushConstant {
		float resolution_width;
		float resolution_height;
		float upscaled_width;
		float upscaled_height;
		float sharpness;
		int pass;
		int _unused0, _unused1;
	};

	FsrUpscaleShaderRD fsr_shader;
	RID shader_version;
	RID pipeline;
};

} // namespace RendererRD

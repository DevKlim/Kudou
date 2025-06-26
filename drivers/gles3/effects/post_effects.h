#pragma once

#ifdef GLES3_ENABLED

#include "drivers/gles3/shaders/effects/post.glsl.gen.h"
#include "glow.h"

namespace GLES3 {

class PostEffects {
private:
	struct Post {
		PostShaderGLES3 shader;
		RID shader_version;
	} post;

	static PostEffects *singleton;

	// Use for full-screen effects. Slightly more efficient than screen_quad as this eliminates pixel overdraw along the diagonal.
	GLuint screen_triangle = 0;
	GLuint screen_triangle_array = 0;

	void _draw_screen_triangle();

public:
	static PostEffects *get_singleton();

	PostEffects();
	~PostEffects();

	void post_copy(GLuint p_dest_framebuffer, Size2i p_dest_size, GLuint p_source_color, Size2i p_source_size, float p_luminance_multiplier, const Glow::GLOWLEVEL *p_glow_buffers, float p_glow_intensity, uint32_t p_view = 0, bool p_use_multiview = false, uint64_t p_spec_constants = 0);
};

} //namespace GLES3

#endif // GLES3_ENABLED

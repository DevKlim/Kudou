#pragma once

#include "../gltf_defines.h"

#include "core/io/resource.h"

class GLTFTexture : public Resource {
	GDCLASS(GLTFTexture, Resource);

private:
	GLTFImageIndex src_image = -1;
	GLTFTextureSamplerIndex sampler = -1;

protected:
	static void _bind_methods();

public:
	GLTFImageIndex get_src_image() const;
	void set_src_image(GLTFImageIndex val);
	GLTFTextureSamplerIndex get_sampler() const;
	void set_sampler(GLTFTextureSamplerIndex val);
};

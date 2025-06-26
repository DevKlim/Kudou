#pragma once

// Note, these classes are part of the Rendering Device based renderer.
// They are included here to ensure the correct order of registration
// is performed.
// Once the renderer has been moved into a module, these classes should
// be moved as well.

#include "scene/resources/texture.h"

class Texture2DRD : public Texture2D {
	GDCLASS(Texture2DRD, Texture2D)

	mutable RID texture_rid;
	RID texture_rd_rid;
	Size2i size;

protected:
	static void _bind_methods();

public:
	virtual int get_width() const override;
	virtual int get_height() const override;
	virtual RID get_rid() const override;
	virtual bool has_alpha() const override;

	virtual Ref<Image> get_image() const override;

	void set_texture_rd_rid(RID p_texture_rd_rid);
	RID get_texture_rd_rid() const;

	// Internal function that should only be called from the rendering thread.
	void _set_texture_rd_rid(RID p_texture_rd_rid);

	Texture2DRD();
	~Texture2DRD();
};

class TextureLayeredRD : public TextureLayered {
	GDCLASS(TextureLayeredRD, TextureLayered)

	LayeredType layer_type;

	mutable RID texture_rid;
	RID texture_rd_rid;

	Image::Format image_format;
	Size2i size;
	uint32_t layers = 0;
	uint32_t mipmaps = 0;

protected:
	static void _bind_methods();

public:
	virtual Image::Format get_format() const override;
	virtual LayeredType get_layered_type() const override;
	virtual int get_width() const override;
	virtual int get_height() const override;
	virtual int get_layers() const override;
	virtual bool has_mipmaps() const override;
	virtual RID get_rid() const override;

	virtual Ref<Image> get_layer_data(int p_layer) const override;

	void set_texture_rd_rid(RID p_texture_rd_rid);
	RID get_texture_rd_rid() const;

	// Internal function that should only be called from the rendering thread.
	void _set_texture_rd_rid(RID p_texture_rd_rid);

	TextureLayeredRD(LayeredType p_layer_type);
	~TextureLayeredRD();
};

class Texture2DArrayRD : public TextureLayeredRD {
	GDCLASS(Texture2DArrayRD, TextureLayeredRD)

public:
	Texture2DArrayRD() :
			TextureLayeredRD(LAYERED_TYPE_2D_ARRAY) {}
};

class TextureCubemapRD : public TextureLayeredRD {
	GDCLASS(TextureCubemapRD, TextureLayeredRD)

public:
	TextureCubemapRD() :
			TextureLayeredRD(LAYERED_TYPE_CUBEMAP) {}
};

class TextureCubemapArrayRD : public TextureLayeredRD {
	GDCLASS(TextureCubemapArrayRD, TextureLayeredRD)

public:
	TextureCubemapArrayRD() :
			TextureLayeredRD(LAYERED_TYPE_CUBEMAP_ARRAY) {}
};

class Texture3DRD : public Texture3D {
	GDCLASS(Texture3DRD, Texture3D)

	mutable RID texture_rid;
	RID texture_rd_rid;

	Image::Format image_format;
	Vector3i size;
	uint32_t mipmaps = 0;

protected:
	static void _bind_methods();

public:
	virtual Image::Format get_format() const override;
	virtual int get_width() const override;
	virtual int get_height() const override;
	virtual int get_depth() const override;
	virtual bool has_mipmaps() const override;
	virtual RID get_rid() const override;

	void set_texture_rd_rid(RID p_texture_rd_rid);
	RID get_texture_rd_rid() const;

	// Internal function that should only be called from the rendering thread.
	void _set_texture_rd_rid(RID p_texture_rd_rid);

	Texture3DRD();
	~Texture3DRD();
};

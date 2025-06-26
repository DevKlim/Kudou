#pragma once

#include "core/io/image.h"

enum BasisDecompressFormat {
	BASIS_DECOMPRESS_RG,
	BASIS_DECOMPRESS_RGB,
	BASIS_DECOMPRESS_RGBA,
	BASIS_DECOMPRESS_RG_AS_RA,
	BASIS_DECOMPRESS_R,
	BASIS_DECOMPRESS_HDR_RGB,
	BASIS_DECOMPRESS_MAX
};
constexpr uint32_t BASIS_DECOMPRESS_FLAG_KTX2 = 1 << 31;

void basis_universal_init();

#ifdef TOOLS_ENABLED
struct BasisRGBAF {
	uint32_t r;
	uint32_t g;
	uint32_t b;
	uint32_t a;
};

Vector<uint8_t> basis_universal_packer(const Ref<Image> &p_image, Image::UsedChannels p_channels, const Image::BasisUniversalPackerParams &p_basisu_params);
#endif

Ref<Image> basis_universal_unpacker_ptr(const uint8_t *p_data, int p_size);
Ref<Image> basis_universal_unpacker(const Vector<uint8_t> &p_buffer);

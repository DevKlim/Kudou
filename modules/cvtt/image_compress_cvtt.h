#pragma once

#include "core/io/image.h"

void image_compress_cvtt(Image *p_image, Image::UsedChannels p_channels);
void image_decompress_cvtt(Image *p_image);

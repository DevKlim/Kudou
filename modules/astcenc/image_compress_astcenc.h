#pragma once

#include "core/io/image.h"

#ifdef TOOLS_ENABLED
void _compress_astc(Image *r_img, Image::ASTCFormat p_format);
#endif

void _decompress_astc(Image *r_img);

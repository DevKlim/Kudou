#ifndef _TVG_COMPRESSOR_H_
#define _TVG_COMPRESSOR_H_

#include <cstdint>

namespace tvg
{
    uint8_t* lzwEncode(const uint8_t* uncompressed, uint32_t uncompressedSizeBytes, uint32_t* compressedSizeBytes, uint32_t* compressedSizeBits);
    uint8_t* lzwDecode(const uint8_t* compressed, uint32_t compressedSizeBytes, uint32_t compressedSizeBits, uint32_t uncompressedSizeBytes);
    size_t b64Decode(const char* encoded, const size_t len, char** decoded);
    unsigned long djb2Encode(const char* str);
}

#endif  //_TVG_COMPRESSOR_H_

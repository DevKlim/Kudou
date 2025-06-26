#ifndef GODOT_CORE_MBEDTLS_CONFIG_H
#define GODOT_CORE_MBEDTLS_CONFIG_H

#include <limits.h>

// For AES
#define MBEDTLS_CIPHER_MODE_CBC
#define MBEDTLS_CIPHER_MODE_CFB
#define MBEDTLS_CIPHER_MODE_CTR
#define MBEDTLS_CIPHER_MODE_OFB
#define MBEDTLS_CIPHER_MODE_XTS

#define MBEDTLS_AES_C
#define MBEDTLS_BASE64_C
#define MBEDTLS_CTR_DRBG_C
#define MBEDTLS_ENTROPY_C
#define MBEDTLS_MD5_C
#define MBEDTLS_SHA1_C
#define MBEDTLS_SHA256_C
#define MBEDTLS_PLATFORM_ZEROIZE_ALT
#define MBEDTLS_NO_DEFAULT_ENTROPY_SOURCES

// This is only to pass a check in the mbedtls check_config.h header, none of
// the files we include as part of the core build uses it anyway, we already
// define MBEDTLS_PLATFORM_ZEROIZE_ALT which is the only relevant function.
#if defined(__MINGW32__)
#define MBEDTLS_PLATFORM_C
#endif

#endif // GODOT_CORE_MBEDTLS_CONFIG_H

#ifndef GODOT_MODULE_MBEDTLS_CONFIG_H
#define GODOT_MODULE_MBEDTLS_CONFIG_H

#include "platform_config.h"

#ifdef GODOT_MBEDTLS_INCLUDE_H

// Allow platforms to customize the mbedTLS configuration.
#include GODOT_MBEDTLS_INCLUDE_H

#else

// Include default mbedTLS config.
#include <mbedtls/mbedtls_config.h>

// Disable weak cryptography.
#undef MBEDTLS_KEY_EXCHANGE_DHE_PSK_ENABLED
#undef MBEDTLS_KEY_EXCHANGE_DHE_RSA_ENABLED
#undef MBEDTLS_DES_C
#undef MBEDTLS_DHM_C

#ifdef THREADS_ENABLED
// In mbedTLS 3, the PSA subsystem has an implicit shared context, MBEDTLS_THREADING_C is required to make it thread safe.
#define MBEDTLS_THREADING_C
#define MBEDTLS_THREADING_ALT
#define GODOT_MBEDTLS_THREADING_ALT
#endif

#if !(defined(__linux__) && defined(__aarch64__))
// ARMv8 hardware AES operations. Detection only possible on linux.
// May technically be supported on some ARM32 arches but doesn't seem
// to be in our current Linux SDK's neon-fp-armv8.
#undef MBEDTLS_AESCE_C
#endif

#if defined(__has_feature)
#if __has_feature(memory_sanitizer)
// MemorySanitizer is incompatible with ASM.
#undef MBEDTLS_HAVE_ASM
#undef MBEDTLS_AESNI_C
#endif
#endif

// Disable deprecated
#define MBEDTLS_DEPRECATED_REMOVED

#endif // GODOT_MBEDTLS_INCLUDE_H

#endif // GODOT_MODULE_MBEDTLS_CONFIG_H

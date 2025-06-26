#include "register_types.h"

#include "crypto_mbedtls.h"
#include "dtls_server_mbedtls.h"
#include "packet_peer_mbed_dtls.h"
#include "stream_peer_mbedtls.h"

#include "core/config/project_settings.h"

#if MBEDTLS_VERSION_MAJOR >= 3
#include <psa/crypto.h>
#endif

#ifdef TESTS_ENABLED
#include "tests/test_crypto_mbedtls.h"
#endif

#ifdef GODOT_MBEDTLS_THREADING_ALT
extern "C" {
void godot_mbedtls_mutex_init(mbedtls_threading_mutex_t *p_mutex) {
	ERR_FAIL_NULL(p_mutex);
	p_mutex->mutex = memnew(Mutex);
}

void godot_mbedtls_mutex_free(mbedtls_threading_mutex_t *p_mutex) {
	ERR_FAIL_NULL(p_mutex);
	ERR_FAIL_NULL(p_mutex->mutex);
	memdelete((Mutex *)p_mutex->mutex);
}

int godot_mbedtls_mutex_lock(mbedtls_threading_mutex_t *p_mutex) {
	ERR_FAIL_NULL_V(p_mutex, MBEDTLS_ERR_THREADING_BAD_INPUT_DATA);
	ERR_FAIL_NULL_V(p_mutex->mutex, MBEDTLS_ERR_THREADING_BAD_INPUT_DATA);
	((Mutex *)p_mutex->mutex)->lock();
	return 0;
}

int godot_mbedtls_mutex_unlock(mbedtls_threading_mutex_t *p_mutex) {
	ERR_FAIL_NULL_V(p_mutex, MBEDTLS_ERR_THREADING_BAD_INPUT_DATA);
	ERR_FAIL_NULL_V(p_mutex->mutex, MBEDTLS_ERR_THREADING_BAD_INPUT_DATA);
	((Mutex *)p_mutex->mutex)->unlock();
	return 0;
}
};
#endif

static bool godot_mbedtls_initialized = false;

void initialize_mbedtls_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_CORE) {
		return;
	}

	GLOBAL_DEF("network/tls/enable_tls_v1.3", true);

#ifdef GODOT_MBEDTLS_THREADING_ALT
	mbedtls_threading_set_alt(
			godot_mbedtls_mutex_init,
			godot_mbedtls_mutex_free,
			godot_mbedtls_mutex_lock,
			godot_mbedtls_mutex_unlock);
#endif

#if MBEDTLS_VERSION_MAJOR >= 3
	int status = psa_crypto_init();
	ERR_FAIL_COND_MSG(status != PSA_SUCCESS, "Failed to initialize psa crypto. The mbedTLS modules will not work.");
#endif

#ifdef DEBUG_ENABLED
	if (OS::get_singleton()->is_stdout_verbose()) {
		mbedtls_debug_set_threshold(1);
	}
#endif

	godot_mbedtls_initialized = true;

	CryptoMbedTLS::initialize_crypto();
	StreamPeerMbedTLS::initialize_tls();
	PacketPeerMbedDTLS::initialize_dtls();
	DTLSServerMbedTLS::initialize();
}

void uninitialize_mbedtls_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_CORE) {
		return;
	}

	if (!godot_mbedtls_initialized) {
		return;
	}

#if MBEDTLS_VERSION_MAJOR >= 3
	mbedtls_psa_crypto_free();
#endif

	DTLSServerMbedTLS::finalize();
	PacketPeerMbedDTLS::finalize_dtls();
	StreamPeerMbedTLS::finalize_tls();
	CryptoMbedTLS::finalize_crypto();

#ifdef GODOT_MBEDTLS_THREADING_ALT
	mbedtls_threading_free_alt();
#endif
}

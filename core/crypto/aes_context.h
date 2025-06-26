#pragma once

#include "core/crypto/crypto_core.h"
#include "core/object/ref_counted.h"

class AESContext : public RefCounted {
	GDCLASS(AESContext, RefCounted);

public:
	enum Mode : int32_t {
		MODE_ECB_ENCRYPT,
		MODE_ECB_DECRYPT,
		MODE_CBC_ENCRYPT,
		MODE_CBC_DECRYPT,
		MODE_MAX
	};

private:
	Mode mode = MODE_MAX;
	CryptoCore::AESContext ctx;
	PackedByteArray iv;

protected:
	static void _bind_methods();

public:
	Error start(Mode p_mode, const PackedByteArray &p_key, const PackedByteArray &p_iv = PackedByteArray());
	PackedByteArray update(const PackedByteArray &p_src);
	PackedByteArray get_iv_state();
	void finish();

	AESContext();
};

VARIANT_ENUM_CAST(AESContext::Mode);

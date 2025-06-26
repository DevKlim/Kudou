#pragma once

#include "core/object/ref_counted.h"

class HashingContext : public RefCounted {
	GDCLASS(HashingContext, RefCounted);

public:
	enum HashType : int32_t {
		HASH_MD5,
		HASH_SHA1,
		HASH_SHA256
	};

private:
	void *ctx = nullptr;
	HashType type = HASH_MD5;

protected:
	static void _bind_methods();
	void _create_ctx(HashType p_type);
	void _delete_ctx();

public:
	Error start(HashType p_type);
	Error update(const PackedByteArray &p_chunk);
	PackedByteArray finish();

	HashingContext() {}
	~HashingContext();
};

VARIANT_ENUM_CAST(HashingContext::HashType);

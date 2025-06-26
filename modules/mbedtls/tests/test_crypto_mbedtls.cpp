#include "test_crypto_mbedtls.h"

#include "../crypto_mbedtls.h"

#include "tests/test_macros.h"
#include "tests/test_utils.h"

namespace TestCryptoMbedTLS {

void hmac_digest_test(HashingContext::HashType ht, String expected_hex) {
	CryptoMbedTLS crypto;
	PackedByteArray key = String("supersecretkey").to_utf8_buffer();
	PackedByteArray msg = String("Return of the MAC!").to_utf8_buffer();
	PackedByteArray digest = crypto.hmac_digest(ht, key, msg);
	String hex = String::hex_encode_buffer(digest.ptr(), digest.size());
	CHECK(hex == expected_hex);
}

void hmac_context_digest_test(HashingContext::HashType ht, String expected_hex) {
	HMACContextMbedTLS ctx;
	PackedByteArray key = String("supersecretkey").to_utf8_buffer();
	PackedByteArray msg1 = String("Return of ").to_utf8_buffer();
	PackedByteArray msg2 = String("the MAC!").to_utf8_buffer();
	Error err = ctx.start(ht, key);
	CHECK(err == OK);
	err = ctx.update(msg1);
	CHECK(err == OK);
	err = ctx.update(msg2);
	CHECK(err == OK);
	PackedByteArray digest = ctx.finish();
	String hex = String::hex_encode_buffer(digest.ptr(), digest.size());
	CHECK(hex == expected_hex);
}

Ref<CryptoKey> create_crypto_key(const String &p_key_path, bool p_public_only) {
	Ref<CryptoKey> crypto_key = Ref<CryptoKey>(CryptoKey::create());
	crypto_key->load(p_key_path, p_public_only);
	return crypto_key;
}

String read_file_s(const String &p_file_path) {
	Ref<FileAccess> file_access = FileAccess::open(p_file_path, FileAccess::READ);
	REQUIRE(file_access.is_valid());
	return file_access->get_as_utf8_string();
}

bool files_equal(const String &p_in_path, const String &p_out_path) {
	const String s_in = read_file_s(p_in_path);
	const String s_out = read_file_s(p_out_path);
	return s_in == s_out;
}

void crypto_key_public_only_test(const String &p_key_path, bool p_public_only) {
	Ref<CryptoKey> crypto_key = create_crypto_key(p_key_path, p_public_only);
	bool is_equal = crypto_key->is_public_only() == p_public_only;
	CHECK(is_equal);
}

void crypto_key_save_test(const String &p_in_path, const String &p_out_path, bool p_public_only) {
	Ref<CryptoKey> crypto_key = create_crypto_key(p_in_path, p_public_only);
	crypto_key->save(p_out_path, p_public_only);
	bool is_equal = files_equal(p_in_path, p_out_path);
	CHECK(is_equal);
}

void crypto_key_save_public_only_test(const String &p_in_priv_path, const String &p_in_pub_path, const String &p_out_path) {
	Ref<CryptoKey> crypto_key = create_crypto_key(p_in_priv_path, false);
	crypto_key->save(p_out_path, true);
	bool is_equal = files_equal(p_in_pub_path, p_out_path);
	CHECK(is_equal);
}
} // namespace TestCryptoMbedTLS

#pragma once

#include "core/object/ref_counted.h"
#include "core/string/string_name.h"
#include "core/templates/hash_map.h"

class FileAccess;

typedef void (*ResourceUIDScanForUIDOnStartup)();

class ResourceUID : public Object {
	GDCLASS(ResourceUID, Object)
public:
	typedef int64_t ID;
	constexpr const static ID INVALID_ID = -1;

	static String get_cache_file();

private:
	void *crypto = nullptr; // CryptoCore::RandomGenerator (avoid including crypto_core.h)
	Mutex mutex;
	struct Cache {
		CharString cs;
		bool saved_to_cache = false;
	};

	HashMap<ID, Cache> unique_ids; //unique IDs and utf8 paths (less memory used)
	static ResourceUID *singleton;

	uint32_t cache_entries = 0;
	bool changed = false;

protected:
	static void _bind_methods();

public:
	inline static ResourceUIDScanForUIDOnStartup scan_for_uid_on_startup = nullptr;

	String id_to_text(ID p_id) const;
	ID text_to_id(const String &p_text) const;

	ID create_id();
	ID create_id_for_path(const String &p_path);
	bool has_id(ID p_id) const;
	void add_id(ID p_id, const String &p_path);
	void set_id(ID p_id, const String &p_path);
	String get_id_path(ID p_id) const;
	void remove_id(ID p_id);

	static String uid_to_path(const String &p_uid);
	static String path_to_uid(const String &p_path);
	static String ensure_path(const String &p_uid_or_path);

	Error load_from_cache(bool p_reset);
	Error save_to_cache();
	Error update_cache();
	static String get_path_from_cache(Ref<FileAccess> &p_cache_file, const String &p_uid_string);

	void clear();

	static ResourceUID *get_singleton() { return singleton; }

	ResourceUID();
	~ResourceUID();
};

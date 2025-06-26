#pragma once

#ifndef DISABLE_DEPRECATED

#include "core/string/string_name.h"
#include "core/templates/hash_map.h"
#include "core/templates/local_vector.h"

// Note: In most situations, compatibility methods should be registered via ClassDB::bind_compatibility_method().
//       This class is only meant to be used in exceptional circumstances, for example, when Godot's hashing
//       algorithm changes and registering compatibility methods for all affect methods would be onerous.

class GDExtensionSpecialCompatHashes {
	struct Mapping {
		StringName method;
		uint32_t legacy_hash;
		uint32_t current_hash;
	};

	static inline HashMap<StringName, LocalVector<Mapping>> mappings;

public:
	static void initialize();
	static void finalize();
	static bool lookup_current_hash(const StringName &p_class, const StringName &p_method, uint32_t p_legacy_hash, uint32_t *r_current_hash);
	static bool get_legacy_hashes(const StringName &p_class, const StringName &p_method, Array &r_hashes, bool p_check_valid = true);
};

#endif // DISABLE_DEPRECATED

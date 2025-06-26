#pragma once

#include "core/string/ustring.h"
#include "core/templates/hash_map.h"
#include "core/templates/local_vector.h"
#include "core/templates/pair.h"
#include "core/variant/array.h"
#include "core/variant/variant_deep_duplicate.h"

class Variant;

struct ContainerType;
struct DictionaryPrivate;
struct StringLikeVariantComparator;
struct VariantHasher;

class Dictionary {
	mutable DictionaryPrivate *_p;

	void _ref(const Dictionary &p_from) const;
	void _unref() const;

public:
	using ConstIterator = HashMap<Variant, Variant, VariantHasher, StringLikeVariantComparator>::ConstIterator;

	ConstIterator begin() const;
	ConstIterator end() const;

	LocalVector<Variant> get_key_list() const;
	Variant get_key_at_index(int p_index) const;
	Variant get_value_at_index(int p_index) const;

	Variant &operator[](const Variant &p_key);
	const Variant &operator[](const Variant &p_key) const;

	const Variant *getptr(const Variant &p_key) const;
	Variant *getptr(const Variant &p_key);

	Variant get_valid(const Variant &p_key) const;
	Variant get(const Variant &p_key, const Variant &p_default) const;
	Variant get_or_add(const Variant &p_key, const Variant &p_default);
	bool set(const Variant &p_key, const Variant &p_value);

	int size() const;
	bool is_empty() const;
	void clear();
	void sort();
	void merge(const Dictionary &p_dictionary, bool p_overwrite = false);
	Dictionary merged(const Dictionary &p_dictionary, bool p_overwrite = false) const;

	bool has(const Variant &p_key) const;
	bool has_all(const Array &p_keys) const;
	Variant find_key(const Variant &p_value) const;

	bool erase(const Variant &p_key);

	bool operator==(const Dictionary &p_dictionary) const;
	bool operator!=(const Dictionary &p_dictionary) const;
	bool recursive_equal(const Dictionary &p_dictionary, int recursion_count) const;

	uint32_t hash() const;
	uint32_t recursive_hash(int recursion_count) const;
	void operator=(const Dictionary &p_dictionary);

	void assign(const Dictionary &p_dictionary);
	const Variant *next(const Variant *p_key = nullptr) const;

	Array keys() const;
	Array values() const;

	Dictionary duplicate(bool p_deep = false) const;
	Dictionary duplicate_deep(ResourceDeepDuplicateMode p_deep_subresources_mode = RESOURCE_DEEP_DUPLICATE_INTERNAL) const;
	Dictionary recursive_duplicate(bool p_deep, ResourceDeepDuplicateMode p_deep_subresources_mode, int recursion_count) const;

	void set_typed(const ContainerType &p_key_type, const ContainerType &p_value_type);
	void set_typed(uint32_t p_key_type, const StringName &p_key_class_name, const Variant &p_key_script, uint32_t p_value_type, const StringName &p_value_class_name, const Variant &p_value_script);

	bool is_typed() const;
	bool is_typed_key() const;
	bool is_typed_value() const;
	bool is_same_instance(const Dictionary &p_other) const;
	bool is_same_typed(const Dictionary &p_other) const;
	bool is_same_typed_key(const Dictionary &p_other) const;
	bool is_same_typed_value(const Dictionary &p_other) const;

	ContainerType get_key_type() const;
	ContainerType get_value_type() const;
	uint32_t get_typed_key_builtin() const;
	uint32_t get_typed_value_builtin() const;
	StringName get_typed_key_class_name() const;
	StringName get_typed_value_class_name() const;
	Variant get_typed_key_script() const;
	Variant get_typed_value_script() const;

	void make_read_only();
	bool is_read_only() const;

	const void *id() const;

	Dictionary(const Dictionary &p_base, uint32_t p_key_type, const StringName &p_key_class_name, const Variant &p_key_script, uint32_t p_value_type, const StringName &p_value_class_name, const Variant &p_value_script);
	Dictionary(const Dictionary &p_from);
	Dictionary(std::initializer_list<KeyValue<Variant, Variant>> p_init);
	Dictionary();
	~Dictionary();
};

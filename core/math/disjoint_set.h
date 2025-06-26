#pragma once

#include "core/templates/hash_map.h"
#include "core/templates/vector.h"

/* This DisjointSet class uses Find with path compression and Union by rank */
template <typename T, typename H = HashMapHasherDefault, typename C = HashMapComparatorDefault<T>, typename AL = DefaultAllocator>
class DisjointSet {
	struct Element {
		T object;
		Element *parent = nullptr;
		int rank = 0;
	};

	typedef HashMap<T, Element *, H, C> MapT;

	MapT elements;

	Element *get_parent(Element *element);

	_FORCE_INLINE_ Element *insert_or_get(T object);

public:
	~DisjointSet();

	_FORCE_INLINE_ void insert(T object) { (void)insert_or_get(object); }

	void create_union(T a, T b);

	void get_representatives(Vector<T> &out_roots);

	void get_members(Vector<T> &out_members, T representative);
};

/* FUNCTIONS */

template <typename T, typename H, typename C, typename AL>
DisjointSet<T, H, C, AL>::~DisjointSet() {
	for (KeyValue<T, Element *> &E : elements) {
		memdelete_allocator<Element, AL>(E.value);
	}
}

template <typename T, typename H, typename C, typename AL>
typename DisjointSet<T, H, C, AL>::Element *DisjointSet<T, H, C, AL>::get_parent(Element *element) {
	if (element->parent != element) {
		element->parent = get_parent(element->parent);
	}

	return element->parent;
}

template <typename T, typename H, typename C, typename AL>
typename DisjointSet<T, H, C, AL>::Element *DisjointSet<T, H, C, AL>::insert_or_get(T object) {
	typename MapT::Iterator itr = elements.find(object);
	if (itr != nullptr) {
		return itr->value;
	}

	Element *new_element = memnew_allocator(Element, AL);
	new_element->object = object;
	new_element->parent = new_element;
	elements.insert(object, new_element);

	return new_element;
}

template <typename T, typename H, typename C, typename AL>
void DisjointSet<T, H, C, AL>::create_union(T a, T b) {
	Element *x = insert_or_get(a);
	Element *y = insert_or_get(b);

	Element *x_root = get_parent(x);
	Element *y_root = get_parent(y);

	// Already in the same set
	if (x_root == y_root) {
		return;
	}

	// Not in the same set, merge
	if (x_root->rank < y_root->rank) {
		SWAP(x_root, y_root);
	}

	// Merge y_root into x_root
	y_root->parent = x_root;
	if (x_root->rank == y_root->rank) {
		++x_root->rank;
	}
}

template <typename T, typename H, typename C, typename AL>
void DisjointSet<T, H, C, AL>::get_representatives(Vector<T> &out_representatives) {
	for (KeyValue<T, Element *> &E : elements) {
		Element *element = E.value;
		if (element->parent == element) {
			out_representatives.push_back(element->object);
		}
	}
}

template <typename T, typename H, typename C, typename AL>
void DisjointSet<T, H, C, AL>::get_members(Vector<T> &out_members, T representative) {
	typename MapT::Iterator rep_itr = elements.find(representative);
	ERR_FAIL_NULL(rep_itr);

	Element *rep_element = rep_itr->value;
	ERR_FAIL_COND(rep_element->parent != rep_element);

	for (KeyValue<T, Element *> &E : elements) {
		Element *parent = get_parent(E.value);
		if (parent == rep_element) {
			out_members.push_back(E.key);
		}
	}
}

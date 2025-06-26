#pragma once

#include "core/object/gdvirtual.gen.inc"
#include "core/object/ref_counted.h"

class EditorInterface;
class EditorNode;
class Node;

class EditorScript : public RefCounted {
	GDCLASS(EditorScript, RefCounted);

protected:
	static void _bind_methods();

	GDVIRTUAL0_REQUIRED(_run)

public:
	void add_root_node(Node *p_node);
	Node *get_scene() const;
	EditorInterface *get_editor_interface() const;

	virtual void run();
};

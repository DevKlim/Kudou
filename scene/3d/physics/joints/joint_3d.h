#pragma once

#include "scene/3d/node_3d.h"
#include "scene/3d/physics/physics_body_3d.h"

class Joint3D : public Node3D {
	GDCLASS(Joint3D, Node3D);

	RID ba, bb;

	RID joint;

	NodePath a;
	NodePath b;

	int solver_priority = 1;
	bool exclude_from_collision = true;
	String warning;
	bool configured = false;

protected:
	void _disconnect_signals();
	void _body_exit_tree();
	void _update_joint(bool p_only_free = false);

	void _notification(int p_what);

	virtual void _configure_joint(RID p_joint, PhysicsBody3D *body_a, PhysicsBody3D *body_b) = 0;

	static void _bind_methods();

	_FORCE_INLINE_ bool is_configured() const { return configured; }

public:
	virtual PackedStringArray get_configuration_warnings() const override;

	void set_node_a(const NodePath &p_node_a);
	NodePath get_node_a() const;

	void set_node_b(const NodePath &p_node_b);
	NodePath get_node_b() const;

	void set_solver_priority(int p_priority);
	int get_solver_priority() const;

	void set_exclude_nodes_from_collision(bool p_enable);
	bool get_exclude_nodes_from_collision() const;

	RID get_rid() const { return joint; }
	Joint3D();
	~Joint3D();
};

#include "godot_pin_joint_3d.h"

bool GodotPinJoint3D::setup(real_t p_step) {
	dynamic_A = (A->get_mode() > PhysicsServer3D::BODY_MODE_KINEMATIC);
	dynamic_B = (B->get_mode() > PhysicsServer3D::BODY_MODE_KINEMATIC);

	if (!dynamic_A && !dynamic_B) {
		return false;
	}

	m_appliedImpulse = real_t(0.);

	Vector3 normal(0, 0, 0);

	for (int i = 0; i < 3; i++) {
		normal[i] = 1;
		memnew_placement(
				&m_jac[i],
				GodotJacobianEntry3D(
						A->get_principal_inertia_axes().transposed(),
						B->get_principal_inertia_axes().transposed(),
						A->get_transform().xform(m_pivotInA) - A->get_transform().origin - A->get_center_of_mass(),
						B->get_transform().xform(m_pivotInB) - B->get_transform().origin - B->get_center_of_mass(),
						normal,
						A->get_inv_inertia(),
						A->get_inv_mass(),
						B->get_inv_inertia(),
						B->get_inv_mass()));
		normal[i] = 0;
	}

	return true;
}

void GodotPinJoint3D::solve(real_t p_step) {
	Vector3 pivotAInW = A->get_transform().xform(m_pivotInA);
	Vector3 pivotBInW = B->get_transform().xform(m_pivotInB);

	Vector3 normal(0, 0, 0);

	//Vector3 angvelA = A->get_transform().origin.getBasis().transpose() * A->getAngularVelocity();
	//Vector3 angvelB = B->get_transform().origin.getBasis().transpose() * B->getAngularVelocity();

	for (int i = 0; i < 3; i++) {
		normal[i] = 1;
		real_t jacDiagABInv = real_t(1.) / m_jac[i].getDiagonal();

		Vector3 rel_pos1 = pivotAInW - A->get_transform().origin;
		Vector3 rel_pos2 = pivotBInW - B->get_transform().origin;
		//this jacobian entry could be reused for all iterations

		Vector3 vel1 = A->get_velocity_in_local_point(rel_pos1);
		Vector3 vel2 = B->get_velocity_in_local_point(rel_pos2);
		Vector3 vel = vel1 - vel2;

		real_t rel_vel;
		rel_vel = normal.dot(vel);

		/*
		//velocity error (first order error)
		real_t rel_vel = m_jac[i].getRelativeVelocity(A->getLinearVelocity(),angvelA,
														B->getLinearVelocity(),angvelB);
	*/

		//positional error (zeroth order error)
		real_t depth = -(pivotAInW - pivotBInW).dot(normal); //this is the error projected on the normal

		real_t impulse = depth * m_tau / p_step * jacDiagABInv - m_damping * rel_vel * jacDiagABInv;

		real_t impulseClamp = m_impulseClamp;
		if (impulseClamp > 0) {
			if (impulse < -impulseClamp) {
				impulse = -impulseClamp;
			}
			if (impulse > impulseClamp) {
				impulse = impulseClamp;
			}
		}

		m_appliedImpulse += impulse;
		Vector3 impulse_vector = normal * impulse;
		if (dynamic_A) {
			A->apply_impulse(impulse_vector, pivotAInW - A->get_transform().origin);
		}
		if (dynamic_B) {
			B->apply_impulse(-impulse_vector, pivotBInW - B->get_transform().origin);
		}

		normal[i] = 0;
	}
}

void GodotPinJoint3D::set_param(PhysicsServer3D::PinJointParam p_param, real_t p_value) {
	switch (p_param) {
		case PhysicsServer3D::PIN_JOINT_BIAS:
			m_tau = p_value;
			break;
		case PhysicsServer3D::PIN_JOINT_DAMPING:
			m_damping = p_value;
			break;
		case PhysicsServer3D::PIN_JOINT_IMPULSE_CLAMP:
			m_impulseClamp = p_value;
			break;
	}
}

real_t GodotPinJoint3D::get_param(PhysicsServer3D::PinJointParam p_param) const {
	switch (p_param) {
		case PhysicsServer3D::PIN_JOINT_BIAS:
			return m_tau;
		case PhysicsServer3D::PIN_JOINT_DAMPING:
			return m_damping;
		case PhysicsServer3D::PIN_JOINT_IMPULSE_CLAMP:
			return m_impulseClamp;
	}

	return 0;
}

GodotPinJoint3D::GodotPinJoint3D(GodotBody3D *p_body_a, const Vector3 &p_pos_a, GodotBody3D *p_body_b, const Vector3 &p_pos_b) :
		GodotJoint3D(_arr, 2) {
	A = p_body_a;
	B = p_body_b;
	m_pivotInA = p_pos_a;
	m_pivotInB = p_pos_b;

	A->add_constraint(this, 0);
	B->add_constraint(this, 1);
}

GodotPinJoint3D::~GodotPinJoint3D() {
}

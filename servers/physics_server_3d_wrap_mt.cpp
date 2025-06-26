#include "physics_server_3d_wrap_mt.h"

void PhysicsServer3DWrapMT::_assign_mt_ids(WorkerThreadPool::TaskID p_pump_task_id) {
	server_thread = Thread::get_caller_id();
	server_task_id = p_pump_task_id;
}

void PhysicsServer3DWrapMT::_thread_exit() {
	exit = true;
}

void PhysicsServer3DWrapMT::_thread_loop() {
	while (!exit) {
		WorkerThreadPool::get_singleton()->yield();
		command_queue.flush_all();
	}
}

/* EVENT QUEUING */

void PhysicsServer3DWrapMT::step(real_t p_step) {
	if (create_thread) {
		command_queue.push(physics_server_3d, &PhysicsServer3D::step, p_step);
	} else {
		physics_server_3d->step(p_step);
	}
}

void PhysicsServer3DWrapMT::sync() {
	if (create_thread) {
		command_queue.sync();
	} else {
		command_queue.flush_all(); // Flush all pending from other threads.
	}
	physics_server_3d->sync();
}

void PhysicsServer3DWrapMT::flush_queries() {
	physics_server_3d->flush_queries();
}

void PhysicsServer3DWrapMT::end_sync() {
	physics_server_3d->end_sync();
}

void PhysicsServer3DWrapMT::init() {
	if (create_thread) {
		WorkerThreadPool::TaskID tid = WorkerThreadPool::get_singleton()->add_task(callable_mp(this, &PhysicsServer3DWrapMT::_thread_loop), true);
		command_queue.set_pump_task_id(tid);
		command_queue.push(this, &PhysicsServer3DWrapMT::_assign_mt_ids, tid);
		command_queue.push_and_sync(physics_server_3d, &PhysicsServer3D::init);
		DEV_ASSERT(server_task_id == tid);
	} else {
		server_thread = Thread::MAIN_ID;
		physics_server_3d->init();
	}
}

void PhysicsServer3DWrapMT::finish() {
	if (create_thread) {
		command_queue.push(physics_server_3d, &PhysicsServer3D::finish);
		command_queue.push(this, &PhysicsServer3DWrapMT::_thread_exit);
		if (server_task_id != WorkerThreadPool::INVALID_TASK_ID) {
			WorkerThreadPool::get_singleton()->wait_for_task_completion(server_task_id);
			server_task_id = WorkerThreadPool::INVALID_TASK_ID;
		}
		server_thread = Thread::MAIN_ID;
	} else {
		physics_server_3d->finish();
	}
}

PhysicsServer3DWrapMT::PhysicsServer3DWrapMT(PhysicsServer3D *p_contained, bool p_create_thread) {
	physics_server_3d = p_contained;
	create_thread = p_create_thread;
}

PhysicsServer3DWrapMT::~PhysicsServer3DWrapMT() {
	memdelete(physics_server_3d);
}

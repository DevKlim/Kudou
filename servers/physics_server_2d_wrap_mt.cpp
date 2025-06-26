#include "physics_server_2d_wrap_mt.h"

void PhysicsServer2DWrapMT::_assign_mt_ids(WorkerThreadPool::TaskID p_pump_task_id) {
	server_thread = Thread::get_caller_id();
	server_task_id = p_pump_task_id;
}

void PhysicsServer2DWrapMT::_thread_exit() {
	exit = true;
}

void PhysicsServer2DWrapMT::_thread_loop() {
	while (!exit) {
		WorkerThreadPool::get_singleton()->yield();
		command_queue.flush_all();
	}
}

/* EVENT QUEUING */

void PhysicsServer2DWrapMT::step(real_t p_step) {
	if (create_thread) {
		command_queue.push(physics_server_2d, &PhysicsServer2D::step, p_step);
	} else {
		physics_server_2d->step(p_step);
	}
}

void PhysicsServer2DWrapMT::sync() {
	if (create_thread) {
		command_queue.sync();
	} else {
		command_queue.flush_all(); // Flush all pending from other threads.
	}
	physics_server_2d->sync();
}

void PhysicsServer2DWrapMT::flush_queries() {
	physics_server_2d->flush_queries();
}

void PhysicsServer2DWrapMT::end_sync() {
	physics_server_2d->end_sync();
}

void PhysicsServer2DWrapMT::init() {
	if (create_thread) {
		WorkerThreadPool::TaskID tid = WorkerThreadPool::get_singleton()->add_task(callable_mp(this, &PhysicsServer2DWrapMT::_thread_loop), true);
		command_queue.set_pump_task_id(tid);
		command_queue.push(this, &PhysicsServer2DWrapMT::_assign_mt_ids, tid);
		command_queue.push_and_sync(physics_server_2d, &PhysicsServer2D::init);
		DEV_ASSERT(server_task_id == tid);
	} else {
		server_thread = Thread::MAIN_ID;
		physics_server_2d->init();
	}
}

void PhysicsServer2DWrapMT::finish() {
	if (create_thread) {
		command_queue.push(physics_server_2d, &PhysicsServer2D::finish);
		command_queue.push(this, &PhysicsServer2DWrapMT::_thread_exit);
		if (server_task_id != WorkerThreadPool::INVALID_TASK_ID) {
			WorkerThreadPool::get_singleton()->wait_for_task_completion(server_task_id);
			server_task_id = WorkerThreadPool::INVALID_TASK_ID;
		}
		server_thread = Thread::MAIN_ID;
	} else {
		physics_server_2d->finish();
	}
}

PhysicsServer2DWrapMT::PhysicsServer2DWrapMT(PhysicsServer2D *p_contained, bool p_create_thread) {
	physics_server_2d = p_contained;
	create_thread = p_create_thread;
}

PhysicsServer2DWrapMT::~PhysicsServer2DWrapMT() {
	memdelete(physics_server_2d);
}

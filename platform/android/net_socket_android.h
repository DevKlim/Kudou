#pragma once

#include "drivers/unix/net_socket_unix.h"

#include <jni.h>

/**
 * Specialized NetSocket implementation for Android.
 *
 * Some devices requires Android-specific code to acquire a MulticastLock
 * before sockets are allowed to receive broadcast and multicast packets.
 * This implementation calls into Java code and automatically acquire/release
 * the lock when broadcasting is enabled/disabled on a socket, or that socket
 * joins/leaves a multicast group.
 */
class NetSocketAndroid : public NetSocketUnix {
private:
	static jobject net_utils;
	static jclass cls;
	static jmethodID _multicast_lock_acquire;
	static jmethodID _multicast_lock_release;

	bool wants_broadcast = false;
	int multicast_groups = 0;

	static void multicast_lock_acquire();
	static void multicast_lock_release();

protected:
	static NetSocket *_create_func();

public:
	static void make_default();
	static void setup(jobject p_net_utils);
	static void terminate();

	virtual void close();

	virtual Error set_broadcasting_enabled(bool p_enabled);
	virtual Error join_multicast_group(const IPAddress &p_multi_address, const String &p_if_name);
	virtual Error leave_multicast_group(const IPAddress &p_multi_address, const String &p_if_name);

	NetSocketAndroid() {}
	~NetSocketAndroid();
};

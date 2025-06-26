#pragma once

#ifdef DBUS_ENABLED

#include "core/os/thread.h"
#include "core/os/thread_safe.h"

class FreeDesktopAtSPIMonitor {
private:
	Thread thread;

	SafeFlag exit_thread;
	SafeFlag sr_enabled;
	SafeFlag supported;

	static void monitor_thread_func(void *p_userdata);

public:
	FreeDesktopAtSPIMonitor();
	~FreeDesktopAtSPIMonitor();

	bool is_supported() { return supported.is_set(); }
	bool is_active() { return sr_enabled.is_set(); }
};

#endif // DBUS_ENABLED

#pragma once

#include "core/os/thread_safe.h"
#include "servers/xr/xr_positional_tracker.h"

/**
	The controller tracker object as an object that represents the position and orientation of a controller.
*/

class XRControllerTracker : public XRPositionalTracker {
	GDCLASS(XRControllerTracker, XRPositionalTracker);
	_THREAD_SAFE_CLASS_

protected:
	static void _bind_methods();

public:
	XRControllerTracker();
};

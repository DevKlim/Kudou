#include "xr_controller_tracker.h"

#include "core/input/input.h"

void XRControllerTracker::_bind_methods() {}

XRControllerTracker::XRControllerTracker() {
	type = XRServer::TRACKER_CONTROLLER;
}

#pragma once

#include "core/os/thread_safe.h"
#include "servers/xr_server.h"

/**
	The XR tracker object is a common base for all different types of XR trackers.
*/

class XRTracker : public RefCounted {
	GDCLASS(XRTracker, RefCounted);
	_THREAD_SAFE_CLASS_

protected:
	XRServer::TrackerType type = XRServer::TRACKER_UNKNOWN; // type of tracker
	StringName name = "Unknown"; // (unique) name of the tracker
	String description; // description of the tracker

	static void _bind_methods();

public:
	virtual void set_tracker_type(XRServer::TrackerType p_type);
	XRServer::TrackerType get_tracker_type() const;
	void set_tracker_name(const StringName &p_name);
	StringName get_tracker_name() const;
	void set_tracker_desc(const String &p_desc);
	String get_tracker_desc() const;
};

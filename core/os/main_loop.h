#pragma once

#include "core/input/input_event.h"
#include "core/object/gdvirtual.gen.inc"
#include "core/object/ref_counted.h"

class MainLoop : public Object {
	GDCLASS(MainLoop, Object);

protected:
	static void _bind_methods();

	GDVIRTUAL0(_initialize)
	GDVIRTUAL1R(bool, _physics_process, double)
	GDVIRTUAL1R(bool, _process, double)
	GDVIRTUAL0(_finalize)

public:
	enum {
		//make sure these are replicated in Node
		NOTIFICATION_OS_MEMORY_WARNING = 2009,
		NOTIFICATION_TRANSLATION_CHANGED = 2010,
		NOTIFICATION_WM_ABOUT = 2011,
		NOTIFICATION_CRASH = 2012,
		NOTIFICATION_OS_IME_UPDATE = 2013,
		NOTIFICATION_APPLICATION_RESUMED = 2014,
		NOTIFICATION_APPLICATION_PAUSED = 2015,
		NOTIFICATION_APPLICATION_FOCUS_IN = 2016,
		NOTIFICATION_APPLICATION_FOCUS_OUT = 2017,
		NOTIFICATION_TEXT_SERVER_CHANGED = 2018,
	};

	virtual void initialize();
	virtual void iteration_prepare() {}
	virtual bool physics_process(double p_time);
	virtual void iteration_end() {}
	virtual bool process(double p_time);
	virtual void finalize();

	MainLoop() {}
	virtual ~MainLoop() {}
};

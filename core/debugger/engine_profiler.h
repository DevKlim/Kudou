#pragma once

#include "core/object/gdvirtual.gen.inc"
#include "core/object/ref_counted.h"

class EngineProfiler : public RefCounted {
	GDCLASS(EngineProfiler, RefCounted);

private:
	String registration;

protected:
	static void _bind_methods();

public:
	virtual void toggle(bool p_enable, const Array &p_opts);
	virtual void add(const Array &p_data);
	virtual void tick(double p_frame_time, double p_process_time, double p_physics_time, double p_physics_frame_time);

	Error bind(const String &p_name);
	Error unbind();
	bool is_bound() const { return registration.length() > 0; }

	GDVIRTUAL2(_toggle, bool, Array);
	GDVIRTUAL1(_add_frame, Array);
	GDVIRTUAL4(_tick, double, double, double, double);

	EngineProfiler() {}
	virtual ~EngineProfiler();
};

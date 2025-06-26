#pragma once

#include "core/object/ref_counted.h"

class GDExtension;

class GDExtensionLoader : public RefCounted {
	GDSOFTCLASS(GDExtensionLoader, RefCounted);

public:
	virtual Error open_library(const String &p_path) = 0;
	virtual Error initialize(GDExtensionInterfaceGetProcAddress p_get_proc_address, const Ref<GDExtension> &p_extension, GDExtensionInitialization *r_initialization) = 0;
	virtual void close_library() = 0;
	virtual bool is_library_open() const = 0;
	virtual bool has_library_changed() const = 0;
	virtual bool library_exists() const = 0;
};

#pragma once

#include "core/io/resource.h"
#include "core/object/gdvirtual.gen.inc"

class EditorResourceConversionPlugin : public RefCounted {
	GDCLASS(EditorResourceConversionPlugin, RefCounted);

protected:
	static void _bind_methods();

	GDVIRTUAL0RC(String, _converts_to)
	GDVIRTUAL1RC(bool, _handles, Ref<Resource>)
	GDVIRTUAL1RC(Ref<Resource>, _convert, Ref<Resource>)

public:
	virtual String converts_to() const;
	virtual bool handles(const Ref<Resource> &p_resource) const;
	virtual Ref<Resource> convert(const Ref<Resource> &p_resource) const;
};

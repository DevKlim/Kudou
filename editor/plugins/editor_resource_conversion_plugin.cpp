#include "editor_resource_conversion_plugin.h"

void EditorResourceConversionPlugin::_bind_methods() {
	GDVIRTUAL_BIND(_converts_to);
	GDVIRTUAL_BIND(_handles, "resource");
	GDVIRTUAL_BIND(_convert, "resource");
}

String EditorResourceConversionPlugin::converts_to() const {
	String ret;
	GDVIRTUAL_CALL(_converts_to, ret);
	return ret;
}

bool EditorResourceConversionPlugin::handles(const Ref<Resource> &p_resource) const {
	bool ret = false;
	GDVIRTUAL_CALL(_handles, p_resource, ret);
	return ret;
}

Ref<Resource> EditorResourceConversionPlugin::convert(const Ref<Resource> &p_resource) const {
	Ref<Resource> ret;
	GDVIRTUAL_CALL(_convert, p_resource, ret);
	return ret;
}

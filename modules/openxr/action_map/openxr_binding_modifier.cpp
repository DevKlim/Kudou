#include "openxr_binding_modifier.h"

void OpenXRBindingModifier::_bind_methods() {
	GDVIRTUAL_BIND(_get_description);
	GDVIRTUAL_BIND(_get_ip_modification);
}

String OpenXRBindingModifier::get_description() const {
	String desc;
	if (GDVIRTUAL_CALL(_get_description, desc)) {
		return desc;
	}
	return "";
}

PackedByteArray OpenXRBindingModifier::get_ip_modification() {
	PackedByteArray data;
	if (GDVIRTUAL_CALL(_get_ip_modification, data)) {
		return data;
	}
	return PackedByteArray();
}

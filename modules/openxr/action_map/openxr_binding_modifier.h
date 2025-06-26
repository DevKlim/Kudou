#pragma once

#include "../action_map/openxr_action.h"
#include "core/io/resource.h"

// Part of implementation for:
// https://registry.khronos.org/OpenXR/specs/1.1/html/xrspec.html#XR_KHR_binding_modification

class OpenXRInteractionProfile;
class OpenXRIPBinding;

class OpenXRBindingModifier : public Resource {
	GDCLASS(OpenXRBindingModifier, Resource);

protected:
	static void _bind_methods();

	GDVIRTUAL0RC_REQUIRED(String, _get_description)
	GDVIRTUAL0R_REQUIRED(PackedByteArray, _get_ip_modification)

public:
	virtual String get_description() const; // Returns the description shown in the editor
	virtual PackedByteArray get_ip_modification(); // Return the XrBindingModificationsKHR binding modifier struct data used when calling xrSuggestInteractionProfileBindings
};

class OpenXRIPBindingModifier : public OpenXRBindingModifier {
	GDCLASS(OpenXRIPBindingModifier, OpenXRBindingModifier);

protected:
	friend class OpenXRInteractionProfile;

	OpenXRInteractionProfile *interaction_profile = nullptr; // action belongs to this interaction profile

public:
	OpenXRInteractionProfile *get_interaction_profile() const { return interaction_profile; }
};

class OpenXRActionBindingModifier : public OpenXRBindingModifier {
	GDCLASS(OpenXRActionBindingModifier, OpenXRBindingModifier);

protected:
	friend class OpenXRIPBinding;

	OpenXRIPBinding *ip_binding = nullptr; // action belongs to this binding

public:
	OpenXRIPBinding *get_ip_binding() const { return ip_binding; }
};

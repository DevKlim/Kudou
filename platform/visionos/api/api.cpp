#include "api.h"

#if defined(VISIONOS_ENABLED)

void register_visionos_api() {
	godot_apple_embedded_plugins_initialize();
}

void unregister_visionos_api() {
	godot_apple_embedded_plugins_deinitialize();
}

#else

void register_visionos_api() {}
void unregister_visionos_api() {}

#endif // VISIONOS_ENABLED

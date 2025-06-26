#pragma once

#if defined(VISIONOS_ENABLED)
extern void godot_apple_embedded_plugins_initialize();
extern void godot_apple_embedded_plugins_deinitialize();
#endif

void register_visionos_api();
void unregister_visionos_api();

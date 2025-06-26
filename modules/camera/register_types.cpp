#include "register_types.h"

#if defined(LINUXBSD_ENABLED)
#include "camera_linux.h"
#endif
#if defined(WINDOWS_ENABLED)
#include "camera_win.h"
#endif
#if defined(MACOS_ENABLED)
#include "camera_macos.h"
#endif
#if defined(ANDROID_ENABLED)
#include "camera_android.h"
#endif

void initialize_camera_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

#if defined(LINUXBSD_ENABLED)
	CameraServer::make_default<CameraLinux>();
#endif
#if defined(WINDOWS_ENABLED)
	CameraServer::make_default<CameraWindows>();
#endif
#if defined(MACOS_ENABLED)
	CameraServer::make_default<CameraMacOS>();
#endif
#if defined(ANDROID_ENABLED)
	CameraServer::make_default<CameraAndroid>();
#endif
}

void uninitialize_camera_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
}

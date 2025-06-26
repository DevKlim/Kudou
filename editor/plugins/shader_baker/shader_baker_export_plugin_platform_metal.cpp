#include "shader_baker_export_plugin_platform_metal.h"

#include "drivers/metal/rendering_shader_container_metal.h"

RenderingShaderContainerFormat *ShaderBakerExportPluginPlatformMetal::create_shader_container_format(const Ref<EditorExportPlatform> &p_platform) {
	const String &os_name = p_platform->get_os_name();
	const MetalDeviceProfile *profile;

	if (os_name == U"macOS") {
		profile = MetalDeviceProfile::get_profile(MetalDeviceProfile::Platform::macOS, MetalDeviceProfile::GPU::Apple7);
	} else if (os_name == U"iOS") {
		profile = MetalDeviceProfile::get_profile(MetalDeviceProfile::Platform::iOS, MetalDeviceProfile::GPU::Apple7);
	} else {
		ERR_FAIL_V_MSG(nullptr, vformat("Unsupported platform: %s", os_name));
	}
	return memnew(RenderingShaderContainerFormatMetal(profile, true));
}

bool ShaderBakerExportPluginPlatformMetal::matches_driver(const String &p_driver) {
	return p_driver == "metal";
}

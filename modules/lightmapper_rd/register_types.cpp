#include "register_types.h"

#include "lightmapper_rd.h"

#include "core/config/project_settings.h"
#include "scene/3d/lightmapper.h"

#ifndef _3D_DISABLED
static Lightmapper *create_lightmapper_rd() {
	return memnew(LightmapperRD);
}
#endif

void initialize_lightmapper_rd_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	GLOBAL_DEF(PropertyInfo(Variant::INT, "rendering/lightmapping/bake_quality/low_quality_ray_count", PROPERTY_HINT_RANGE, "1,4096,1,or_greater"), 32);
	GLOBAL_DEF(PropertyInfo(Variant::INT, "rendering/lightmapping/bake_quality/medium_quality_ray_count", PROPERTY_HINT_RANGE, "1,4096,1,or_greater"), 128);
	GLOBAL_DEF(PropertyInfo(Variant::INT, "rendering/lightmapping/bake_quality/high_quality_ray_count", PROPERTY_HINT_RANGE, "1,4096,1,or_greater"), 512);
	GLOBAL_DEF(PropertyInfo(Variant::INT, "rendering/lightmapping/bake_quality/ultra_quality_ray_count", PROPERTY_HINT_RANGE, "1,4096,1,or_greater"), 2048);
	GLOBAL_DEF(PropertyInfo(Variant::INT, "rendering/lightmapping/bake_performance/max_rays_per_pass", PROPERTY_HINT_RANGE, "1,256,1,or_greater"), 4);
	GLOBAL_DEF(PropertyInfo(Variant::INT, "rendering/lightmapping/bake_performance/region_size", PROPERTY_HINT_RANGE, "1,4096,1,or_greater"), 512);
	GLOBAL_DEF(PropertyInfo(Variant::INT, "rendering/lightmapping/bake_performance/max_transparency_rays", PROPERTY_HINT_RANGE, "1,256,1,or_greater"), 8);

	GLOBAL_DEF(PropertyInfo(Variant::INT, "rendering/lightmapping/bake_quality/low_quality_probe_ray_count", PROPERTY_HINT_RANGE, "1,4096,1,or_greater"), 64);
	GLOBAL_DEF(PropertyInfo(Variant::INT, "rendering/lightmapping/bake_quality/medium_quality_probe_ray_count", PROPERTY_HINT_RANGE, "1,4096,1,or_greater"), 256);
	GLOBAL_DEF(PropertyInfo(Variant::INT, "rendering/lightmapping/bake_quality/high_quality_probe_ray_count", PROPERTY_HINT_RANGE, "1,4096,1,or_greater"), 512);
	GLOBAL_DEF(PropertyInfo(Variant::INT, "rendering/lightmapping/bake_quality/ultra_quality_probe_ray_count", PROPERTY_HINT_RANGE, "1,4096,1,or_greater"), 2048);
	GLOBAL_DEF(PropertyInfo(Variant::INT, "rendering/lightmapping/bake_performance/max_rays_per_probe_pass", PROPERTY_HINT_RANGE, "1,256,1,or_greater"), 64);

	GLOBAL_DEF(PropertyInfo(Variant::INT, "rendering/lightmapping/denoising/denoiser", PROPERTY_HINT_ENUM, "JNLM,OIDN"), 0);
#ifndef _3D_DISABLED
	GDREGISTER_CLASS(LightmapperRD);
	Lightmapper::create_gpu = create_lightmapper_rd;
#endif
}

void uninitialize_lightmapper_rd_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
}

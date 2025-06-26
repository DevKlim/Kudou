#include "register_types.h"

#include "csharp_script.h"

#include "core/config/engine.h"

CSharpLanguage *script_language_cs = nullptr;
Ref<ResourceFormatLoaderCSharpScript> resource_loader_cs;
Ref<ResourceFormatSaverCSharpScript> resource_saver_cs;

MonoBind::GodotSharp *_godotsharp = nullptr;

void initialize_mono_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	GDREGISTER_CLASS(CSharpScript);

	_godotsharp = memnew(MonoBind::GodotSharp);

	script_language_cs = memnew(CSharpLanguage);
	script_language_cs->set_language_index(ScriptServer::get_language_count());
	ScriptServer::register_language(script_language_cs);

	if (GD_IS_CLASS_ENABLED(CSharpScript)) {
		resource_loader_cs.instantiate();
		ResourceLoader::add_resource_format_loader(resource_loader_cs);
		resource_saver_cs.instantiate();
		ResourceSaver::add_resource_format_saver(resource_saver_cs);
	}
}

void uninitialize_mono_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	ScriptServer::unregister_language(script_language_cs);

	if (script_language_cs) {
		memdelete(script_language_cs);
	}

	if (GD_IS_CLASS_ENABLED(CSharpScript)) {
		ResourceLoader::remove_resource_format_loader(resource_loader_cs);
		resource_loader_cs.unref();
		ResourceSaver::remove_resource_format_saver(resource_saver_cs);
		resource_saver_cs.unref();
	}

	if (_godotsharp) {
		memdelete(_godotsharp);
	}
}

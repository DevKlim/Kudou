#include "register_types.h"

#include "video_stream_theora.h"

#ifdef TOOLS_ENABLED
#include "editor/movie_writer_ogv.h"
#endif

static Ref<ResourceFormatLoaderTheora> resource_loader_theora;
#ifdef TOOLS_ENABLED
static MovieWriterOGV *writer_ogv = nullptr;
#endif

void initialize_theora_module(ModuleInitializationLevel p_level) {
	switch (p_level) {
		case MODULE_INITIALIZATION_LEVEL_SERVERS: {
#ifdef TOOLS_ENABLED
			if (GD_IS_CLASS_ENABLED(MovieWriterOGV)) {
				writer_ogv = memnew(MovieWriterOGV);
				MovieWriter::add_writer(writer_ogv);
			}
#endif
		} break;

		case MODULE_INITIALIZATION_LEVEL_SCENE: {
			resource_loader_theora.instantiate();
			ResourceLoader::add_resource_format_loader(resource_loader_theora, true);
			GDREGISTER_CLASS(VideoStreamTheora);
		} break;
		default:
			break;
	}
}

void uninitialize_theora_module(ModuleInitializationLevel p_level) {
	switch (p_level) {
		case MODULE_INITIALIZATION_LEVEL_SCENE: {
			ResourceLoader::remove_resource_format_loader(resource_loader_theora);
			resource_loader_theora.unref();
		} break;

		case MODULE_INITIALIZATION_LEVEL_SERVERS: {
#ifdef TOOLS_ENABLED
			if (GD_IS_CLASS_ENABLED(MovieWriterOGV)) {
				memdelete(writer_ogv);
			}
#endif
		} break;
		default:
			break;
	}
}

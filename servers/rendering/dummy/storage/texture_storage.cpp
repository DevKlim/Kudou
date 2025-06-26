#include "texture_storage.h"

using namespace RendererDummy;

TextureStorage *TextureStorage::singleton = nullptr;

TextureStorage::TextureStorage() {
	singleton = this;
}

TextureStorage::~TextureStorage() {
	singleton = nullptr;
}

#include "forward_id_storage.h"

using namespace RendererRD;

ForwardIDStorage *ForwardIDStorage::singleton = nullptr;

ForwardIDStorage::ForwardIDStorage() {
	singleton = this;
}

ForwardIDStorage::~ForwardIDStorage() {
	singleton = nullptr;
}

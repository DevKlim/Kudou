#include "dtls_server.h"

DTLSServer *DTLSServer::create(bool p_notify_postinitialize) {
	if (_create) {
		return _create(p_notify_postinitialize);
	}
	return nullptr;
}

bool DTLSServer::is_available() {
	return available;
}

void DTLSServer::_bind_methods() {
	ClassDB::bind_method(D_METHOD("setup", "server_options"), &DTLSServer::setup);
	ClassDB::bind_method(D_METHOD("take_connection", "udp_peer"), &DTLSServer::take_connection);
}

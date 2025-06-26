#include "packet_peer_dtls.h"

PacketPeerDTLS *PacketPeerDTLS::create(bool p_notify_postinitialize) {
	if (_create) {
		return _create(p_notify_postinitialize);
	}
	return nullptr;
}

bool PacketPeerDTLS::is_available() {
	return available;
}

void PacketPeerDTLS::_bind_methods() {
	ClassDB::bind_method(D_METHOD("poll"), &PacketPeerDTLS::poll);
	ClassDB::bind_method(D_METHOD("connect_to_peer", "packet_peer", "hostname", "client_options"), &PacketPeerDTLS::connect_to_peer, DEFVAL(Ref<TLSOptions>()));
	ClassDB::bind_method(D_METHOD("get_status"), &PacketPeerDTLS::get_status);
	ClassDB::bind_method(D_METHOD("disconnect_from_peer"), &PacketPeerDTLS::disconnect_from_peer);

	BIND_ENUM_CONSTANT(STATUS_DISCONNECTED);
	BIND_ENUM_CONSTANT(STATUS_HANDSHAKING);
	BIND_ENUM_CONSTANT(STATUS_CONNECTED);
	BIND_ENUM_CONSTANT(STATUS_ERROR);
	BIND_ENUM_CONSTANT(STATUS_ERROR_HOSTNAME_MISMATCH);
}

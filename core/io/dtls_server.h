#pragma once

#include "core/io/net_socket.h"
#include "core/io/packet_peer_dtls.h"

class DTLSServer : public RefCounted {
	GDCLASS(DTLSServer, RefCounted);

protected:
	static inline DTLSServer *(*_create)(bool p_notify_postinitialize) = nullptr;
	static void _bind_methods();

	static inline bool available = false;

public:
	static bool is_available();
	static DTLSServer *create(bool p_notify_postinitialize = true);

	virtual Error setup(Ref<TLSOptions> p_options) = 0;
	virtual void stop() = 0;
	virtual Ref<PacketPeerDTLS> take_connection(Ref<PacketPeerUDP> p_peer) = 0;

	DTLSServer() {}
};

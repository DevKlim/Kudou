#pragma once

#include "core/io/ip.h"
#include "core/io/net_socket.h"
#include "core/io/stream_peer.h"
#include "core/io/stream_peer_tcp.h"

class TCPServer : public RefCounted {
	GDCLASS(TCPServer, RefCounted);

protected:
	enum {
		MAX_PENDING_CONNECTIONS = 8
	};

	Ref<NetSocket> _sock;
	static void _bind_methods();

public:
	Error listen(uint16_t p_port, const IPAddress &p_bind_address = IPAddress("*"));
	int get_local_port() const;
	bool is_listening() const;
	bool is_connection_available() const;
	Ref<StreamPeerTCP> take_connection();

	void stop(); // Stop listening

	TCPServer();
	~TCPServer();
};

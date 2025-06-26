#pragma once

#include "core/io/net_socket.h"

#include <sys/socket.h>

class NetSocketWeb : public NetSocket {
protected:
	static NetSocket *_create_func();

public:
	static void make_default();

	virtual Error open(Type p_sock_type, IP::Type &ip_type) override { return ERR_UNAVAILABLE; }
	virtual void close() override {}
	virtual Error bind(IPAddress p_addr, uint16_t p_port) override { return ERR_UNAVAILABLE; }
	virtual Error listen(int p_max_pending) override { return ERR_UNAVAILABLE; }
	virtual Error connect_to_host(IPAddress p_host, uint16_t p_port) override { return ERR_UNAVAILABLE; }
	virtual Error poll(PollType p_type, int timeout) const override { return ERR_UNAVAILABLE; }
	virtual Error recv(uint8_t *p_buffer, int p_len, int &r_read) override { return ERR_UNAVAILABLE; }
	virtual Error recvfrom(uint8_t *p_buffer, int p_len, int &r_read, IPAddress &r_ip, uint16_t &r_port, bool p_peek = false) override { return ERR_UNAVAILABLE; }
	virtual Error send(const uint8_t *p_buffer, int p_len, int &r_sent) override { return ERR_UNAVAILABLE; }
	virtual Error sendto(const uint8_t *p_buffer, int p_len, int &r_sent, IPAddress p_ip, uint16_t p_port) override { return ERR_UNAVAILABLE; }
	virtual Ref<NetSocket> accept(IPAddress &r_ip, uint16_t &r_port) override { return Ref<NetSocket>(); }

	virtual bool is_open() const override { return false; }
	virtual int get_available_bytes() const override { return -1; }
	virtual Error get_socket_address(IPAddress *r_ip, uint16_t *r_port) const override { return ERR_UNAVAILABLE; }

	virtual Error set_broadcasting_enabled(bool p_enabled) override { return ERR_UNAVAILABLE; }
	virtual void set_blocking_enabled(bool p_enabled) override {}
	virtual void set_ipv6_only_enabled(bool p_enabled) override {}
	virtual void set_tcp_no_delay_enabled(bool p_enabled) override {}
	virtual void set_reuse_address_enabled(bool p_enabled) override {}
	virtual Error join_multicast_group(const IPAddress &p_multi_address, const String &p_if_name) override { return ERR_UNAVAILABLE; }
	virtual Error leave_multicast_group(const IPAddress &p_multi_address, const String &p_if_name) override { return ERR_UNAVAILABLE; }

	NetSocketWeb() {}
};

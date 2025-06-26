#include "net_socket_web.h"

NetSocket *NetSocketWeb::_create_func() {
	return memnew(NetSocketWeb);
}

void NetSocketWeb::make_default() {
	_create = _create_func;
}
